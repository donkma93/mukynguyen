import { getServerSession, type NextAuthOptions } from "next-auth";
import CredentialsProvider from "next-auth/providers/credentials";
import { verifyAdminPassword } from "@/lib/cms";
import { verifyGameLogin } from "@/lib/game";
import {
  clearRateLimitKey,
  isRateLimited,
  RATE_LIMITS,
  recordAuthFailure,
} from "@/lib/security/rate-limit";

function loginLockKey(kind: string, identity: string) {
  return `login:${kind}:${identity.trim().toLowerCase()}`;
}

export const authOptions: NextAuthOptions = {
  session: { strategy: "jwt" },
  pages: {
    signIn: "/login",
  },
  providers: [
    CredentialsProvider({
      id: "credentials",
      name: "Credentials",
      credentials: {
        kind: { label: "Kind", type: "text" },
        account: { label: "Account", type: "text" },
        username: { label: "Username", type: "text" },
        password: { label: "Password", type: "password" },
      },
      async authorize(credentials) {
        const kind = (credentials?.kind || "user").toLowerCase();
        const password = credentials?.password || "";
        if (!password) return null;

        if (kind === "admin") {
          const username = (
            credentials?.username ||
            credentials?.account ||
            ""
          ).trim();
          if (!username) return null;

          const lockKey = loginLockKey("admin", username);
          const gate = isRateLimited(lockKey, RATE_LIMITS.loginFail);
          if (!gate.ok) throw new Error(gate.reason);

          const result = await verifyAdminPassword(username, password);
          if (!result.ok) {
            const fail = recordAuthFailure(lockKey, RATE_LIMITS.loginFail);
            if (!fail.ok) throw new Error(fail.reason);
            throw new Error(result.reason);
          }
          clearRateLimitKey(lockKey);
          return {
            id: result.username,
            name: result.displayName,
            role: "admin" as const,
          };
        }

        const account = (
          credentials?.account ||
          credentials?.username ||
          ""
        ).trim();
        if (!account) return null;

        const lockKey = loginLockKey("user", account);
        const gate = isRateLimited(lockKey, RATE_LIMITS.loginFail);
        if (!gate.ok) throw new Error(gate.reason);

        const result = await verifyGameLogin(account, password);
        if (!result.ok) {
          const fail = recordAuthFailure(lockKey, RATE_LIMITS.loginFail);
          if (!fail.ok) throw new Error(fail.reason);
          throw new Error(result.reason);
        }
        clearRateLimitKey(lockKey);
        return {
          id: result.account,
          name: result.account,
          role: "user" as const,
        };
      },
    }),
  ],
  callbacks: {
    async jwt({ token, user }) {
      if (user) {
        token.id = user.id;
        token.role = (user as { role?: "user" | "admin" }).role || "user";
        token.name = user.name;
      }
      return token;
    },
    async session({ session, token }) {
      if (session.user) {
        session.user.id = String(token.id || token.sub || "");
        session.user.role = (token.role as "user" | "admin") || "user";
        if (token.name) session.user.name = String(token.name);
      }
      return session;
    },
  },
  secret: process.env.NEXTAUTH_SECRET,
};

export function getServerAuthSession() {
  return getServerSession(authOptions);
}

export async function requireUser() {
  const session = await getServerAuthSession();
  if (!session?.user?.id || session.user.role !== "user") {
    throw new Error("UNAUTHORIZED_USER");
  }
  return session;
}

export async function requireAdmin() {
  const session = await getServerAuthSession();
  if (!session?.user?.id || session.user.role !== "admin") {
    throw new Error("UNAUTHORIZED_ADMIN");
  }
  return session;
}
