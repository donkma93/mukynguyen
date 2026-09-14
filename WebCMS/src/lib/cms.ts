import bcrypt from "bcryptjs";
import { getPool, sql } from "@/lib/db";
import { addCoins, getAccountCount, getOnlineCount } from "@/lib/game";

export type Giftcode = {
  id: number;
  code: string;
  description: string | null;
  rewardWc: number;
  rewardWp: number;
  rewardWg: number;
  maxUses: number;
  usedCount: number;
  expiresAt: Date | null;
  isActive: boolean;
  createdAt: Date;
};

export type CmsAdmin = {
  id: number;
  username: string;
  passwordHash: string;
  displayName: string | null;
  createdAt: Date;
};

function asBool(v: unknown): boolean {
  return v === true || v === 1 || v === "1";
}

function mapGiftcode(row: Record<string, unknown>): Giftcode {
  return {
    id: Number(row.id),
    code: String(row.code),
    description: (row.description as string | null) ?? null,
    rewardWc: Number(row.reward_wc ?? 0),
    rewardWp: Number(row.reward_wp ?? 0),
    rewardWg: Number(row.reward_wg ?? 0),
    maxUses: Number(row.max_uses ?? 0),
    usedCount: Number(row.used_count ?? 0),
    expiresAt: (row.expires_at as Date | null) ?? null,
    isActive: asBool(row.is_active),
    createdAt: row.created_at as Date,
  };
}

export async function listGiftcodes(limit = 200): Promise<Giftcode[]> {
  const pool = await getPool();
  const r = await pool
    .request()
    .input("limit", sql.Int, limit)
    .query(`
      SELECT TOP (@limit) id, code, description, reward_wc, reward_wp, reward_wg,
             max_uses, used_count, expires_at, is_active, created_at
      FROM cms.giftcodes
      ORDER BY created_at DESC
    `);
  return r.recordset.map((row) => mapGiftcode(row as Record<string, unknown>));
}

export async function createGiftcode(input: {
  code: string;
  description?: string;
  rewardWc?: number;
  rewardWp?: number;
  rewardWg?: number;
  maxUses?: number;
  expiresAt?: Date | string | null;
  isActive?: boolean;
}): Promise<Giftcode> {
  const code = input.code.trim().toUpperCase();
  const pool = await getPool();
  const r = await pool
    .request()
    .input("code", sql.NVarChar(32), code)
    .input("description", sql.NVarChar(500), input.description ?? null)
    .input("wc", sql.Int, input.rewardWc ?? 0)
    .input("wp", sql.Int, input.rewardWp ?? 0)
    .input("wg", sql.Int, input.rewardWg ?? 0)
    .input("max_uses", sql.Int, input.maxUses ?? 1)
    .input(
      "expires_at",
      sql.DateTime,
      input.expiresAt ? new Date(input.expiresAt) : null
    )
    .input("is_active", sql.Bit, input.isActive === false ? 0 : 1)
    .query(`
      INSERT INTO cms.giftcodes (
        code, description, reward_wc, reward_wp, reward_wg, max_uses, used_count,
        expires_at, is_active, created_at
      )
      OUTPUT INSERTED.id, INSERTED.code, INSERTED.description, INSERTED.reward_wc,
             INSERTED.reward_wp, INSERTED.reward_wg, INSERTED.max_uses, INSERTED.used_count,
             INSERTED.expires_at, INSERTED.is_active, INSERTED.created_at
      VALUES (
        @code, @description, @wc, @wp, @wg, @max_uses, 0, @expires_at, @is_active, GETDATE()
      )
    `);
  return mapGiftcode(r.recordset[0] as Record<string, unknown>);
}

export async function updateGiftcode(
  id: number,
  input: {
    code?: string;
    description?: string | null;
    rewardWc?: number;
    rewardWp?: number;
    rewardWg?: number;
    maxUses?: number;
    expiresAt?: Date | string | null;
    isActive?: boolean;
  }
): Promise<Giftcode> {
  const pool = await getPool();
  const existing = await pool
    .request()
    .input("id", sql.Int, id)
    .query(`SELECT TOP 1 * FROM cms.giftcodes WHERE id = @id`);
  if (!existing.recordset[0]) throw new Error("Giftcode không tồn tại");

  const cur = existing.recordset[0] as Record<string, unknown>;
  const code =
    input.code !== undefined ? input.code.trim().toUpperCase() : String(cur.code);
  const description =
    input.description !== undefined
      ? input.description
      : ((cur.description as string | null) ?? null);
  const rewardWc = input.rewardWc ?? Number(cur.reward_wc ?? 0);
  const rewardWp = input.rewardWp ?? Number(cur.reward_wp ?? 0);
  const rewardWg = input.rewardWg ?? Number(cur.reward_wg ?? 0);
  const maxUses = input.maxUses ?? Number(cur.max_uses ?? 0);
  const expiresAt =
    input.expiresAt !== undefined
      ? input.expiresAt
        ? new Date(input.expiresAt)
        : null
      : ((cur.expires_at as Date | null) ?? null);
  const isActive =
    input.isActive !== undefined ? input.isActive : asBool(cur.is_active);

  const r = await pool
    .request()
    .input("id", sql.Int, id)
    .input("code", sql.NVarChar(32), code)
    .input("description", sql.NVarChar(500), description)
    .input("wc", sql.Int, rewardWc)
    .input("wp", sql.Int, rewardWp)
    .input("wg", sql.Int, rewardWg)
    .input("max_uses", sql.Int, maxUses)
    .input("expires_at", sql.DateTime, expiresAt)
    .input("is_active", sql.Bit, isActive ? 1 : 0)
    .query(`
      UPDATE cms.giftcodes
      SET code = @code,
          description = @description,
          reward_wc = @wc,
          reward_wp = @wp,
          reward_wg = @wg,
          max_uses = @max_uses,
          expires_at = @expires_at,
          is_active = @is_active
      OUTPUT INSERTED.id, INSERTED.code, INSERTED.description, INSERTED.reward_wc,
             INSERTED.reward_wp, INSERTED.reward_wg, INSERTED.max_uses, INSERTED.used_count,
             INSERTED.expires_at, INSERTED.is_active, INSERTED.created_at
      WHERE id = @id
    `);
  return mapGiftcode(r.recordset[0] as Record<string, unknown>);
}

export async function deleteGiftcode(id: number): Promise<void> {
  const pool = await getPool();
  await pool
    .request()
    .input("id", sql.Int, id)
    .query(`DELETE FROM cms.giftcode_claims WHERE giftcode_id = @id`);
  const r = await pool
    .request()
    .input("id", sql.Int, id)
    .query(`DELETE FROM cms.giftcodes WHERE id = @id`);
  if (!r.rowsAffected[0]) throw new Error("Giftcode không tồn tại");
}

export async function claimGiftcode(account: string, code: string) {
  const acc = account.trim().toLowerCase();
  const normalized = code.trim().toUpperCase();
  const pool = await getPool();
  const tx = new sql.Transaction(pool);
  await tx.begin();
  try {
    const found = await new sql.Request(tx)
      .input("code", sql.NVarChar(32), normalized)
      .query(`
        SELECT TOP 1 id, code, description, reward_wc, reward_wp, reward_wg,
               max_uses, used_count, expires_at, is_active, created_at
        FROM cms.giftcodes WITH (UPDLOCK, ROWLOCK)
        WHERE code = @code
      `);
    const row = found.recordset[0] as Record<string, unknown> | undefined;
    if (!row) throw new Error("Giftcode không tồn tại");
    if (!asBool(row.is_active)) throw new Error("Giftcode đã bị vô hiệu hóa");
    if (row.expires_at && new Date(row.expires_at as Date).getTime() < Date.now()) {
      throw new Error("Giftcode đã hết hạn");
    }
    const maxUses = Number(row.max_uses ?? 0);
    const usedCount = Number(row.used_count ?? 0);
    if (maxUses > 0 && usedCount >= maxUses) {
      throw new Error("Giftcode đã hết lượt sử dụng");
    }

    // Partner live/newbie codes must be claimed via /api/partner/claim (items + budget).
    try {
      const partnerCode = await new sql.Request(tx)
        .input("code", sql.NVarChar(32), normalized)
        .query(`
          SELECT TOP 1 id FROM cms.partner_sessions WHERE code = @code
        `);
      if (partnerCode.recordset[0]) {
        throw new Error(
          "Đây là mã quà đối tác — hãy nhận ở mục Quà đối tác và chọn nhân vật"
        );
      }
    } catch (e) {
      if (e instanceof Error && e.message.includes("Quà đối tác")) throw e;
      // partner_sessions may not exist yet on older DBs before ensurePartnerSchema
    }

    const claimed = await new sql.Request(tx)
      .input("gid", sql.Int, Number(row.id))
      .input("account", sql.VarChar(10), acc)
      .query(`
        SELECT TOP 1 id FROM cms.giftcode_claims
        WHERE giftcode_id = @gid AND account = @account
      `);
    if (claimed.recordset[0]) throw new Error("Bạn đã nhận giftcode này rồi");

    await new sql.Request(tx)
      .input("gid", sql.Int, Number(row.id))
      .input("account", sql.VarChar(10), acc)
      .query(`
        INSERT INTO cms.giftcode_claims (giftcode_id, account, claimed_at)
        VALUES (@gid, @account, GETDATE())
      `);

    await new sql.Request(tx)
      .input("gid", sql.Int, Number(row.id))
      .query(`
        UPDATE cms.giftcodes
        SET used_count = used_count + 1
        WHERE id = @gid
      `);

    const reward = {
      wc: Number(row.reward_wc ?? 0),
      wp: Number(row.reward_wp ?? 0),
      wg: Number(row.reward_wg ?? 0),
    };

    await tx.commit();

    if (reward.wc || reward.wp || reward.wg) {
      await addCoins(acc, reward);
    }

    return {
      code: String(row.code),
      reward,
      description: (row.description as string | null) ?? null,
    };
  } catch (e) {
    await tx.rollback();
    throw e;
  }
}

export async function findAdmin(username: string): Promise<CmsAdmin | null> {
  const pool = await getPool();
  const r = await pool
    .request()
    .input("username", sql.NVarChar(50), username.trim().toLowerCase())
    .query(`
      SELECT TOP 1 id, username, password_hash, display_name, created_at
      FROM cms.admins
      WHERE username = @username
    `);
  const row = r.recordset[0] as Record<string, unknown> | undefined;
  if (!row) return null;
  return {
    id: Number(row.id),
    username: String(row.username),
    passwordHash: String(row.password_hash),
    displayName: (row.display_name as string | null) ?? null,
    createdAt: row.created_at as Date,
  };
}

export async function ensureBootstrapAdmin(): Promise<void> {
  const username = (process.env.ADMIN_BOOTSTRAP_USER || "admin").trim().toLowerCase();
  const password = process.env.ADMIN_BOOTSTRAP_PASS || "admin123";
  const existing = await findAdmin(username);
  if (existing) return;

  const hash = await bcrypt.hash(password, 10);
  const pool = await getPool();
  await pool
    .request()
    .input("username", sql.NVarChar(50), username)
    .input("hash", sql.NVarChar(200), hash)
    .input("display", sql.NVarChar(100), username)
    .query(`
      IF NOT EXISTS (SELECT 1 FROM cms.admins WHERE username = @username)
        INSERT INTO cms.admins (username, password_hash, display_name, created_at)
        VALUES (@username, @hash, @display, GETDATE())
    `);
}

export async function verifyAdminPassword(username: string, password: string) {
  await ensureBootstrapAdmin();
  const admin = await findAdmin(username);
  if (!admin) return { ok: false as const, reason: "Sai tài khoản hoặc mật khẩu admin" };
  const match = await bcrypt.compare(password, admin.passwordHash);
  if (!match) return { ok: false as const, reason: "Sai tài khoản hoặc mật khẩu admin" };
  return {
    ok: true as const,
    username: admin.username,
    displayName: admin.displayName || admin.username,
  };
}

export async function getAdminStats() {
  const [accounts, online] = await Promise.all([
    getAccountCount(),
    getOnlineCount(),
  ]);

  return {
    accounts,
    online,
  };
}
