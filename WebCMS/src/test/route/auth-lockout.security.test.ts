import { describe, expect, it, vi } from "vitest";
import {
  clearRateLimitKey,
  isRateLimited,
  RATE_LIMITS,
  recordAuthFailure,
} from "@/lib/security/rate-limit";

/**
 * Mirrors the lockout logic in auth.ts authorize():
 * peek → on fail record → on success clear.
 */
async function simulateLoginAttempts(
  identity: string,
  passwords: string[],
  correctPassword: string
) {
  const lockKey = `login:user:${identity.toLowerCase()}`;
  clearRateLimitKey(lockKey);
  const outcomes: Array<"ok" | "bad" | "locked"> = [];

  for (const pwd of passwords) {
    const gate = isRateLimited(lockKey, RATE_LIMITS.loginFail);
    if (!gate.ok) {
      outcomes.push("locked");
      continue;
    }
    if (pwd === correctPassword) {
      clearRateLimitKey(lockKey);
      outcomes.push("ok");
    } else {
      const fail = recordAuthFailure(lockKey, RATE_LIMITS.loginFail);
      outcomes.push(fail.ok ? "bad" : "locked");
    }
  }
  return outcomes;
}

describe("login lockout policy", () => {
  it("locks after too many wrong passwords even if later password is correct", async () => {
    const wrong = Array.from({ length: RATE_LIMITS.loginFail.limit }, () => "wrong1");
    const outcomes = await simulateLoginAttempts("playerx", [...wrong, "correct"], "correct");
    expect(outcomes.slice(0, RATE_LIMITS.loginFail.limit).every((o) => o === "bad")).toBe(
      true
    );
    expect(outcomes[RATE_LIMITS.loginFail.limit]).toBe("locked");
  });

  it("clears lockout counter after a successful login mid-way", async () => {
    const attempts = ["wrong1", "wrong1", "correct", "wrong1"];
    const outcomes = await simulateLoginAttempts("playerY", attempts, "correct");
    expect(outcomes).toEqual(["bad", "bad", "ok", "bad"]);
    expect(isRateLimited("login:user:playery", RATE_LIMITS.loginFail).ok).toBe(true);
  });
});


