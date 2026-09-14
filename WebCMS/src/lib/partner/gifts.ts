import { randomBytes } from "crypto";
import { createGiftcode } from "@/lib/cms";
import { getPool, sql } from "@/lib/db";
import { queueItemDelivery } from "@/lib/admin-deliveries";
import {
  addCoins,
  findAccount,
  getCharacterByName,
} from "@/lib/game";
import {
  HIGHLIGHT_PER_LIVE_SESSION,
  NEWBIE_COOLDOWN_MS,
  PARTNER_PACKAGES,
  packageCoinCost,
  type PartnerPackageId,
  vietnamDayStartUtc,
} from "@/lib/partner/catalog";
import {
  assertCanAfford,
  consumeBudget,
  getOrCreateBudgetMonth,
} from "@/lib/partner/budget";
import { getPartner } from "@/lib/partner/partners";
import { ensurePartnerSchema } from "@/lib/partner/schema";

export type PartnerSession = {
  id: number;
  partnerAccount: string;
  packageId: PartnerPackageId;
  giftcodeId: number | null;
  code: string;
  maxUses: number;
  highlightCount: number;
  expiresAt: Date | null;
  isActive: boolean;
  createdAt: Date;
};

export type PartnerClaim = {
  id: number;
  partnerAccount: string;
  packageId: PartnerPackageId;
  sessionId: number | null;
  claimerAccount: string;
  claimerCharacter: string;
  wc: number;
  wp: number;
  wg: number;
  note: string | null;
  claimedAt: Date;
};

function asBool(v: unknown): boolean {
  return v === true || v === 1 || v === "1";
}

function mapSession(row: Record<string, unknown>): PartnerSession {
  return {
    id: Number(row.id),
    partnerAccount: String(row.partner_account).trim().toLowerCase(),
    packageId: String(row.package_id) as PartnerPackageId,
    giftcodeId: row.giftcode_id == null ? null : Number(row.giftcode_id),
    code: String(row.code),
    maxUses: Number(row.max_uses ?? 0),
    highlightCount: Number(row.highlight_count ?? 0),
    expiresAt: (row.expires_at as Date | null) ?? null,
    isActive: asBool(row.is_active),
    createdAt: row.created_at as Date,
  };
}

function mapClaim(row: Record<string, unknown>): PartnerClaim {
  return {
    id: Number(row.id),
    partnerAccount: String(row.partner_account).trim().toLowerCase(),
    packageId: String(row.package_id) as PartnerPackageId,
    sessionId: row.session_id == null ? null : Number(row.session_id),
    claimerAccount: String(row.claimer_account).trim().toLowerCase(),
    claimerCharacter: String(row.claimer_character),
    wc: Number(row.wc ?? 0),
    wp: Number(row.wp ?? 0),
    wg: Number(row.wg ?? 0),
    note: (row.note as string | null) ?? null,
    claimedAt: row.claimed_at as Date,
  };
}

function normalizeAccount(account: string) {
  return account.trim().toLowerCase();
}

async function requireActivePartner(account: string) {
  const partner = await getPartner(account);
  if (!partner || !partner.isActive) {
    throw new Error("Tài khoản không phải đối tác đang hoạt động");
  }
  return partner;
}

async function assertCharacterOwned(characterName: string, account: string) {
  const character = await getCharacterByName(characterName.trim());
  if (!character) throw new Error("Nhân vật không tồn tại");
  if (normalizeAccount(character.AccountID) !== normalizeAccount(account)) {
    throw new Error("Nhân vật không thuộc tài khoản này");
  }
  return character;
}

function makePartnerCode(prefix: string) {
  const token = randomBytes(4).toString("hex").toUpperCase();
  return `${prefix}-${token}`.slice(0, 32);
}

async function insertSession(input: {
  partnerAccount: string;
  packageId: PartnerPackageId;
  giftcodeId: number;
  code: string;
  maxUses: number;
  expiresAt: Date | null;
}): Promise<PartnerSession> {
  await ensurePartnerSchema();
  const pool = await getPool();
  const r = await pool
    .request()
    .input("partner", sql.VarChar(10), normalizeAccount(input.partnerAccount))
    .input("packageId", sql.VarChar(16), input.packageId)
    .input("giftcodeId", sql.Int, input.giftcodeId)
    .input("code", sql.NVarChar(32), input.code)
    .input("maxUses", sql.Int, input.maxUses)
    .input("expires", sql.DateTime, input.expiresAt)
    .query(`
      INSERT INTO cms.partner_sessions (
        partner_account, package_id, giftcode_id, code, max_uses,
        highlight_count, expires_at, is_active, created_at
      )
      OUTPUT INSERTED.id, INSERTED.partner_account, INSERTED.package_id,
             INSERTED.giftcode_id, INSERTED.code, INSERTED.max_uses,
             INSERTED.highlight_count, INSERTED.expires_at, INSERTED.is_active,
             INSERTED.created_at
      VALUES (
        @partner, @packageId, @giftcodeId, @code, @maxUses,
        0, @expires, 1, GETDATE()
      )
    `);
  return mapSession(r.recordset[0] as Record<string, unknown>);
}

async function queuePackageItems(
  characterName: string,
  packageId: PartnerPackageId,
  createdBy: string
) {
  const pack = PARTNER_PACKAGES[packageId];
  for (const item of pack.items) {
    await queueItemDelivery({
      characterName,
      itemIndex: item.itemIndex,
      itemLevel: 0,
      durability: 1,
      skill: false,
      luck: false,
      option: 0,
      excellent: 0,
      ancient: 0,
      socketCount: 0,
      durationSeconds: item.durationSeconds,
      quantity: item.quantity,
      note: `partner:${packageId}`,
      createdBy,
    });
  }
}

export async function listPartnerSessions(partnerAccount: string, limit = 20) {
  await ensurePartnerSchema();
  const pool = await getPool();
  const r = await pool
    .request()
    .input("partner", sql.VarChar(10), normalizeAccount(partnerAccount))
    .input("limit", sql.Int, Math.max(1, Math.min(100, Math.trunc(limit))))
    .query(`
      SELECT TOP (@limit) id, partner_account, package_id, giftcode_id, code,
             max_uses, highlight_count, expires_at, is_active, created_at
      FROM cms.partner_sessions
      WHERE partner_account = @partner
      ORDER BY created_at DESC
    `);
  return r.recordset.map((row) => mapSession(row as Record<string, unknown>));
}

export async function listPartnerClaims(partnerAccount: string, limit = 50) {
  await ensurePartnerSchema();
  const pool = await getPool();
  const r = await pool
    .request()
    .input("partner", sql.VarChar(10), normalizeAccount(partnerAccount))
    .input("limit", sql.Int, Math.max(1, Math.min(200, Math.trunc(limit))))
    .query(`
      SELECT TOP (@limit) id, partner_account, package_id, session_id,
             claimer_account, claimer_character, wc, wp, wg, note, claimed_at
      FROM cms.partner_claims
      WHERE partner_account = @partner
      ORDER BY claimed_at DESC
    `);
  return r.recordset.map((row) => mapClaim(row as Record<string, unknown>));
}

export async function getActiveLiveSession(partnerAccount: string) {
  await ensurePartnerSchema();
  const pool = await getPool();
  const r = await pool
    .request()
    .input("partner", sql.VarChar(10), normalizeAccount(partnerAccount))
    .query(`
      SELECT TOP 1 id, partner_account, package_id, giftcode_id, code,
             max_uses, highlight_count, expires_at, is_active, created_at
      FROM cms.partner_sessions
      WHERE partner_account = @partner
        AND package_id = 'LIVE_DROP'
        AND is_active = 1
        AND (expires_at IS NULL OR expires_at > GETDATE())
      ORDER BY created_at DESC
    `);
  const row = r.recordset[0] as Record<string, unknown> | undefined;
  return row ? mapSession(row) : null;
}

export async function createLiveSession(partnerAccount: string) {
  const partner = await requireActivePartner(partnerAccount);
  const used = await getOrCreateBudgetMonth(partner.account);
  assertCanAfford(partner.tier, used, { liveSessions: 1 });

  const pack = PARTNER_PACKAGES.LIVE_DROP;
  const expiresAt = pack.ttlMs ? new Date(Date.now() + pack.ttlMs) : null;
  let lastError: unknown = null;

  for (let attempt = 0; attempt < 5; attempt++) {
    const code = makePartnerCode("LIVE");
    try {
      const gift = await createGiftcode({
        code,
        description: `Partner LIVE_DROP · ${partner.account}`,
        rewardWc: pack.wc,
        rewardWp: pack.wp,
        rewardWg: pack.wg,
        maxUses: pack.maxUses,
        expiresAt,
        isActive: true,
      });

      const pool = await getPool();
      const tx = new sql.Transaction(pool);
      await tx.begin();
      try {
        await consumeBudget(partner.account, partner.tier, { liveSessions: 1 }, tx);
        const sessionRes = await new sql.Request(tx)
          .input("partner", sql.VarChar(10), partner.account)
          .input("packageId", sql.VarChar(16), "LIVE_DROP")
          .input("giftcodeId", sql.Int, gift.id)
          .input("code", sql.NVarChar(32), gift.code)
          .input("maxUses", sql.Int, pack.maxUses)
          .input("expires", sql.DateTime, expiresAt)
          .query(`
            INSERT INTO cms.partner_sessions (
              partner_account, package_id, giftcode_id, code, max_uses,
              highlight_count, expires_at, is_active, created_at
            )
            OUTPUT INSERTED.id, INSERTED.partner_account, INSERTED.package_id,
                   INSERTED.giftcode_id, INSERTED.code, INSERTED.max_uses,
                   INSERTED.highlight_count, INSERTED.expires_at, INSERTED.is_active,
                   INSERTED.created_at
            VALUES (
              @partner, @packageId, @giftcodeId, @code, @maxUses,
              0, @expires, 1, GETDATE()
            )
          `);
        await tx.commit();
        return {
          session: mapSession(sessionRes.recordset[0] as Record<string, unknown>),
          gift,
        };
      } catch (e) {
        await tx.rollback();
        throw e;
      }
    } catch (e) {
      lastError = e;
      const msg = e instanceof Error ? e.message : String(e);
      if (!/unique|duplicate|UQ_/i.test(msg)) throw e;
    }
  }
  throw lastError instanceof Error
    ? lastError
    : new Error("Không tạo được mã Live Drop");
}

export async function createNewbieSession(partnerAccount: string) {
  const partner = await requireActivePartner(partnerAccount);
  const pack = PARTNER_PACKAGES.NEWBIE;
  const expiresAt = pack.ttlMs ? new Date(Date.now() + pack.ttlMs) : null;

  // Deactivate previous active NEWBIE sessions for this partner
  await ensurePartnerSchema();
  const pool = await getPool();
  await pool
    .request()
    .input("partner", sql.VarChar(10), partner.account)
    .query(`
      UPDATE cms.partner_sessions
      SET is_active = 0
      WHERE partner_account = @partner AND package_id = 'NEWBIE' AND is_active = 1
    `);

  let lastError: unknown = null;
  for (let attempt = 0; attempt < 5; attempt++) {
    const code = makePartnerCode("NEWBIE");
    try {
      const gift = await createGiftcode({
        code,
        description: `Partner NEWBIE · ${partner.account}`,
        rewardWc: pack.wc,
        rewardWp: pack.wp,
        rewardWg: pack.wg,
        maxUses: pack.maxUses,
        expiresAt,
        isActive: true,
      });
      const session = await insertSession({
        partnerAccount: partner.account,
        packageId: "NEWBIE",
        giftcodeId: gift.id,
        code: gift.code,
        maxUses: pack.maxUses,
        expiresAt,
      });
      return { session, gift };
    } catch (e) {
      lastError = e;
      const msg = e instanceof Error ? e.message : String(e);
      if (!/unique|duplicate|UQ_/i.test(msg)) throw e;
    }
  }
  throw lastError instanceof Error
    ? lastError
    : new Error("Không tạo được mã Newbie");
}

async function hasLiveDropToday(claimerAccount: string): Promise<boolean> {
  const dayStart = vietnamDayStartUtc();
  const pool = await getPool();
  const r = await pool
    .request()
    .input("account", sql.VarChar(10), normalizeAccount(claimerAccount))
    .input("since", sql.DateTime, dayStart)
    .query(`
      SELECT TOP 1 id FROM cms.partner_claims
      WHERE claimer_account = @account
        AND package_id = 'LIVE_DROP'
        AND claimed_at >= @since
    `);
  return Boolean(r.recordset[0]);
}

async function hasHighlightToday(claimerAccount: string): Promise<boolean> {
  const dayStart = vietnamDayStartUtc();
  const pool = await getPool();
  const r = await pool
    .request()
    .input("account", sql.VarChar(10), normalizeAccount(claimerAccount))
    .input("since", sql.DateTime, dayStart)
    .query(`
      SELECT TOP 1 id FROM cms.partner_claims
      WHERE claimer_account = @account
        AND package_id = 'HIGHLIGHT'
        AND claimed_at >= @since
    `);
  return Boolean(r.recordset[0]);
}

async function hasRecentNewbie(
  partnerAccount: string,
  claimerAccount: string
): Promise<boolean> {
  const since = new Date(Date.now() - NEWBIE_COOLDOWN_MS);
  const pool = await getPool();
  const r = await pool
    .request()
    .input("partner", sql.VarChar(10), normalizeAccount(partnerAccount))
    .input("account", sql.VarChar(10), normalizeAccount(claimerAccount))
    .input("since", sql.DateTime, since)
    .query(`
      SELECT TOP 1 id FROM cms.partner_claims
      WHERE partner_account = @partner
        AND claimer_account = @account
        AND package_id = 'NEWBIE'
        AND claimed_at >= @since
    `);
  return Boolean(r.recordset[0]);
}

export async function claimPartnerCode(input: {
  claimerAccount: string;
  code: string;
  characterName: string;
}) {
  await ensurePartnerSchema();
  const claimer = normalizeAccount(input.claimerAccount);
  const code = input.code.trim().toUpperCase();
  const character = await assertCharacterOwned(input.characterName, claimer);

  const pool = await getPool();
  const tx = new sql.Transaction(pool);
  await tx.begin();

  let partnerAccount = "";
  let packageId: PartnerPackageId = "LIVE_DROP";
  let sessionId: number | null = null;
  let coins = { wc: 0, wp: 0, wg: 0 };

  try {
    const sessionRes = await new sql.Request(tx)
      .input("code", sql.NVarChar(32), code)
      .query(`
        SELECT TOP 1 s.id, s.partner_account, s.package_id, s.giftcode_id,
               s.code, s.max_uses, s.highlight_count, s.expires_at, s.is_active,
               s.created_at,
               g.reward_wc, g.reward_wp, g.reward_wg, g.used_count, g.max_uses AS g_max_uses,
               g.expires_at AS g_expires_at, g.is_active AS g_is_active
        FROM cms.partner_sessions s
        INNER JOIN cms.giftcodes g ON g.id = s.giftcode_id
        WHERE s.code = @code
      `);
    const row = sessionRes.recordset[0] as Record<string, unknown> | undefined;
    if (!row) throw new Error("Mã quà đối tác không tồn tại");
    if (!asBool(row.is_active) || !asBool(row.g_is_active)) {
      throw new Error("Mã quà đối tác đã bị vô hiệu hóa");
    }
    const expiresAt = (row.expires_at as Date | null) ?? (row.g_expires_at as Date | null);
    if (expiresAt && new Date(expiresAt).getTime() < Date.now()) {
      throw new Error("Mã quà đối tác đã hết hạn");
    }

    partnerAccount = normalizeAccount(String(row.partner_account));
    packageId = String(row.package_id) as PartnerPackageId;
    sessionId = Number(row.id);
    if (packageId !== "NEWBIE" && packageId !== "LIVE_DROP") {
      throw new Error("Loại mã không hợp lệ");
    }

    const partner = await getPartner(partnerAccount);
    if (!partner || !partner.isActive) {
      throw new Error("Đối tác không còn hoạt động");
    }

    const maxUses = Number(row.g_max_uses ?? 0);
    const usedCount = Number(row.used_count ?? 0);
    if (maxUses > 0 && usedCount >= maxUses) {
      throw new Error("Mã quà đối tác đã hết lượt sử dụng");
    }

    const giftcodeId = Number(row.giftcode_id);
    const already = await new sql.Request(tx)
      .input("gid", sql.Int, giftcodeId)
      .input("account", sql.VarChar(10), claimer)
      .query(`
        SELECT TOP 1 id FROM cms.giftcode_claims
        WHERE giftcode_id = @gid AND account = @account
      `);
    if (already.recordset[0]) throw new Error("Bạn đã nhận mã này rồi");

    if (packageId === "LIVE_DROP" && (await hasLiveDropToday(claimer))) {
      throw new Error("Mỗi tài khoản chỉ nhận Live Drop 1 lần mỗi ngày");
    }
    if (packageId === "NEWBIE" && (await hasRecentNewbie(partnerAccount, claimer))) {
      throw new Error("Bạn đã nhận quà Newbie từ đối tác này trong 7 ngày qua");
    }

    coins = packageCoinCost(packageId);
    await consumeBudget(partnerAccount, partner.tier, coins, tx);

    await new sql.Request(tx)
      .input("gid", sql.Int, giftcodeId)
      .input("account", sql.VarChar(10), claimer)
      .query(`
        INSERT INTO cms.giftcode_claims (giftcode_id, account, claimed_at)
        VALUES (@gid, @account, GETDATE())
      `);
    await new sql.Request(tx)
      .input("gid", sql.Int, giftcodeId)
      .query(`
        UPDATE cms.giftcodes SET used_count = used_count + 1 WHERE id = @gid
      `);

    await new sql.Request(tx)
      .input("partner", sql.VarChar(10), partnerAccount)
      .input("packageId", sql.VarChar(16), packageId)
      .input("sessionId", sql.Int, sessionId)
      .input("claimer", sql.VarChar(10), claimer)
      .input("character", sql.VarChar(10), character.Name)
      .input("wc", sql.Int, coins.wc)
      .input("wp", sql.Int, coins.wp)
      .input("wg", sql.Int, coins.wg)
      .query(`
        INSERT INTO cms.partner_claims (
          partner_account, package_id, session_id, claimer_account,
          claimer_character, wc, wp, wg, note, claimed_at
        )
        VALUES (
          @partner, @packageId, @sessionId, @claimer,
          @character, @wc, @wp, @wg, NULL, GETDATE()
        )
      `);

    await tx.commit();
  } catch (e) {
    await tx.rollback();
    throw e;
  }

  if (coins.wc || coins.wp || coins.wg) {
    await addCoins(claimer, coins);
  }
  await queuePackageItems(character.Name, packageId, `partner:${partnerAccount}`);

  return {
    code,
    packageId,
    partnerAccount,
    character: character.Name,
    reward: coins,
    items: PARTNER_PACKAGES[packageId].items.map((i) => ({
      label: i.label,
      quantity: i.quantity,
      durationSeconds: i.durationSeconds,
    })),
  };
}

export async function grantHighlight(input: {
  partnerAccount: string;
  targetAccount: string;
  characterName: string;
}) {
  const partner = await requireActivePartner(input.partnerAccount);
  const target = normalizeAccount(input.targetAccount);
  if (target === partner.account) {
    throw new Error("Không thể tự phát Highlight cho chính mình");
  }
  const targetAcc = await findAccount(target);
  if (!targetAcc) throw new Error("Tài khoản nhận không tồn tại");

  const character = await assertCharacterOwned(input.characterName, target);
  if (await hasHighlightToday(target)) {
    throw new Error("Tài khoản này đã nhận Highlight hôm nay");
  }

  const live = await getActiveLiveSession(partner.account);
  if (live && live.highlightCount >= HIGHLIGHT_PER_LIVE_SESSION) {
    throw new Error(
      `Mỗi phiên Live chỉ phát tối đa ${HIGHLIGHT_PER_LIVE_SESSION} Highlight`
    );
  }

  const coins = packageCoinCost("HIGHLIGHT");
  const used = await getOrCreateBudgetMonth(partner.account);
  assertCanAfford(partner.tier, used, { ...coins, highlight: 1 });

  await ensurePartnerSchema();
  const pool = await getPool();
  const tx = new sql.Transaction(pool);
  await tx.begin();
  try {
    await consumeBudget(partner.account, partner.tier, { ...coins, highlight: 1 }, tx);

    if (live) {
      await new sql.Request(tx)
        .input("id", sql.Int, live.id)
        .query(`
          UPDATE cms.partner_sessions
          SET highlight_count = highlight_count + 1
          WHERE id = @id
        `);
    }

    await new sql.Request(tx)
      .input("partner", sql.VarChar(10), partner.account)
      .input("packageId", sql.VarChar(16), "HIGHLIGHT")
      .input("sessionId", sql.Int, live?.id ?? null)
      .input("claimer", sql.VarChar(10), target)
      .input("character", sql.VarChar(10), character.Name)
      .input("wc", sql.Int, coins.wc)
      .input("wp", sql.Int, coins.wp)
      .input("wg", sql.Int, coins.wg)
      .query(`
        INSERT INTO cms.partner_claims (
          partner_account, package_id, session_id, claimer_account,
          claimer_character, wc, wp, wg, note, claimed_at
        )
        VALUES (
          @partner, @packageId, @sessionId, @claimer,
          @character, @wc, @wp, @wg, N'highlight-direct', GETDATE()
        )
      `);

    await tx.commit();
  } catch (e) {
    await tx.rollback();
    throw e;
  }

  await addCoins(target, coins);
  await queuePackageItems(character.Name, "HIGHLIGHT", `partner:${partner.account}`);

  return {
    packageId: "HIGHLIGHT" as const,
    partnerAccount: partner.account,
    targetAccount: target,
    character: character.Name,
    reward: coins,
    items: PARTNER_PACKAGES.HIGHLIGHT.items.map((i) => ({
      label: i.label,
      quantity: i.quantity,
      durationSeconds: i.durationSeconds,
    })),
    liveSessionId: live?.id ?? null,
  };
}
