import { describe, expect, it } from "vitest";
import {
  encodeCaptchaCookie,
  verifyCaptchaCookie,
} from "@/lib/captcha";

describe("math captcha (dev fallback) security", () => {
  it("accepts valid signed cookie + answer", () => {
    const cookie = encodeCaptchaCookie("7", "abc123");
    expect(verifyCaptchaCookie(cookie, "7").ok).toBe(true);
  });

  it("rejects wrong answer", () => {
    const cookie = encodeCaptchaCookie("7", "abc123");
    expect(verifyCaptchaCookie(cookie, "8").ok).toBe(false);
  });

  it("rejects tampered HMAC", () => {
    const cookie = encodeCaptchaCookie("7", "abc123");
    const parts = cookie.split(".");
    parts[3] = "0".repeat(parts[3].length);
    expect(verifyCaptchaCookie(parts.join("."), "7").ok).toBe(false);
  });

  it("rejects missing cookie", () => {
    expect(verifyCaptchaCookie(undefined, "7").ok).toBe(false);
  });
});
