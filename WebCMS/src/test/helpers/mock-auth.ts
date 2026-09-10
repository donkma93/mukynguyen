import { vi } from "vitest";

export type MockSession = {
  user: { id: string; name?: string; role: "user" | "admin" };
} | null;

export function mockServerSession(session: MockSession) {
  vi.doMock("next-auth", async () => {
    const actual = await vi.importActual<typeof import("next-auth")>("next-auth");
    return {
      ...actual,
      getServerSession: vi.fn(async () => session),
    };
  });
}

/** Simpler: mock the auth module helpers used by routes. */
export function mockAuthModule(session: MockSession) {
  vi.mock("@/lib/auth", () => ({
    getServerAuthSession: vi.fn(async () => session),
    requireUser: vi.fn(async () => {
      if (!session?.user?.id || session.user.role !== "user") {
        throw new Error("UNAUTHORIZED_USER");
      }
      return session;
    }),
    requireAdmin: vi.fn(async () => {
      if (!session?.user?.id || session.user.role !== "admin") {
        throw new Error("UNAUTHORIZED_ADMIN");
      }
      return session;
    }),
    authOptions: {},
  }));
}
