"use client";

import Link from "next/link";
import { useRouter, useSearchParams } from "next/navigation";
import { signIn } from "next-auth/react";
import { FormEvent, Suspense, useMemo, useState } from "react";
import FlashMessage from "@/components/FlashMessage";
import { useI18n } from "@/components/I18nProvider";

function LoginForm() {
  const { t } = useI18n();
  const router = useRouter();
  const searchParams = useSearchParams();
  const isAdmin = searchParams.get("admin") === "1";
  const kind = useMemo(() => (isAdmin ? "admin" : "user"), [isAdmin]);

  const [account, setAccount] = useState("");
  const [password, setPassword] = useState("");
  const [error, setError] = useState<string | null>(null);
  const [loading, setLoading] = useState(false);

  async function resolvePublicIp(): Promise<string | undefined> {
    try {
      const response = await fetch(
        process.env.NEXT_PUBLIC_IP_LOOKUP_URL || "https://api64.ipify.org?format=json",
        {
          cache: "no-store",
          signal: AbortSignal.timeout(2500),
        }
      );
      if (!response.ok) return undefined;
      const data = (await response.json()) as { ip?: unknown };
      return typeof data.ip === "string" ? data.ip : undefined;
    } catch {
      // A blocked lookup must never prevent the player from signing in.
      return undefined;
    }
  }

  async function onSubmit(e: FormEvent) {
    e.preventDefault();
    setLoading(true);
    setError(null);
    try {
      const publicIp = await resolvePublicIp();
      const res = await signIn("credentials", {
        account,
        username: account,
        password,
        kind,
        publicIp,
        redirect: false,
      });
      if (!res || res.error) {
        setError(
          res?.error === "CredentialsSignin"
            ? t.login.badCredentials
            : res?.error || t.login.failed
        );
        return;
      }
      router.push(kind === "admin" ? "/admin" : "/panel");
      router.refresh();
    } catch {
      setError(t.login.networkError);
    } finally {
      setLoading(false);
    }
  }

  return (
    <div className="mx-auto flex min-h-[70vh] max-w-md items-center px-4 py-10">
      <div className="card-glow w-full p-6 md:p-8">
        <h1 className="font-display text-2xl font-bold text-mu-gold">
          {isAdmin ? t.login.adminTitle : t.login.title}
        </h1>
        <p className="muted mt-1 mb-6">
          {isAdmin ? t.login.adminSubtitle : t.login.subtitle}
        </p>

        <FlashMessage type="error" message={error} />

        <form onSubmit={onSubmit} className="space-y-4">
          <div>
            <label className="label" htmlFor="account">
              {t.login.account}
            </label>
            <input
              id="account"
              className="input"
              value={account}
              onChange={(e) => setAccount(e.target.value)}
              autoComplete="username"
              required
              minLength={isAdmin ? 3 : 4}
              maxLength={isAdmin ? 32 : 10}
            />
          </div>
          <div>
            <label className="label" htmlFor="password">
              {t.login.password}
            </label>
            <input
              id="password"
              type="password"
              className="input"
              value={password}
              onChange={(e) => setPassword(e.target.value)}
              autoComplete="current-password"
              required
              minLength={isAdmin ? 4 : 4}
              maxLength={isAdmin ? 64 : 10}
            />
          </div>
          <button type="submit" className="btn-gold w-full" disabled={loading}>
            {loading ? t.login.submitting : t.login.submit}
          </button>
        </form>

        <div className="mt-6 flex flex-wrap items-center justify-between gap-2 text-sm">
          <Link href="/register" className="text-mu-lime hover:underline">
            {t.login.noAccount}
          </Link>
          {isAdmin ? (
            <Link href="/login" className="text-mu-gold hover:underline">
              {t.login.playerLogin}
            </Link>
          ) : (
            <Link
              href="/login?admin=1"
              className="text-purple-200 hover:underline"
            >
              {t.login.adminLogin}
            </Link>
          )}
        </div>
      </div>
    </div>
  );
}

export default function LoginPage() {
  return (
    <Suspense
      fallback={
        <div className="p-10 text-center text-mu-muted">
          <LoginFallback />
        </div>
      }
    >
      <LoginForm />
    </Suspense>
  );
}

function LoginFallback() {
  const { t } = useI18n();
  return <>{t.login.loading}</>;
}
