import { afterEach, describe, expect, it, vi } from "vitest";
import {
  allowDevCaptchaFallback,
  isTurnstileConfigured,
  verifyTurnstileToken,
} from "@/lib/security/turnstile";

describe("turnstile security", () => {
  afterEach(() => {
    vi.unstubAllEnvs();
    vi.unstubAllGlobals();
    vi.restoreAllMocks();
  });

  it("production without keys is not configured; dev captcha blocked in prod", () => {
    vi.stubEnv("NODE_ENV", "production");
    vi.stubEnv("TURNSTILE_SITE_KEY", "");
    vi.stubEnv("TURNSTILE_SECRET_KEY", "");
    vi.stubEnv("ALLOW_DEV_CAPTCHA", "1");
    expect(isTurnstileConfigured()).toBe(false);
    expect(allowDevCaptchaFallback()).toBe(false);
  });

  it("rejects empty token when secret present", async () => {
    vi.stubEnv("TURNSTILE_SECRET_KEY", "test-secret");
    const r = await verifyTurnstileToken("");
    expect(r.ok).toBe(false);
  });

  it("accepts token when Cloudflare siteverify returns success", async () => {
    vi.stubEnv("TURNSTILE_SECRET_KEY", "test-secret");
    vi.stubGlobal(
      "fetch",
      vi.fn(async () => ({
        json: async () => ({ success: true }),
      }))
    );
    const r = await verifyTurnstileToken("tok_ok");
    expect(r.ok).toBe(true);
  });

  it("rejects token when siteverify fails", async () => {
    vi.stubEnv("TURNSTILE_SECRET_KEY", "test-secret");
    vi.stubGlobal(
      "fetch",
      vi.fn(async () => ({
        json: async () => ({ success: false, "error-codes": ["invalid"] }),
      }))
    );
    const r = await verifyTurnstileToken("bad");
    expect(r.ok).toBe(false);
  });
});
