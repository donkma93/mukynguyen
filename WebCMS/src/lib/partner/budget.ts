import { getPool, sql } from "@/lib/db";
import {
  getTierBudget,
  remainingBudget,
  type PartnerTier,
  vietnamYearMonth,
} from "@/lib/partner/catalog";
import { ensurePartnerSchema } from "@/lib/partner/schema";

type SqlTransaction = InstanceType<typeof sql.Transaction>;

export type BudgetUsage = {
  partnerAccount: string;
  yearMonth: string;
  wcUsed: number;
  wpUsed: number;
  wgUsed: number;
  liveSessionsUsed: number;
  highlightUsed: number;
};

export type BudgetCost = {
  wc?: number;
  wp?: number;
  wg?: number;
  liveSessions?: number;
  highlight?: number;
};

function mapBudget(row: Record<string, unknown>): BudgetUsage {
  return {
    partnerAccount: String(row.partner_account).trim().toLowerCase(),
    yearMonth: String(row.year_month),
    wcUsed: Number(row.wc_used ?? 0),
    wpUsed: Number(row.wp_used ?? 0),
    wgUsed: Number(row.wg_used ?? 0),
    liveSessionsUsed: Number(row.live_sessions_used ?? 0),
    highlightUsed: Number(row.highlight_used ?? 0),
  };
}

async function ensureBudgetRow(
  partnerAccount: string,
  yearMonth: string,
  tx?: SqlTransaction
): Promise<BudgetUsage> {
  await ensurePartnerSchema();
  const account = partnerAccount.trim().toLowerCase();
  const req = tx ? new sql.Request(tx) : (await getPool()).request();
  await req
    .input("account", sql.VarChar(10), account)
    .input("ym", sql.Char(7), yearMonth)
    .query(`
      IF NOT EXISTS (
        SELECT 1 FROM cms.partner_budget_months
        WHERE partner_account = @account AND year_month = @ym
      )
      BEGIN
        INSERT INTO cms.partner_budget_months (
          partner_account, year_month, wc_used, wp_used, wg_used,
          live_sessions_used, highlight_used
        )
        VALUES (@account, @ym, 0, 0, 0, 0, 0)
      END
    `);

  const read = tx ? new sql.Request(tx) : (await getPool()).request();
  const r = await read
    .input("account", sql.VarChar(10), account)
    .input("ym", sql.Char(7), yearMonth)
    .query(`
      SELECT TOP 1 partner_account, year_month, wc_used, wp_used, wg_used,
             live_sessions_used, highlight_used
      FROM cms.partner_budget_months
      WHERE partner_account = @account AND year_month = @ym
    `);
  return mapBudget(r.recordset[0] as Record<string, unknown>);
}

export async function getOrCreateBudgetMonth(
  partnerAccount: string,
  yearMonth: string = vietnamYearMonth(),
  tx?: SqlTransaction
): Promise<BudgetUsage> {
  return ensureBudgetRow(partnerAccount, yearMonth, tx);
}

export async function getBudgetSnapshot(
  partnerAccount: string,
  tier: PartnerTier,
  yearMonth: string = vietnamYearMonth()
) {
  const used = await getOrCreateBudgetMonth(partnerAccount, yearMonth);
  const remaining = remainingBudget(tier, used);
  return { used, remaining, caps: getTierBudget(tier), yearMonth };
}

export function assertCanAfford(
  tier: PartnerTier,
  used: BudgetUsage,
  cost: BudgetCost
): void {
  const rem = remainingBudget(tier, used);
  const needWc = cost.wc ?? 0;
  const needWp = cost.wp ?? 0;
  const needWg = cost.wg ?? 0;
  const needLive = cost.liveSessions ?? 0;
  const needHl = cost.highlight ?? 0;

  if (needLive > rem.liveSessions) {
    throw new Error("Đã hết hạn mức phiên Live trong tháng");
  }
  if (needHl > rem.highlight) {
    throw new Error("Đã hết hạn mức Highlight trong tháng");
  }
  if (needWc > rem.wc || needWp > rem.wp || needWg > rem.wg) {
    throw new Error("Ngân sách WC/WP/WG tháng này không đủ");
  }
}

export async function consumeBudget(
  partnerAccount: string,
  tier: PartnerTier,
  cost: BudgetCost,
  tx: SqlTransaction,
  yearMonth: string = vietnamYearMonth()
): Promise<BudgetUsage> {
  const used = await ensureBudgetRow(partnerAccount, yearMonth, tx);
  assertCanAfford(tier, used, cost);

  const wc = cost.wc ?? 0;
  const wp = cost.wp ?? 0;
  const wg = cost.wg ?? 0;
  const live = cost.liveSessions ?? 0;
  const hl = cost.highlight ?? 0;

  const r = await new sql.Request(tx)
    .input("account", sql.VarChar(10), partnerAccount.trim().toLowerCase())
    .input("ym", sql.Char(7), yearMonth)
    .input("wc", sql.Int, wc)
    .input("wp", sql.Int, wp)
    .input("wg", sql.Int, wg)
    .input("live", sql.Int, live)
    .input("hl", sql.Int, hl)
    .query(`
      UPDATE cms.partner_budget_months
      SET wc_used = wc_used + @wc,
          wp_used = wp_used + @wp,
          wg_used = wg_used + @wg,
          live_sessions_used = live_sessions_used + @live,
          highlight_used = highlight_used + @hl
      OUTPUT INSERTED.partner_account, INSERTED.year_month, INSERTED.wc_used,
             INSERTED.wp_used, INSERTED.wg_used, INSERTED.live_sessions_used,
             INSERTED.highlight_used
      WHERE partner_account = @account AND year_month = @ym
    `);
  return mapBudget(r.recordset[0] as Record<string, unknown>);
}
