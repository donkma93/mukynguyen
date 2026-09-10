import { describe, expect, it } from "vitest";
import {
  accountSchema,
  giftcodeSchema,
  passwordSchema,
  registerSchema,
} from "@/lib/validators";

describe("validators security", () => {
  it("rejects SQL-ish and path-like account names", () => {
    for (const bad of [
      "' OR 1=1--",
      "../admin",
      "admin drop",
      "abc",
      "abcdefghijk",
      "user name",
      "user@x",
    ]) {
      expect(accountSchema.safeParse(bad).success).toBe(false);
    }
  });

  it("accepts normal alnum accounts 4-10", () => {
    expect(accountSchema.safeParse("test").success).toBe(true);
    expect(accountSchema.safeParse("Player01").success).toBe(true);
  });

  it("rejects empty / oversized passwords", () => {
    expect(passwordSchema.safeParse("").success).toBe(false);
    expect(passwordSchema.safeParse("abc").success).toBe(false);
    expect(passwordSchema.safeParse("abcdefghijk").success).toBe(false);
    expect(passwordSchema.safeParse("pass12").success).toBe(true);
  });

  it("rejects empty / oversized giftcodes", () => {
    expect(giftcodeSchema.safeParse({ code: "" }).success).toBe(false);
    expect(giftcodeSchema.safeParse({ code: "ab" }).success).toBe(false);
    expect(
      giftcodeSchema.safeParse({ code: "x".repeat(33) }).success
    ).toBe(false);
    expect(giftcodeSchema.safeParse({ code: "WELCOME" }).success).toBe(true);
  });

  it("register schema requires matching passwords", () => {
    const bad = registerSchema.safeParse({
      account: "player01",
      password: "pass12",
      confirmPassword: "pass99",
      captcha: "1",
    });
    expect(bad.success).toBe(false);
  });
});
