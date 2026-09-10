import { beforeEach, describe, expect, it, vi } from "vitest";

const registerAccount = vi.fn();
const verifyTurnstileToken = vi.fn();

vi.mock("@/lib/game", () => ({
  registerAccount: (...args: unknown[]) => registerAccount(...args),
}));

vi.mock("@/lib/security/turnstile", async () => {
  const actual = await vi.importActual<typeof import("@/lib/security/turnstile")>(
    "@/lib/security/turnstile"
  );
  return {
    ...actual,
    verifyTurnstileToken: (...args: unknown[]) => verifyTurnstileToken(...args),
    isTurnstileConfigured: () => true,
    allowDevCaptchaFallback: () => false,
    getTurnstileSecretKey: () => "secret",
    getTurnstileSiteKey: () => "site",
    isProductionRuntime: () => true,
  };
});

vi.mock("@/lib/security/env-guards", async () => {
  const actual = await vi.importActual<typeof import("@/lib/security/env-guards")>(
    "@/lib/security/env-guards"
  );
  return {
    ...actual,
    getRegisterBotMode: () => "turnstile" as const,
  };
});

vi.mock("next/headers", () => ({
  cookies: async () => ({
    get: () => undefined,
  }),
}));

describe("POST /api/register anti-bot", () => {
  beforeEach(() => {
    vi.resetModules();
    registerAccount.mockReset();
    verifyTurnstileToken.mockReset();
  });

  it("rejects missing turnstile token in turnstile mode", async () => {
    verifyTurnstileToken.mockResolvedValue({
      ok: false,
      reason: "Vui lòng xác minh bạn không phải robot (Turnstile)",
    });
    const { POST } = await import("@/app/api/register/route");
    const res = await POST(
      new Request("http://localhost/api/register", {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
          "x-forwarded-for": "9.9.9.9",
        },
        body: JSON.stringify({
          account: "botuser1",
          password: "pass12",
          confirmPassword: "pass12",
          turnstileToken: "",
        }),
      })
    );
    expect(res.status).toBe(400);
    expect(registerAccount).not.toHaveBeenCalled();
  });

  it("rate-limits register spam from same IP", async () => {
    verifyTurnstileToken.mockResolvedValue({ ok: true });
    registerAccount.mockResolvedValue({ account: "botuser1" });
    const { POST } = await import("@/app/api/register/route");
    const mk = (n: number) =>
      POST(
        new Request("http://localhost/api/register", {
          method: "POST",
          headers: {
            "Content-Type": "application/json",
            "x-forwarded-for": "8.8.8.8",
          },
          body: JSON.stringify({
            account: `botu${n}xx`,
            password: "pass12",
            confirmPassword: "pass12",
            turnstileToken: "tok",
          }),
        })
      );

    // 5 allowed
    for (let i = 0; i < 5; i++) {
      const res = await mk(i);
      // may be 200 or 400 depending on account schema length — botu0xx is 7 chars ok
      expect([200, 400, 409, 500]).toContain(res.status);
      expect(res.status).not.toBe(429);
    }
    const blocked = await mk(99);
    expect(blocked.status).toBe(429);
  });
});
