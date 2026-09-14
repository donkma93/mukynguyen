import { describe, expect, it } from "vitest";

const enabled = process.env.RUN_INTEGRATION === "1";
const base = (process.env.BASE_URL || "http://localhost:5000").replace(/\/$/, "");

describe.skipIf(!enabled)("HTTP integration: admin forbidden without auth", () => {
  const paths = [
    "/api/admin/accounts",
    "/api/admin/coins",
    "/api/admin/giftcodes",
    "/api/admin/ops/status",
    "/api/admin/ops/restart",
    "/api/admin/gs/ini",
    "/api/admin/hack",
    "/api/admin/characters",
    "/api/admin/items",
  ];

  for (const path of paths) {
    it(`GET/POST ${path} → 401 without session`, async () => {
      const getRes = await fetch(`${base}${path}`, { method: "GET" });
      // Some routes only POST — accept 401/405/404 but never 200 with data
      if (getRes.status === 200) {
        const data = await getRes.json().catch(() => ({}));
        expect(data.ok).not.toBe(true);
      } else {
        expect([401, 405, 404]).toContain(getRes.status);
      }

      const postRes = await fetch(`${base}${path}`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({}),
      });
      expect([401, 400, 405, 404]).toContain(postRes.status);
      if (postRes.status === 200) {
        throw new Error(`${path} POST succeeded without auth`);
      }
    });
  }

  it("VIP purchase without auth → 401", async () => {
    const res = await fetch(`${base}/api/vip/purchase`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ packageId: "vip7" }),
    });
    expect(res.status).toBe(401);
  });
});
