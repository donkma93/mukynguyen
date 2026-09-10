import { beforeEach, describe, expect, it, vi } from "vitest";

const requireAdmin = vi.fn();
const requireUser = vi.fn();

vi.mock("@/lib/auth", () => ({
  requireAdmin: (...args: unknown[]) => requireAdmin(...args),
  requireUser: (...args: unknown[]) => requireUser(...args),
  getServerAuthSession: vi.fn(),
  authOptions: {},
}));

/** Minimal stubs so handlers can import heavy libs without DB. */
vi.mock("@/lib/game", () => ({
  searchAccounts: vi.fn(),
  setAccountBlock: vi.fn(),
  setVip: vi.fn(),
  addCoins: vi.fn(),
  getOnlineCount: vi.fn(),
  findAccount: vi.fn(),
  getCash: vi.fn(),
}));

vi.mock("@/lib/cms", () => ({
  listGiftcodes: vi.fn(async () => []),
  createGiftcode: vi.fn(),
  updateGiftcode: vi.fn(),
  deleteGiftcode: vi.fn(),
}));

vi.mock("@/lib/gs/ops", () => ({
  restartServers: vi.fn(),
  getOpsStatus: vi.fn(async () => ({})),
}));

vi.mock("@/lib/gs/audit", () => ({
  writeAudit: vi.fn(),
}));

vi.mock("@/lib/gs/files", () => ({
  readTextFile: vi.fn(async () => ""),
  writeTextFile: vi.fn(),
}));

vi.mock("@/lib/gs/ini", () => ({
  applyIniUpdates: vi.fn(),
  listIniEntries: vi.fn(() => []),
  parseIni: vi.fn(),
}));

vi.mock("@/lib/gs/hack-logs", () => ({
  searchHackLogs: vi.fn(async () => []),
}));

vi.mock("@/lib/gs/item-catalog", () => ({
  searchItems: vi.fn(async () => []),
}));

vi.mock("@/lib/gs/groups", () => ({
  readGroupRates: vi.fn(async () => ({})),
  writeGroupRates: vi.fn(),
}));

vi.mock("@/lib/gs/inventory", () => ({
  // inventory route imports from game mostly
}));

describe("admin API auth guards", () => {
  beforeEach(() => {
    vi.resetModules();
    requireAdmin.mockReset();
    requireUser.mockReset();
  });

  async function expectUnauthorized(
    importPath: string,
    method: "GET" | "POST" | "PUT" | "DELETE",
    url: string,
    body?: unknown
  ) {
    requireAdmin.mockRejectedValue(new Error("UNAUTHORIZED_ADMIN"));
    const mod = await import(importPath);
    const handler = mod[method];
    expect(handler).toBeTypeOf("function");
    const init: RequestInit = { method };
    if (body !== undefined) {
      init.headers = { "Content-Type": "application/json" };
      init.body = JSON.stringify(body);
    }
    const res = await handler(new Request(url, init), {
      params: Promise.resolve({ slug: "common" }),
    });
    expect(res.status).toBe(401);
  }

  it("blocks anonymous/user from admin accounts GET/POST", async () => {
    await expectUnauthorized(
      "@/app/api/admin/accounts/route",
      "GET",
      "http://localhost/api/admin/accounts"
    );
    await expectUnauthorized(
      "@/app/api/admin/accounts/route",
      "POST",
      "http://localhost/api/admin/accounts",
      { account: "x", action: "coins", wc: 1 }
    );
  });

  it("blocks unauth from coins, giftcodes, ops restart, ini, hack", async () => {
    await expectUnauthorized(
      "@/app/api/admin/coins/route",
      "POST",
      "http://localhost/api/admin/coins",
      { account: "x", wc: 1 }
    );
    await expectUnauthorized(
      "@/app/api/admin/giftcodes/route",
      "GET",
      "http://localhost/api/admin/giftcodes"
    );
    await expectUnauthorized(
      "@/app/api/admin/ops/restart/route",
      "POST",
      "http://localhost/api/admin/ops/restart",
      { scope: "gameserver" }
    );
    await expectUnauthorized(
      "@/app/api/admin/gs/ini/[slug]/route",
      "GET",
      "http://localhost/api/admin/gs/ini/common"
    );
    await expectUnauthorized(
      "@/app/api/admin/hack/route",
      "GET",
      "http://localhost/api/admin/hack"
    );
  });

  it("rejects dangerous ops scope even for admin", async () => {
    requireAdmin.mockResolvedValue({
      user: { id: "admin", role: "admin", name: "admin" },
    });
    const { POST } = await import("@/app/api/admin/ops/restart/route");
    const res = await POST(
      new Request("http://localhost/api/admin/ops/restart", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ scope: "all; rm -rf /" }),
      })
    );
    expect(res.status).toBe(400);
  });

  it("invalid INI slug returns 404 for admin (no path escape)", async () => {
    requireAdmin.mockResolvedValue({
      user: { id: "admin", role: "admin", name: "admin" },
    });
    const { GET } = await import("@/app/api/admin/gs/ini/[slug]/route");
    const res = await GET(
      new Request("http://localhost/api/admin/gs/ini/../../etc/passwd"),
      { params: Promise.resolve({ slug: "../../etc/passwd" }) }
    );
    expect(res.status).toBe(404);
  });
});
