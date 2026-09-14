import { beforeEach, describe, expect, it, vi } from "vitest";

const requireUser = vi.fn();
const requireAdmin = vi.fn();

vi.mock("@/lib/auth", () => ({
  requireAdmin: (...args: unknown[]) => requireAdmin(...args),
  requireUser: (...args: unknown[]) => requireUser(...args),
  getServerAuthSession: vi.fn(),
  authOptions: {},
}));

vi.mock("@/lib/partner/partners", () => ({
  isActivePartner: vi.fn(async () => false),
  getPartner: vi.fn(async () => null),
  getPartnerDashboard: vi.fn(),
  listPartners: vi.fn(async () => []),
  upsertPartner: vi.fn(),
}));

vi.mock("@/lib/partner/gifts", () => ({
  createLiveSession: vi.fn(),
  createNewbieSession: vi.fn(),
  grantHighlight: vi.fn(),
  claimPartnerCode: vi.fn(),
  listPartnerClaims: vi.fn(async () => []),
  listPartnerSessions: vi.fn(async () => []),
  getActiveLiveSession: vi.fn(async () => null),
}));

vi.mock("@/lib/partner/budget", () => ({
  getBudgetSnapshot: vi.fn(),
}));

vi.mock("@/lib/partner/catalog", async () => {
  const actual = await vi.importActual<typeof import("@/lib/partner/catalog")>(
    "@/lib/partner/catalog"
  );
  return actual;
});

vi.mock("@/lib/game", () => ({
  findAccount: vi.fn(),
  getCharacters: vi.fn(async () => []),
}));

describe("partner API auth guards", () => {
  beforeEach(() => {
    vi.resetModules();
    requireUser.mockReset();
    requireAdmin.mockReset();
  });

  it("rejects unauthenticated partner me", async () => {
    requireUser.mockRejectedValue(new Error("UNAUTHORIZED_USER"));
    const { GET } = await import("@/app/api/partner/me/route");
    const res = await GET();
    expect(res.status).toBe(401);
  });

  it("rejects unauthenticated partner claim", async () => {
    requireUser.mockRejectedValue(new Error("UNAUTHORIZED_USER"));
    const { POST } = await import("@/app/api/partner/claim/route");
    const res = await POST(
      new Request("http://localhost/api/partner/claim", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ code: "LIVE-TEST123", characterName: "Hero" }),
      })
    );
    expect(res.status).toBe(401);
  });

  it("rejects unauthenticated admin partners", async () => {
    requireAdmin.mockRejectedValue(new Error("UNAUTHORIZED_ADMIN"));
    const { GET } = await import("@/app/api/admin/partners/route");
    const res = await GET();
    expect(res.status).toBe(401);
  });
});
