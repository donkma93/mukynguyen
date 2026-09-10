import { describe, expect, it } from "vitest";
import {
  clearRateLimitKey,
  consumeRateLimit,
  isRateLimited,
  RATE_LIMITS,
  recordAuthFailure,
  resetRateLimitStore,
} from "@/lib/security/rate-limit";

describe("rate-limit security", () => {
  it("allows up to limit then blocks register spam", () => {
    const key = "register:1.2.3.4";
    for (let i = 0; i < RATE_LIMITS.register.limit; i++) {
      expect(consumeRateLimit(key, RATE_LIMITS.register).ok).toBe(true);
    }
    const blocked = consumeRateLimit(key, RATE_LIMITS.register);
    expect(blocked.ok).toBe(false);
    if (!blocked.ok) {
      expect(blocked.retryAfterSec).toBeGreaterThan(0);
    }
  });

  it("login lockout peeks without consuming, then records failures", () => {
    const key = "login:user:victim";
    expect(isRateLimited(key, RATE_LIMITS.loginFail).ok).toBe(true);

    for (let i = 0; i < RATE_LIMITS.loginFail.limit; i++) {
      expect(recordAuthFailure(key, RATE_LIMITS.loginFail).ok).toBe(true);
    }
    const locked = isRateLimited(key, RATE_LIMITS.loginFail);
    expect(locked.ok).toBe(false);

    clearRateLimitKey(key);
    expect(isRateLimited(key, RATE_LIMITS.loginFail).ok).toBe(true);
  });

  it("resetRateLimitStore clears all keys", () => {
    consumeRateLimit("a", { limit: 1, windowMs: 60_000 });
    resetRateLimitStore();
    expect(consumeRateLimit("a", { limit: 1, windowMs: 60_000 }).ok).toBe(true);
  });
});
