import { beforeEach, describe, expect, it, vi } from "vitest";

const purchaseVipWithWcoin = vi.fn();
const requireUser = vi.fn();

vi.mock("@/lib/auth", () => ({
  requireUser: (...args: unknown[]) => requireUser(...args),
  requireAdmin: vi.fn(),
  getServerAuthSession: vi.fn(),
  authOptions: {},
}));

vi.mock("@/lib/game", () => ({
  purchaseVipWithWcoin: (...args: unknown[]) => purchaseVipWithWcoin(...args),
}));

describe("POST /api/vip/purchase security", () => {
  beforeEach(() => {
    vi.resetModules();
    requireUser.mockReset();
    purchaseVipWithWcoin.mockReset();
  });

  it("returns 401 when anonymous", async () => {
    requireUser.mockRejectedValue(new Error("UNAUTHORIZED_USER"));
    const { POST } = await import("@/app/api/vip/purchase/route");
    const res = await POST(
      new Request("http://localhost/api/vip/purchase", {
        method: "POST",
        body: JSON.stringify({ packageId: "vip7" }),
      })
    );
    expect(res.status).toBe(401);
    expect(purchaseVipWithWcoin).not.toHaveBeenCalled();
  });

  it("returns 401 when admin session hits user API", async () => {
    requireUser.mockRejectedValue(new Error("UNAUTHORIZED_USER"));
    const { POST } = await import("@/app/api/vip/purchase/route");
    const res = await POST(
      new Request("http://localhost/api/vip/purchase", {
        method: "POST",
        body: JSON.stringify({ packageId: "vip7" }),
      })
    );
    expect(res.status).toBe(401);
  });

  it("ignores spoofed body.account and charges session user only", async () => {
    requireUser.mockResolvedValue({
      user: { id: "playerA", role: "user", name: "playerA" },
    });
    purchaseVipWithWcoin.mockResolvedValue({
      ok: true,
      packageId: "vip7",
      days: 7,
      priceWc: 5000,
      wcLeft: 100,
      accountLevel: 1,
      expireDate: new Date(),
    });
    const { POST } = await import("@/app/api/vip/purchase/route");
    const res = await POST(
      new Request("http://localhost/api/vip/purchase", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({
          packageId: "vip7",
          account: "victim",
          id: "victim",
        }),
      })
    );
    expect(res.status).toBe(200);
    expect(purchaseVipWithWcoin).toHaveBeenCalledTimes(1);
    expect(purchaseVipWithWcoin).toHaveBeenCalledWith("playerA", "vip7");
  });

  it("rejects unknown package without calling purchase", async () => {
    requireUser.mockResolvedValue({
      user: { id: "playerA", role: "user" },
    });
    const { POST } = await import("@/app/api/vip/purchase/route");
    const res = await POST(
      new Request("http://localhost/api/vip/purchase", {
        method: "POST",
        body: JSON.stringify({ packageId: "vip999" }),
      })
    );
    expect(res.status).toBe(400);
    expect(purchaseVipWithWcoin).not.toHaveBeenCalled();
  });
});
