import { beforeEach, describe, expect, it, vi } from "vitest";

const requireUser = vi.fn();
const changePassword = vi.fn();

vi.mock("@/lib/auth", () => ({
  requireUser: (...args: unknown[]) => requireUser(...args),
  requireAdmin: vi.fn(),
  getServerAuthSession: vi.fn(),
  authOptions: {},
}));

vi.mock("@/lib/game", () => ({
  changePassword: (...args: unknown[]) => changePassword(...args),
}));

describe("POST /api/password security", () => {
  beforeEach(() => {
    vi.resetModules();
    requireUser.mockReset();
    changePassword.mockReset();
  });

  it("returns 401 when not logged in as user", async () => {
    requireUser.mockRejectedValue(new Error("UNAUTHORIZED_USER"));
    const { POST } = await import("@/app/api/password/route");
    const res = await POST(
      new Request("http://localhost/api/password", {
        method: "POST",
        body: JSON.stringify({
          oldPassword: "old12",
          newPassword: "new12",
          confirmPassword: "new12",
        }),
      })
    );
    expect(res.status).toBe(401);
    expect(changePassword).not.toHaveBeenCalled();
  });

  it("changes only session account, ignoring body.account", async () => {
    requireUser.mockResolvedValue({
      user: { id: "playerA", role: "user" },
    });
    changePassword.mockResolvedValue(undefined);
    const { POST } = await import("@/app/api/password/route");
    const res = await POST(
      new Request("http://localhost/api/password", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({
          account: "victim",
          oldPassword: "old12",
          newPassword: "new12",
          confirmPassword: "new12",
        }),
      })
    );
    expect(res.status).toBe(200);
    expect(changePassword).toHaveBeenCalledWith("playerA", "old12", "new12");
  });
});
