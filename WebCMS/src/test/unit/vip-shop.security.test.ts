import { describe, expect, it } from "vitest";
import {
  clampVipLevel,
  getVipPackage,
  VIP_PACKAGES,
} from "@/lib/vip-shop";

describe("vip-shop security", () => {
  it("only allows known package ids", () => {
    expect(getVipPackage("vip7")?.days).toBe(7);
    expect(getVipPackage("vip15")?.days).toBe(15);
    expect(getVipPackage("vip30")?.days).toBe(30);
    expect(getVipPackage("vip7'; DROP TABLE--")).toBeUndefined();
    expect(getVipPackage("../vip7")).toBeUndefined();
    expect(getVipPackage("")).toBeUndefined();
    expect(getVipPackage("VIP7")).toBeUndefined();
  });

  it("clamps account level to 0 or 1", () => {
    expect(clampVipLevel(-1)).toBe(0);
    expect(clampVipLevel(0)).toBe(0);
    expect(clampVipLevel(1)).toBe(1);
    expect(clampVipLevel(2)).toBe(1);
    expect(clampVipLevel(999)).toBe(1);
    expect(clampVipLevel(Number.NaN)).toBe(0);
  });

  it("prices are positive finite numbers", () => {
    for (const p of VIP_PACKAGES) {
      expect(p.priceWc).toBeGreaterThan(0);
      expect(Number.isFinite(p.priceWc)).toBe(true);
    }
  });
});
