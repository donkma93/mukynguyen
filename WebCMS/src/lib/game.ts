import { getPool, sql } from "@/lib/db";
import {
  buildGiveItemBytes,
  insertItemIntoInventory,
  listBagItems,
  type GiveItemInput,
} from "@/lib/gs/inventory";

export type MembInfo = {
  memb___id: string;
  memb__pwd: string;
  bloc_code: string | null;
  AccountLevel: number | null;
  AccountExpireDate: Date | null;
  Lock: number | null;
  mail_addr: string | null;
};

export type CashData = {
  AccountID: string;
  WC: number;
  WP: number;
  WG: number;
  RD: number;
  AT: number;
};

export type CharacterRow = {
  Name: string;
  cLevel: number;
  Class: number;
  ResetCount: number;
  MasterResetCount: number;
  Money: number;
  Kills: number;
  Deads: number;
  MapNumber: number | null;
};

export type CharacterDetail = CharacterRow & {
  AccountID: string;
  LevelUpPoint: number;
  Strength: number;
  Dexterity: number;
  Vitality: number;
  Energy: number;
  Leadership: number;
  MapPosX: number | null;
  MapPosY: number | null;
  CtlCode: number;
  rDanhHieu: number;
  rQuanHam: number;
  rTuLuyen: number;
  rHonHoan: number;
  rNewVip: number;
  rHuyChuong: number;
};

export type RankRow = CharacterRow & { AccountID: string };

function normalizeAccount(account: string) {
  return account.trim().toLowerCase();
}

export async function findAccount(account: string): Promise<MembInfo | null> {
  const pool = await getPool();
  const r = await pool
    .request()
    .input("id", sql.VarChar(10), normalizeAccount(account))
    .query<MembInfo>(
      `SELECT TOP 1 memb___id, memb__pwd, bloc_code, AccountLevel, AccountExpireDate, Lock, mail_addr
       FROM dbo.MEMB_INFO WHERE memb___id = @id`
    );
  return r.recordset[0] ?? null;
}

export async function registerAccount(input: {
  account: string;
  password: string;
  email?: string;
}) {
  const account = normalizeAccount(input.account);
  const existing = await findAccount(account);
  if (existing) throw new Error("Tài khoản đã tồn tại");

  const email =
    input.email && input.email.length > 0
      ? input.email
      : `${account}@kynguyen.local`;

  const pool = await getPool();
  const tx = new sql.Transaction(pool);
  await tx.begin();
  try {
    const req = new sql.Request(tx);
    await req
      .input("id", sql.VarChar(10), account)
      .input("pwd", sql.VarChar(10), input.password)
      .input("email", sql.VarChar(50), email.slice(0, 50))
      .query(`
        INSERT INTO dbo.MEMB_INFO (
          memb___id, memb__pwd, memb_name, sno__numb, mail_addr,
          appl_days, mail_chek, bloc_code, ctl1_code,
          AccountLevel, AccountExpireDate, Lock
        ) VALUES (
          @id, @pwd, 'KyNguyen', '0968092399', @email,
          GETDATE(), '1', '0', '1',
          0, GETDATE(), 0
        )
      `);

    await new sql.Request(tx)
      .input("id", sql.VarChar(10), account)
      .query(`
        IF NOT EXISTS (SELECT 1 FROM dbo.CashShopData WHERE AccountID = @id)
          INSERT INTO dbo.CashShopData (AccountID, WC, WP, WG, RD, AT)
          VALUES (@id, 0, 0, 0, 0, 0)
      `);

    await new sql.Request(tx)
      .input("id", sql.VarChar(10), account)
      .query(`
        IF NOT EXISTS (SELECT 1 FROM dbo.ThangCuoi_MocNap WHERE Account = @id)
          INSERT INTO dbo.ThangCuoi_MocNap (Account, NhanMocNap, TienNap)
          VALUES (@id, 0, 0)
      `);

    await tx.commit();
    return { account };
  } catch (e) {
    await tx.rollback();
    throw e;
  }
}

export async function verifyGameLogin(account: string, password: string) {
  const row = await findAccount(account);
  if (!row) return { ok: false as const, reason: "Sai tài khoản hoặc mật khẩu" };
  if (row.bloc_code && String(row.bloc_code).trim() !== "0") {
    return { ok: false as const, reason: "Tài khoản đang bị khóa" };
  }
  if (row.Lock && Number(row.Lock) !== 0) {
    return { ok: false as const, reason: "Tài khoản đang bị khóa" };
  }
  if (row.memb__pwd !== password) {
    return { ok: false as const, reason: "Sai tài khoản hoặc mật khẩu" };
  }
  return { ok: true as const, account: row.memb___id, vip: row.AccountLevel ?? 0 };
}

export async function changePassword(account: string, oldPassword: string, newPassword: string) {
  const check = await verifyGameLogin(account, oldPassword);
  if (!check.ok) throw new Error(check.reason);
  const pool = await getPool();
  await pool
    .request()
    .input("id", sql.VarChar(10), normalizeAccount(account))
    .input("pwd", sql.VarChar(10), newPassword)
    .query(`UPDATE dbo.MEMB_INFO SET memb__pwd = @pwd, modi_days = GETDATE() WHERE memb___id = @id`);
}

export async function getCash(account: string): Promise<CashData> {
  const pool = await getPool();
  const id = normalizeAccount(account);
  const r = await pool
    .request()
    .input("id", sql.VarChar(10), id)
    .query<CashData>(`SELECT AccountID, WC, WP, WG, RD, AT FROM dbo.CashShopData WHERE AccountID = @id`);
  if (r.recordset[0]) return r.recordset[0];
  await pool
    .request()
    .input("id", sql.VarChar(10), id)
    .query(`INSERT INTO dbo.CashShopData (AccountID, WC, WP, WG, RD, AT) VALUES (@id, 0, 0, 0, 0, 0)`);
  return { AccountID: id, WC: 0, WP: 0, WG: 0, RD: 0, AT: 0 };
}

export async function addCoins(
  account: string,
  delta: { wc?: number; wp?: number; wg?: number; rd?: number; at?: number }
) {
  const id = normalizeAccount(account);
  await getCash(id);
  const pool = await getPool();
  await pool
    .request()
    .input("id", sql.VarChar(10), id)
    .input("wc", sql.Int, delta.wc ?? 0)
    .input("wp", sql.Int, delta.wp ?? 0)
    .input("wg", sql.Int, delta.wg ?? 0)
    .input("rd", sql.Int, delta.rd ?? 0)
    .input("at", sql.Int, delta.at ?? 0)
    .query(`
      UPDATE dbo.CashShopData
      SET WC = WC + @wc, WP = WP + @wp, WG = WG + @wg, RD = RD + @rd, AT = AT + @at
      WHERE AccountID = @id
    `);
}

export async function addTienNap(account: string, amountVnd: number) {
  const id = normalizeAccount(account);
  const pool = await getPool();
  await pool
    .request()
    .input("id", sql.VarChar(10), id)
    .query(`
      IF NOT EXISTS (SELECT 1 FROM dbo.ThangCuoi_MocNap WHERE Account = @id)
        INSERT INTO dbo.ThangCuoi_MocNap (Account, NhanMocNap, TienNap) VALUES (@id, 0, 0)
    `);
  await pool
    .request()
    .input("id", sql.VarChar(10), id)
    .input("amt", sql.Int, amountVnd)
    .query(`UPDATE dbo.ThangCuoi_MocNap SET TienNap = TienNap + @amt WHERE Account = @id`);
}

export async function getMocNap(account: string) {
  const pool = await getPool();
  const r = await pool
    .request()
    .input("id", sql.VarChar(10), normalizeAccount(account))
    .query<{ Account: string; NhanMocNap: number; TienNap: number }>(
      `SELECT Account, NhanMocNap, TienNap FROM dbo.ThangCuoi_MocNap WHERE Account = @id`
    );
  return r.recordset[0] ?? { Account: account, NhanMocNap: 0, TienNap: 0 };
}

export async function getCharacters(account: string): Promise<CharacterRow[]> {
  const pool = await getPool();
  const r = await pool
    .request()
    .input("id", sql.VarChar(10), normalizeAccount(account))
    .query<CharacterRow>(`
      SELECT Name, cLevel, Class, ISNULL(ResetCount,0) ResetCount,
             ISNULL(MasterResetCount,0) MasterResetCount, ISNULL(Money,0) Money,
             ISNULL(Kills,0) Kills, ISNULL(Deads,0) Deads, MapNumber
      FROM dbo.Character WHERE AccountID = @id
      ORDER BY ResetCount DESC, cLevel DESC
    `);
  return r.recordset;
}

const CHARACTER_DETAIL_SELECT = `
  AccountID, Name, ISNULL(cLevel,1) cLevel, ISNULL(Class,0) Class,
  ISNULL(LevelUpPoint,0) LevelUpPoint,
  ISNULL(Strength,0) Strength, ISNULL(Dexterity,0) Dexterity,
  ISNULL(Vitality,0) Vitality, ISNULL(Energy,0) Energy, ISNULL(Leadership,0) Leadership,
  ISNULL(ResetCount,0) ResetCount, ISNULL(MasterResetCount,0) MasterResetCount,
  ISNULL(Money,0) Money, ISNULL(Kills,0) Kills, ISNULL(Deads,0) Deads,
  MapNumber, MapPosX, MapPosY,
  ISNULL(CtlCode,0) CtlCode,
  ISNULL(rDanhHieu,0) rDanhHieu, ISNULL(rQuanHam,0) rQuanHam,
  ISNULL(rTuLuyen,0) rTuLuyen, ISNULL(rHonHoan,0) rHonHoan,
  ISNULL(rNewVip,0) rNewVip, ISNULL(rHuyChuong,0) rHuyChuong
`;

export async function getCharacterByName(name: string): Promise<CharacterDetail | null> {
  const pool = await getPool();
  const r = await pool
    .request()
    .input("name", sql.VarChar(10), name.trim())
    .query<CharacterDetail>(`
      SELECT TOP 1 ${CHARACTER_DETAIL_SELECT}
      FROM dbo.Character WHERE Name = @name
    `);
  return r.recordset[0] ?? null;
}

export async function searchCharacters(q: string, limit = 50): Promise<CharacterDetail[]> {
  const pool = await getPool();
  const term = `%${q.trim()}%`;
  const r = await pool
    .request()
    .input("q", sql.VarChar(20), term)
    .input("limit", sql.Int, limit)
    .query<CharacterDetail>(`
      SELECT TOP (@limit) ${CHARACTER_DETAIL_SELECT}
      FROM dbo.Character
      WHERE Name LIKE @q OR AccountID LIKE @q
      ORDER BY ResetCount DESC, cLevel DESC, Name
    `);
  return r.recordset;
}

export type CharacterUpdateInput = {
  Class?: number;
  cLevel?: number;
  LevelUpPoint?: number;
  Strength?: number;
  Dexterity?: number;
  Vitality?: number;
  Energy?: number;
  Leadership?: number;
  ResetCount?: number;
  MasterResetCount?: number;
  Money?: number;
  MapNumber?: number;
  MapPosX?: number;
  MapPosY?: number;
  CtlCode?: number;
  rDanhHieu?: number;
  rQuanHam?: number;
  rTuLuyen?: number;
  rHonHoan?: number;
  rNewVip?: number;
  rHuyChuong?: number;
  ResetPointBonusApplied?: boolean;
};

const RESET_POINT_BONUS = 300;
const RESET_START_LEVEL = 10;
// Matches gLevelExperience[9] in the running GameServer's level experience table.
const RESET_START_EXPERIENCE = 14_580;
const RESET_LORENCIA_MAP = 0;
const RESET_LORENCIA_X = 142;
const RESET_LORENCIA_Y = 126;

export async function updateCharacterStats(name: string, input: CharacterUpdateInput) {
  const current = await getCharacterByName(name);
  if (!current) throw new Error("Nhân vật không tồn tại");
  if (await isAccountOnline(current.AccountID)) {
    throw new Error("Nhân vật đang Online — hãy thoát game rồi mới cập nhật chỉ số");
  }

  const next = {
    Class: input.Class ?? current.Class,
    cLevel: input.cLevel ?? current.cLevel,
    LevelUpPoint: input.LevelUpPoint ?? current.LevelUpPoint,
    Strength: input.Strength ?? current.Strength,
    Dexterity: input.Dexterity ?? current.Dexterity,
    Vitality: input.Vitality ?? current.Vitality,
    Energy: input.Energy ?? current.Energy,
    Leadership: input.Leadership ?? current.Leadership,
    ResetCount: input.ResetCount ?? current.ResetCount,
    MasterResetCount: input.MasterResetCount ?? current.MasterResetCount,
    Money: input.Money ?? current.Money,
    MapNumber: input.MapNumber ?? current.MapNumber ?? 0,
    MapPosX: input.MapPosX ?? current.MapPosX ?? 125,
    MapPosY: input.MapPosY ?? current.MapPosY ?? 125,
    CtlCode: input.CtlCode ?? current.CtlCode,
    rDanhHieu: input.rDanhHieu ?? current.rDanhHieu,
    rQuanHam: input.rQuanHam ?? current.rQuanHam,
    rTuLuyen: input.rTuLuyen ?? current.rTuLuyen,
    rHonHoan: input.rHonHoan ?? current.rHonHoan,
    rNewVip: input.rNewVip ?? current.rNewVip,
    rHuyChuong: input.rHuyChuong ?? current.rHuyChuong,
  };

  const clamp = (v: number, min: number, max: number) => Math.max(min, Math.min(max, Math.trunc(v)));
  next.Class = clamp(next.Class, 0, 255);
  next.cLevel = clamp(next.cLevel, 1, 400);
  next.LevelUpPoint = clamp(next.LevelUpPoint, 0, 2_000_000_000);
  next.Strength = clamp(next.Strength, 0, 65000);
  next.Dexterity = clamp(next.Dexterity, 0, 65000);
  next.Vitality = clamp(next.Vitality, 0, 65000);
  next.Energy = clamp(next.Energy, 0, 65000);
  next.Leadership = clamp(next.Leadership, 0, 65000);
  next.ResetCount = clamp(next.ResetCount, 0, 200);
  const addedResetPoints = Math.max(0, next.ResetCount - current.ResetCount) * RESET_POINT_BONUS;
  if (addedResetPoints > 0) {
    next.cLevel = RESET_START_LEVEL;
    next.MapNumber = RESET_LORENCIA_MAP;
    next.MapPosX = RESET_LORENCIA_X;
    next.MapPosY = RESET_LORENCIA_Y;
  }
  if (input.ResetPointBonusApplied) {
    next.LevelUpPoint = current.LevelUpPoint;
  }
  next.LevelUpPoint = clamp(next.LevelUpPoint + addedResetPoints, 0, 2_000_000_000);
  next.MasterResetCount = clamp(next.MasterResetCount, 0, 1_000_000);
  next.Money = clamp(Number(next.Money), 0, 9_000_000_000_000_000);
  next.MapNumber = clamp(Number(next.MapNumber), 0, 255);
  next.MapPosX = clamp(Number(next.MapPosX), 0, 255);
  next.MapPosY = clamp(Number(next.MapPosY), 0, 255);
  next.CtlCode = clamp(next.CtlCode, 0, 255);
  next.rDanhHieu = clamp(next.rDanhHieu, 0, 2_000_000_000);
  next.rQuanHam = clamp(next.rQuanHam, 0, 2_000_000_000);
  next.rTuLuyen = clamp(next.rTuLuyen, 0, 2_000_000_000);
  next.rHonHoan = clamp(next.rHonHoan, 0, 2_000_000_000);
  next.rNewVip = clamp(next.rNewVip, 0, 2_000_000_000);
  next.rHuyChuong = clamp(next.rHuyChuong, 0, 2_000_000_000);

  const pool = await getPool();
  await pool
    .request()
    .input("name", sql.VarChar(10), name.trim())
    .input("cls", sql.TinyInt, next.Class)
    .input("cLevel", sql.Int, next.cLevel)
    .input("lup", sql.Int, next.LevelUpPoint)
    .input("str", sql.Int, next.Strength)
    .input("dex", sql.Int, next.Dexterity)
    .input("vit", sql.Int, next.Vitality)
    .input("ene", sql.Int, next.Energy)
    .input("cmd", sql.Int, next.Leadership)
    .input("reset", sql.Int, next.ResetCount)
    .input("experience", sql.Int, addedResetPoints > 0 ? RESET_START_EXPERIENCE : null)
    .input("mreset", sql.Int, next.MasterResetCount)
    .input("money", sql.BigInt, next.Money)
    .input("map", sql.SmallInt, next.MapNumber)
    .input("x", sql.SmallInt, next.MapPosX)
    .input("y", sql.SmallInt, next.MapPosY)
    .input("ctl", sql.TinyInt, next.CtlCode)
    .input("danhhieu", sql.Int, next.rDanhHieu)
    .input("quanham", sql.Int, next.rQuanHam)
    .input("tuluyen", sql.Int, next.rTuLuyen)
    .input("honhoan", sql.Int, next.rHonHoan)
    .input("newvip", sql.Int, next.rNewVip)
    .input("huychuong", sql.Int, next.rHuyChuong)
    .query(`
      UPDATE dbo.Character
      SET Class = @cls,
          cLevel = @cLevel,
          Experience = COALESCE(@experience, Experience),
          LevelUpPoint = @lup,
          Strength = @str,
          Dexterity = @dex,
          Vitality = @vit,
          Energy = @ene,
          Leadership = @cmd,
          ResetCount = @reset,
          MasterResetCount = @mreset,
          Money = @money,
          MapNumber = @map,
          MapPosX = @x,
          MapPosY = @y,
          CtlCode = @ctl,
          rDanhHieu = @danhhieu,
          rQuanHam = @quanham,
          rTuLuyen = @tuluyen,
          rHonHoan = @honhoan,
          rNewVip = @newvip,
          rHuyChuong = @huychuong
      WHERE Name = @name
    `);

  return getCharacterByName(name);
}

export async function getRanking(
  type: "reset" | "master" | "level" | "zen" | "kill",
  limit = 50
): Promise<RankRow[]> {
  const order: Record<typeof type, string> = {
    reset: "ISNULL(ResetCount,0) DESC, cLevel DESC",
    master: "ISNULL(MasterResetCount,0) DESC, ISNULL(ResetCount,0) DESC",
    level: "cLevel DESC, ISNULL(ResetCount,0) DESC",
    zen: "ISNULL(Money,0) DESC",
    kill: "ISNULL(Kills,0) DESC",
  };
  const pool = await getPool();
  const r = await pool.request().query<RankRow>(`
    SELECT TOP (${limit}) AccountID, Name, cLevel, Class,
           ISNULL(ResetCount,0) ResetCount, ISNULL(MasterResetCount,0) MasterResetCount,
           ISNULL(Money,0) Money, ISNULL(Kills,0) Kills, ISNULL(Deads,0) Deads, MapNumber
    FROM dbo.Character
    ORDER BY ${order[type]}
  `);
  return r.recordset;
}

export async function getOnlineCount() {
  const pool = await getPool();
  const r = await pool
    .request()
    .query<{ c: number }>(`SELECT COUNT(*) AS c FROM dbo.MEMB_STAT WHERE ConnectStat = 1`);
  return r.recordset[0]?.c ?? 0;
}

export async function getAccountCount() {
  const pool = await getPool();
  const r = await pool.request().query<{ c: number }>(`SELECT COUNT(*) AS c FROM dbo.MEMB_INFO`);
  return r.recordset[0]?.c ?? 0;
}

export async function setAccountBlock(account: string, blocked: boolean) {
  const pool = await getPool();
  await pool
    .request()
    .input("id", sql.VarChar(10), normalizeAccount(account))
    .input("bloc", sql.Char(1), blocked ? "1" : "0")
    .query(`UPDATE dbo.MEMB_INFO SET bloc_code = @bloc WHERE memb___id = @id`);
}

export async function setVip(account: string, level: number, expireDays: number) {
  const { clampVipLevel } = await import("@/lib/vip-shop");
  const lv = clampVipLevel(level);
  const pool = await getPool();
  await pool
    .request()
    .input("id", sql.VarChar(10), normalizeAccount(account))
    .input("lv", sql.Int, lv)
    .input("days", sql.Int, expireDays)
    .query(`
      UPDATE dbo.MEMB_INFO
      SET AccountLevel = @lv,
          AccountExpireDate = CASE WHEN @days > 0 THEN DATEADD(day, @days, GETDATE()) ELSE AccountExpireDate END
      WHERE memb___id = @id
    `);
}

export type PurchaseVipResult = {
  ok: true;
  packageId: string;
  days: number;
  priceWc: number;
  wcLeft: number;
  accountLevel: number;
  expireDate: Date | null;
};

/** Buy / extend single VIP (AccountLevel=1) with CashShopData.WC. */
export async function purchaseVipWithWcoin(
  account: string,
  packageId: string
): Promise<PurchaseVipResult> {
  const { getVipPackage } = await import("@/lib/vip-shop");
  const pack = getVipPackage(packageId);
  if (!pack) throw new Error("Gói VIP không hợp lệ");

  const id = normalizeAccount(account);
  await getCash(id);

  const pool = await getPool();
  const tx = new sql.Transaction(pool);
  await tx.begin();
  try {
    const cash = await new sql.Request(tx)
      .input("id", sql.VarChar(10), id)
      .query<{ WC: number }>(`
        SELECT WC FROM dbo.CashShopData WITH (UPDLOCK, ROWLOCK)
        WHERE AccountID = @id
      `);
    const wc = Number(cash.recordset[0]?.WC ?? 0);
    if (wc < pack.priceWc) {
      throw new Error(`Không đủ WCoin (cần ${pack.priceWc}, còn ${wc})`);
    }

    const deducted = await new sql.Request(tx)
      .input("id", sql.VarChar(10), id)
      .input("price", sql.Int, pack.priceWc)
      .query(`
        UPDATE dbo.CashShopData
        SET WC = WC - @price
        WHERE AccountID = @id AND WC >= @price
      `);
    if (Number(deducted.rowsAffected?.[0] ?? 0) < 1) {
      throw new Error(`Không đủ WCoin (cần ${pack.priceWc})`);
    }

    await new sql.Request(tx)
      .input("id", sql.VarChar(10), id)
      .input("days", sql.Int, pack.days)
      .query(`
        UPDATE dbo.MEMB_INFO
        SET AccountLevel = 1,
            AccountExpireDate = DATEADD(
              day,
              @days,
              CASE
                WHEN AccountExpireDate > GETDATE() AND ISNULL(AccountLevel, 0) >= 1
                THEN AccountExpireDate
                ELSE GETDATE()
              END
            )
        WHERE memb___id = @id
      `);

    const after = await new sql.Request(tx)
      .input("id", sql.VarChar(10), id)
      .query<{ WC: number; AccountLevel: number; AccountExpireDate: Date | null }>(`
        SELECT c.WC, m.AccountLevel, m.AccountExpireDate
        FROM dbo.CashShopData c
        INNER JOIN dbo.MEMB_INFO m ON m.memb___id = c.AccountID
        WHERE c.AccountID = @id
      `);

    await tx.commit();
    const row = after.recordset[0];
    return {
      ok: true,
      packageId: pack.id,
      days: pack.days,
      priceWc: pack.priceWc,
      wcLeft: Number(row?.WC ?? wc - pack.priceWc),
      accountLevel: Number(row?.AccountLevel ?? 1),
      expireDate: row?.AccountExpireDate ?? null,
    };
  } catch (e) {
    await tx.rollback();
    throw e;
  }
}

export async function searchAccounts(q: string, limit = 50) {
  const pool = await getPool();
  const r = await pool
    .request()
    .input("q", sql.VarChar(20), `%${normalizeAccount(q)}%`)
    .input("limit", sql.Int, limit)
    .query<MembInfo>(`
      SELECT TOP (@limit) memb___id, memb__pwd, bloc_code, AccountLevel, AccountExpireDate, Lock, mail_addr
      FROM dbo.MEMB_INFO
      WHERE memb___id LIKE @q
      ORDER BY memb___id
    `);
  return r.recordset;
}

export async function isAccountOnline(account: string): Promise<boolean> {
  const pool = await getPool();
  const r = await pool
    .request()
    .input("id", sql.VarChar(10), normalizeAccount(account))
    .query<{ ConnectStat: number }>(`
      SELECT TOP 1 ISNULL(ConnectStat,0) AS ConnectStat
      FROM dbo.MEMB_STAT
      WHERE memb___id = @id
    `);
  return Number(r.recordset[0]?.ConnectStat ?? 0) === 1;
}

export async function getNextItemSerial(): Promise<number> {
  const pool = await getPool();
  const r = await pool.request().query(`EXEC WZ_GetItemSerial`);
  const row = r.recordset?.[0] as Record<string, unknown> | undefined;
  if (!row) return Math.floor(Date.now() % 0x7fffffff);
  const first = Object.values(row)[0];
  const n = Number(first);
  return Number.isFinite(n) && n > 0 ? n >>> 0 : Math.floor(Date.now() % 0x7fffffff);
}

export async function getCharacterInventory(name: string): Promise<{
  account: string;
  name: string;
  extInventory: number;
  online: boolean;
  inventory: Buffer;
  bagItems: ReturnType<typeof listBagItems>;
} | null> {
  const pool = await getPool();
  const r = await pool
    .request()
    .input("name", sql.VarChar(10), name.trim())
    .query(`
      SELECT TOP 1 AccountID, Name, ISNULL(ExtInventory,0) AS ExtInventory, Inventory
      FROM dbo.Character
      WHERE Name = @name
    `);
  const row = r.recordset[0] as
    | { AccountID: string; Name: string; ExtInventory: number; Inventory: Buffer }
    | undefined;
  if (!row) return null;
  const online = await isAccountOnline(row.AccountID);
  const inventory = Buffer.isBuffer(row.Inventory)
    ? row.Inventory
    : Buffer.from(row.Inventory ?? []);
  return {
    account: row.AccountID,
    name: row.Name,
    extInventory: Number(row.ExtInventory ?? 0),
    online,
    inventory,
    bagItems: listBagItems(inventory, Number(row.ExtInventory ?? 0)),
  };
}

export async function giveItemToCharacter(name: string, input: GiveItemInput) {
  const pool = await getPool();
  const tx = new sql.Transaction(pool);
  await tx.begin();
  try {
    const existing = await new sql.Request(tx)
      .input("name", sql.VarChar(10), name.trim())
      .query(`
        SELECT TOP 1 AccountID, Name, ISNULL(ExtInventory,0) AS ExtInventory, Inventory
        FROM dbo.Character WITH (UPDLOCK, ROWLOCK)
        WHERE Name = @name
      `);
    const row = existing.recordset[0] as
      | { AccountID: string; Name: string; ExtInventory: number; Inventory: Buffer }
      | undefined;
    if (!row) throw new Error("Nhân vật không tồn tại");

    const onlineCheck = await new sql.Request(tx)
      .input("id", sql.VarChar(10), normalizeAccount(row.AccountID))
      .query<{ ConnectStat: number }>(`
        SELECT TOP 1 ISNULL(ConnectStat,0) AS ConnectStat
        FROM dbo.MEMB_STAT
        WHERE memb___id = @id
      `);
    if (Number(onlineCheck.recordset[0]?.ConnectStat ?? 0) === 1) {
      throw new Error(
        "Nhân vật đang Online — hãy thoát game (offline) rồi mới set đồ"
      );
    }

    const serialRes = await new sql.Request(tx).query(`EXEC WZ_GetItemSerial`);
    const serialRow = serialRes.recordset?.[0] as Record<string, unknown> | undefined;
    const serialRaw = serialRow ? Number(Object.values(serialRow)[0]) : 0;
    const serial =
      Number.isFinite(serialRaw) && serialRaw > 0
        ? serialRaw >>> 0
        : Math.floor(Date.now() % 0x7fffffff);

    const built = buildGiveItemBytes(input, serial);
    const invRaw = Buffer.isBuffer(row.Inventory)
      ? row.Inventory
      : Buffer.from(row.Inventory ?? []);
    const inserted = insertItemIntoInventory(
      invRaw,
      Number(row.ExtInventory ?? 0),
      built.bytes,
      built.info.width,
      built.info.height
    );

    await new sql.Request(tx)
      .input("name", sql.VarChar(10), row.Name)
      .input("inv", sql.VarBinary(3776), inserted.inventory)
      .query(`
        UPDATE dbo.Character
        SET Inventory = @inv
        WHERE Name = @name
      `);

    await tx.commit();

    return {
      account: row.AccountID,
      name: row.Name,
      slot: inserted.slot,
      item: {
        name: built.info.name,
        section: built.info.section,
        type: built.info.type,
        width: built.info.width,
        height: built.info.height,
        ...built.meta,
      },
      bagItems: listBagItems(inserted.inventory, Number(row.ExtInventory ?? 0)),
    };
  } catch (e) {
    try {
      await tx.rollback();
    } catch {
      // ignore
    }
    throw e;
  }
}

export { CHARACTER_CLASS_OPTIONS, className } from "@/lib/character-classes";
