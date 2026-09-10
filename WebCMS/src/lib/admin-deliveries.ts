import { getPool, sql } from "@/lib/db";
import { getCharacterByName } from "@/lib/game";

export type ItemDeliveryStatus = "pending" | "dispatched" | "cancelled";

export type AdminItemDelivery = {
  id: number;
  characterName: string;
  accountId: string;
  itemIndex: number;
  itemLevel: number;
  durability: number;
  skill: boolean;
  luck: boolean;
  option: number;
  excellent: number;
  ancient: number;
  socketCount: number;
  durationSeconds: number;
  quantity: number;
  status: ItemDeliveryStatus;
  note: string | null;
  createdBy: string | null;
  createdAt: Date;
  dispatchedAt: Date | null;
};

function mapDelivery(row: Record<string, unknown>): AdminItemDelivery {
  return {
    id: Number(row.id),
    characterName: String(row.character_name),
    accountId: String(row.account_id),
    itemIndex: Number(row.item_index),
    itemLevel: Number(row.item_level),
    durability: Number(row.durability),
    skill: Boolean(row.skill),
    luck: Boolean(row.luck),
    option: Number(row.item_option),
    excellent: Number(row.excellent),
    ancient: Number(row.ancient),
    socketCount: Number(row.socket_count),
    durationSeconds: Number(row.duration_seconds),
    quantity: Number(row.quantity),
    status: String(row.status) as ItemDeliveryStatus,
    note: (row.note as string | null) ?? null,
    createdBy: (row.created_by as string | null) ?? null,
    createdAt: row.created_at as Date,
    dispatchedAt: (row.dispatched_at as Date | null) ?? null,
  };
}

const DELIVERY_COLUMNS = `
  id, character_name, account_id, item_index, item_level, durability,
  skill, luck, item_option, excellent, ancient, socket_count,
  duration_seconds, quantity, status, note, created_by, created_at, dispatched_at
`;
const DELIVERY_OUTPUT_COLUMNS = DELIVERY_COLUMNS.replace(/\s+/g, " ")
  .trim()
  .split(",")
  .map((column) => `INSERTED.${column.trim()}`)
  .join(", ");

/** Creates the GM delivery queue on existing game databases without a manual migration. */
export async function ensureAdminItemDeliverySchema(): Promise<void> {
  const pool = await getPool();
  await pool.request().query(`
    IF OBJECT_ID(N'cms.admin_item_deliveries', N'U') IS NULL
    BEGIN
      CREATE TABLE cms.admin_item_deliveries (
        id INT IDENTITY(1,1) NOT NULL PRIMARY KEY,
        character_name VARCHAR(10) NOT NULL,
        account_id VARCHAR(10) NOT NULL,
        item_index INT NOT NULL,
        item_level TINYINT NOT NULL,
        durability TINYINT NOT NULL,
        skill BIT NOT NULL CONSTRAINT DF_cms_admin_item_deliveries_skill DEFAULT 0,
        luck BIT NOT NULL CONSTRAINT DF_cms_admin_item_deliveries_luck DEFAULT 0,
        item_option TINYINT NOT NULL CONSTRAINT DF_cms_admin_item_deliveries_option DEFAULT 0,
        excellent TINYINT NOT NULL CONSTRAINT DF_cms_admin_item_deliveries_excellent DEFAULT 0,
        ancient TINYINT NOT NULL CONSTRAINT DF_cms_admin_item_deliveries_ancient DEFAULT 0,
        socket_count TINYINT NOT NULL CONSTRAINT DF_cms_admin_item_deliveries_socket_count DEFAULT 0,
        duration_seconds INT NOT NULL CONSTRAINT DF_cms_admin_item_deliveries_duration_seconds DEFAULT 0,
        quantity TINYINT NOT NULL CONSTRAINT DF_cms_admin_item_deliveries_quantity DEFAULT 1,
        status VARCHAR(16) NOT NULL CONSTRAINT DF_cms_admin_item_deliveries_status DEFAULT 'pending',
        note NVARCHAR(300) NULL,
        created_by NVARCHAR(100) NULL,
        created_at DATETIME NOT NULL CONSTRAINT DF_cms_admin_item_deliveries_created_at DEFAULT GETDATE(),
        dispatched_at DATETIME NULL
      );
      CREATE INDEX IX_cms_admin_item_deliveries_pending
        ON cms.admin_item_deliveries (character_name, account_id, status, created_at);
    END
  `);
}

export async function listAdminItemDeliveries(characterName?: string, limit = 100) {
  await ensureAdminItemDeliverySchema();
  const pool = await getPool();
  const r = await pool
    .request()
    .input("name", sql.VarChar(10), characterName?.trim() || null)
    .input("limit", sql.Int, Math.max(1, Math.min(200, Math.trunc(limit))))
    .query(`
      SELECT TOP (@limit) ${DELIVERY_COLUMNS}
      FROM cms.admin_item_deliveries
      WHERE @name IS NULL OR character_name = @name
      ORDER BY CASE WHEN status = 'pending' THEN 0 ELSE 1 END, created_at DESC
    `);
  return r.recordset.map((row) => mapDelivery(row as Record<string, unknown>));
}

export type QueueItemDeliveryInput = {
  characterName: string;
  itemIndex: number;
  itemLevel: number;
  durability: number;
  skill: boolean;
  luck: boolean;
  option: number;
  excellent: number;
  ancient: number;
  socketCount: number;
  durationSeconds: number;
  quantity: number;
  note?: string;
  createdBy?: string;
};

const clamp = (value: number, min: number, max: number) =>
  Math.max(min, Math.min(max, Math.trunc(value)));

export async function queueItemDelivery(input: QueueItemDeliveryInput) {
  const characterName = input.characterName.trim();
  const character = await getCharacterByName(characterName);
  if (!character) throw new Error("Nhân vật không tồn tại");

  const values = {
    itemIndex: clamp(input.itemIndex, 0, 8191),
    itemLevel: clamp(input.itemLevel, 0, 15),
    durability: clamp(input.durability, 0, 255),
    option: clamp(input.option, 0, 7),
    excellent: clamp(input.excellent, 0, 63),
    ancient: clamp(input.ancient, 0, 255),
    socketCount: clamp(input.socketCount, 0, 5),
    durationSeconds: clamp(input.durationSeconds, 0, 2_147_483_647),
    quantity: clamp(input.quantity, 1, 25),
  };

  await ensureAdminItemDeliverySchema();
  const pool = await getPool();
  const r = await pool
    .request()
    .input("character", sql.VarChar(10), character.Name)
    .input("account", sql.VarChar(10), character.AccountID)
    .input("itemIndex", sql.Int, values.itemIndex)
    .input("itemLevel", sql.TinyInt, values.itemLevel)
    .input("durability", sql.TinyInt, values.durability)
    .input("skill", sql.Bit, input.skill ? 1 : 0)
    .input("luck", sql.Bit, input.luck ? 1 : 0)
    .input("option", sql.TinyInt, values.option)
    .input("excellent", sql.TinyInt, values.excellent)
    .input("ancient", sql.TinyInt, values.ancient)
    .input("socketCount", sql.TinyInt, values.socketCount)
    .input("duration", sql.Int, values.durationSeconds)
    .input("quantity", sql.TinyInt, values.quantity)
    .input("note", sql.NVarChar(300), input.note?.trim().slice(0, 300) || null)
    .input("createdBy", sql.NVarChar(100), input.createdBy?.trim().slice(0, 100) || null)
    .query(`
      INSERT INTO cms.admin_item_deliveries (
        character_name, account_id, item_index, item_level, durability,
        skill, luck, item_option, excellent, ancient, socket_count,
        duration_seconds, quantity, status, note, created_by, created_at
      )
      OUTPUT ${DELIVERY_OUTPUT_COLUMNS}
      VALUES (
        @character, @account, @itemIndex, @itemLevel, @durability,
        @skill, @luck, @option, @excellent, @ancient, @socketCount,
        @duration, @quantity, 'pending', @note, @createdBy, GETDATE()
      )
    `);
  return mapDelivery(r.recordset[0] as Record<string, unknown>);
}

export async function cancelItemDelivery(id: number) {
  await ensureAdminItemDeliverySchema();
  const pool = await getPool();
  const r = await pool
    .request()
    .input("id", sql.Int, id)
    .query(`
      UPDATE cms.admin_item_deliveries
      SET status = 'cancelled'
      OUTPUT ${DELIVERY_OUTPUT_COLUMNS}
      WHERE id = @id AND status = 'pending'
    `);
  if (!r.recordset[0]) throw new Error("Chỉ có thể hủy lệnh đang chờ");
  return mapDelivery(r.recordset[0] as Record<string, unknown>);
}
