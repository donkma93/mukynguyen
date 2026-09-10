import { describe, expect, it } from "vitest";

/**
 * Requires:
 *   RUN_INTEGRATION=1
 *   BASE_URL=http://localhost:3000
 *   TEST_USER / TEST_USER_PASS with known WC (optional for full buy test)
 *
 * Without credentials, only asserts unauthenticated denial.
 */
const enabled = process.env.RUN_INTEGRATION === "1";
const base = (process.env.BASE_URL || "http://localhost:3000").replace(/\/$/, "");

describe.skipIf(!enabled)("HTTP integration: VIP purchase", () => {
  it("unauthenticated purchase denied", async () => {
    const res = await fetch(`${base}/api/vip/purchase`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ packageId: "vip7", account: "someone" }),
    });
    expect(res.status).toBe(401);
  });
});
