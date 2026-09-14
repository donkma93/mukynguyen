import { describe, expect, it } from "vitest";
import {
  partnerClaimSchema,
  partnerHighlightSchema,
  partnerUpsertSchema,
} from "@/lib/validators";

describe("partner validators", () => {
  it("accepts claim payload", () => {
    expect(
      partnerClaimSchema.safeParse({
        code: "LIVE-ABCD1234",
        characterName: "HeroOne",
      }).success
    ).toBe(true);
  });

  it("rejects short claim code", () => {
    expect(
      partnerClaimSchema.safeParse({ code: "AB", characterName: "Hero" }).success
    ).toBe(false);
  });

  it("accepts highlight payload", () => {
    expect(
      partnerHighlightSchema.safeParse({
        targetAccount: "player01",
        characterName: "DK01",
      }).success
    ).toBe(true);
  });

  it("accepts partner upsert tiers", () => {
    expect(
      partnerUpsertSchema.safeParse({
        account: "stream01",
        tier: "stable",
        isActive: true,
      }).success
    ).toBe(true);
    expect(
      partnerUpsertSchema.safeParse({
        account: "stream01",
        tier: "legend",
      }).success
    ).toBe(false);
  });
});
