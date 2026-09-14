import { getPool, sql } from "@/lib/db";
import {
  isPartnerTier,
  type PartnerTier,
  vietnamYearMonth,
} from "@/lib/partner/catalog";
import { ensurePartnerSchema } from "@/lib/partner/schema";
import { getBudgetSnapshot } from "@/lib/partner/budget";

export type PartnerRow = {
  account: string;
  tier: PartnerTier;
  isActive: boolean;
  createdBy: string | null;
  createdAt: Date;
  updatedAt: Date;
};

function asBool(v: unknown): boolean {
  return v === true || v === 1 || v === "1";
}

function mapPartner(row: Record<string, unknown>): PartnerRow {
  const tierRaw = String(row.tier || "new");
  return {
    account: String(row.account).trim().toLowerCase(),
    tier: isPartnerTier(tierRaw) ? tierRaw : "new",
    isActive: asBool(row.is_active),
    createdBy: (row.created_by as string | null) ?? null,
    createdAt: row.created_at as Date,
    updatedAt: row.updated_at as Date,
  };
}

export async function getPartner(account: string): Promise<PartnerRow | null> {
  await ensurePartnerSchema();
  const id = account.trim().toLowerCase();
  const pool = await getPool();
  const r = await pool
    .request()
    .input("account", sql.VarChar(10), id)
    .query(`
      SELECT TOP 1 account, tier, is_active, created_by, created_at, updated_at
      FROM cms.partners
      WHERE account = @account
    `);
  const row = r.recordset[0] as Record<string, unknown> | undefined;
  return row ? mapPartner(row) : null;
}

export async function isActivePartner(account: string): Promise<boolean> {
  const partner = await getPartner(account);
  return Boolean(partner?.isActive);
}

export async function listPartners(limit = 200): Promise<PartnerRow[]> {
  await ensurePartnerSchema();
  const pool = await getPool();
  const r = await pool
    .request()
    .input("limit", sql.Int, Math.max(1, Math.min(500, Math.trunc(limit))))
    .query(`
      SELECT TOP (@limit) account, tier, is_active, created_by, created_at, updated_at
      FROM cms.partners
      ORDER BY updated_at DESC, account
    `);
  return r.recordset.map((row) => mapPartner(row as Record<string, unknown>));
}

export async function upsertPartner(input: {
  account: string;
  tier: PartnerTier;
  isActive?: boolean;
  createdBy?: string;
}): Promise<PartnerRow> {
  await ensurePartnerSchema();
  const account = input.account.trim().toLowerCase();
  if (!account || account.length > 10) {
    throw new Error("Tài khoản partner không hợp lệ");
  }
  if (!isPartnerTier(input.tier)) {
    throw new Error("Hạng partner không hợp lệ");
  }

  const pool = await getPool();
  const r = await pool
    .request()
    .input("account", sql.VarChar(10), account)
    .input("tier", sql.VarChar(16), input.tier)
    .input("active", sql.Bit, input.isActive === false ? 0 : 1)
    .input("createdBy", sql.NVarChar(100), input.createdBy?.trim().slice(0, 100) || null)
    .query(`
      MERGE cms.partners AS t
      USING (SELECT @account AS account) AS s
      ON t.account = s.account
      WHEN MATCHED THEN
        UPDATE SET
          tier = @tier,
          is_active = @active,
          updated_at = GETDATE()
      WHEN NOT MATCHED THEN
        INSERT (account, tier, is_active, created_by, created_at, updated_at)
        VALUES (@account, @tier, @active, @createdBy, GETDATE(), GETDATE())
      OUTPUT INSERTED.account, INSERTED.tier, INSERTED.is_active,
             INSERTED.created_by, INSERTED.created_at, INSERTED.updated_at;
    `);
  return mapPartner(r.recordset[0] as Record<string, unknown>);
}

export async function getPartnerDashboard(account: string) {
  const partner = await getPartner(account);
  if (!partner || !partner.isActive) {
    throw new Error("Tài khoản không phải đối tác đang hoạt động");
  }
  const yearMonth = vietnamYearMonth();
  const budget = await getBudgetSnapshot(partner.account, partner.tier, yearMonth);
  return { partner, yearMonth, budget };
}
