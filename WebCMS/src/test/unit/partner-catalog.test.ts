import { describe, expect, it } from "vitest";
import {
  getTierBudget,
  isPartnerPackageId,
  isPartnerTier,
  packageCoinCost,
  PARTNER_ITEMS,
  PARTNER_PACKAGES,
  remainingBudget,
  vietnamDayKey,
  vietnamDayStartUtc,
  vietnamYearMonth,
} from "@/lib/partner/catalog";

describe("partner catalog", () => {
  it("locks VIP-safe coin costs", () => {
    expect(packageCoinCost("NEWBIE")).toEqual({ wc: 0, wp: 100, wg: 0 });
    expect(packageCoinCost("LIVE_DROP")).toEqual({ wc: 0, wp: 80, wg: 30 });
    expect(packageCoinCost("HIGHLIGHT")).toEqual({ wc: 100, wp: 200, wg: 0 });
  });

  it("uses known item indexes", () => {
    expect(PARTNER_ITEMS.bless.itemIndex).toBe(7181);
    expect(PARTNER_ITEMS.soul.itemIndex).toBe(7182);
    expect(PARTNER_ITEMS.chaos.itemIndex).toBe(6159);
    expect(PARTNER_ITEMS.life.itemIndex).toBe(7184);
    expect(PARTNER_ITEMS.expBuff.itemIndex).toBe(6699);
    expect(PARTNER_PACKAGES.LIVE_DROP.items.some((i) => i.durationSeconds === 1800)).toBe(
      true
    );
  });

  it("defines monthly tier budgets", () => {
    expect(getTierBudget("new").liveSessions).toBe(12);
    expect(getTierBudget("stable").wp).toBe(15000);
    expect(getTierBudget("top").highlight).toBe(60);
  });

  it("computes remaining budget", () => {
    const rem = remainingBudget("new", {
      wcUsed: 200,
      wpUsed: 1000,
      wgUsed: 500,
      liveSessionsUsed: 3,
      highlightUsed: 5,
    });
    expect(rem.wc).toBe(800);
    expect(rem.wp).toBe(7000);
    expect(rem.wg).toBe(1500);
    expect(rem.liveSessions).toBe(9);
    expect(rem.highlight).toBe(15);
  });

  it("validates tier and package ids", () => {
    expect(isPartnerTier("new")).toBe(true);
    expect(isPartnerTier("vip")).toBe(false);
    expect(isPartnerPackageId("LIVE_DROP")).toBe(true);
    expect(isPartnerPackageId("VIP")).toBe(false);
  });

  it("formats Vietnam day / month keys", () => {
    const sample = new Date("2026-03-15T10:00:00+07:00");
    expect(vietnamDayKey(sample)).toBe("2026-03-15");
    expect(vietnamYearMonth(sample)).toBe("2026-03");
    expect(vietnamDayStartUtc(sample).toISOString()).toBe(
      new Date("2026-03-15T00:00:00+07:00").toISOString()
    );
  });
});
