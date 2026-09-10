"use client";

import Link from "next/link";
import { useRouter } from "next/navigation";
import { signIn } from "next-auth/react";
import { FormEvent, useCallback, useEffect, useState } from "react";
import FlashMessage from "@/components/FlashMessage";
import { useI18n } from "@/components/I18nProvider";
import TurnstileWidget from "@/components/TurnstileWidget";

type BotMode = "turnstile" | "dev-captcha" | "blocked" | "loading";

export default function RegisterPage() {
  const { t } = useI18n();
  const router = useRouter();
  const [account, setAccount] = useState("");
  const [password, setPassword] = useState("");
  const [confirmPassword, setConfirmPassword] = useState("");
  const [email, setEmail] = useState("");
  const [captcha, setCaptcha] = useState("");
  const [captchaSvg, setCaptchaSvg] = useState<string | null>(null);
  const [captchaLoading, setCaptchaLoading] = useState(false);
  const [turnstileToken, setTurnstileToken] = useState("");
  const [turnstileSiteKey, setTurnstileSiteKey] = useState<string | null>(null);
  const [botMode, setBotMode] = useState<BotMode>("loading");
  const [error, setError] = useState<string | null>(null);
  const [success, setSuccess] = useState<string | null>(null);
  const [loading, setLoading] = useState(false);

  const loadBotMode = useCallback(async () => {
    try {
      const res = await fetch("/api/bot-mode", { cache: "no-store" });
      const data = await res.json().catch(() => ({}));
      const mode = (data.mode as BotMode) || "blocked";
      setBotMode(mode);
      setTurnstileSiteKey(
        typeof data.turnstileSiteKey === "string" ? data.turnstileSiteKey : null
      );
    } catch {
      setBotMode("blocked");
    }
  }, []);

  const loadCaptcha = useCallback(async () => {
    setCaptchaLoading(true);
    try {
      const res = await fetch("/api/captcha", {
        cache: "no-store",
        credentials: "same-origin",
      });
      const data = await res.json().catch(() => ({}));
      if (!res.ok || !data.svg) {
        setError(data.error || t.register.captchaLoadFail);
        return;
      }
      setCaptchaSvg(data.svg);
      setCaptcha("");
    } catch {
      setError(t.register.captchaLoadFail);
    } finally {
      setCaptchaLoading(false);
    }
  }, [t.register.captchaLoadFail]);

  useEffect(() => {
    void loadBotMode();
  }, [loadBotMode]);

  useEffect(() => {
    if (botMode === "dev-captcha") void loadCaptcha();
  }, [botMode, loadCaptcha]);

  async function onSubmit(e: FormEvent) {
    e.preventDefault();
    setError(null);
    setSuccess(null);

    if (password !== confirmPassword) {
      setError(t.register.mismatch);
      return;
    }
    if (botMode === "blocked" || botMode === "loading") {
      setError(
        "Đăng ký tạm khóa: chưa cấu hình chống bot (Turnstile). Liên hệ admin."
      );
      return;
    }
    if (botMode === "turnstile" && !turnstileToken.trim()) {
      setError("Vui lòng xác minh bạn không phải robot.");
      return;
    }
    if (botMode === "dev-captcha" && !captcha.trim()) {
      setError(t.register.needCaptcha);
      return;
    }

    setLoading(true);
    try {
      const res = await fetch("/api/register", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        credentials: "same-origin",
        body: JSON.stringify({
          account,
          password,
          confirmPassword,
          email,
          captcha: botMode === "dev-captcha" ? captcha : "",
          turnstileToken: botMode === "turnstile" ? turnstileToken : "",
        }),
      });
      const data = await res.json().catch(() => ({}));
      if (!res.ok) {
        setError(data.error || data.message || t.register.failed);
        setTurnstileToken("");
        if (botMode === "dev-captcha") await loadCaptcha();
        return;
      }

      setSuccess(t.register.success);
      const login = await signIn("credentials", {
        account,
        password,
        kind: "user",
        redirect: false,
      });
      if (login?.error) {
        setSuccess(t.register.success);
        router.push("/login");
        return;
      }
      router.push("/panel");
      router.refresh();
    } catch {
      setError(t.login.networkError);
      setTurnstileToken("");
      if (botMode === "dev-captcha") await loadCaptcha();
    } finally {
      setLoading(false);
    }
  }

  const submitDisabled =
    loading ||
    botMode === "loading" ||
    botMode === "blocked" ||
    (botMode === "dev-captcha" && captchaLoading);

  return (
    <div className="mx-auto flex min-h-[70vh] max-w-md items-center px-4 py-10">
      <div className="card-glow w-full p-6 md:p-8">
        <h1 className="font-display text-2xl font-bold text-mu-gold">
          {t.register.title}
        </h1>
        <p className="muted mt-1 mb-6">{t.register.subtitle}</p>

        <FlashMessage type="error" message={error} />
        <FlashMessage type="success" message={success} />

        {botMode === "blocked" ? (
          <p className="mb-4 rounded-lg border border-red-500/40 bg-red-950/40 p-3 text-sm text-red-200">
            Máy chủ chưa cấu hình Cloudflare Turnstile. Đăng ký đang bị khóa để
            chống bot.
          </p>
        ) : null}

        <form onSubmit={onSubmit} className="space-y-4">
          <div>
            <label className="label" htmlFor="account">
              {t.register.account}
            </label>
            <input
              id="account"
              className="input"
              value={account}
              onChange={(e) => setAccount(e.target.value)}
              required
              minLength={4}
              maxLength={10}
              pattern="[a-zA-Z0-9]+"
              autoComplete="username"
            />
          </div>
          <div>
            <label className="label" htmlFor="password">
              {t.register.password}
            </label>
            <input
              id="password"
              type="password"
              className="input"
              value={password}
              onChange={(e) => setPassword(e.target.value)}
              required
              minLength={4}
              maxLength={10}
              autoComplete="new-password"
            />
          </div>
          <div>
            <label className="label" htmlFor="confirmPassword">
              {t.register.confirmPassword}
            </label>
            <input
              id="confirmPassword"
              type="password"
              className="input"
              value={confirmPassword}
              onChange={(e) => setConfirmPassword(e.target.value)}
              required
              minLength={4}
              maxLength={10}
              autoComplete="new-password"
            />
          </div>
          <div>
            <label className="label" htmlFor="email">
              {t.register.email}
            </label>
            <input
              id="email"
              type="email"
              className="input"
              value={email}
              onChange={(e) => setEmail(e.target.value)}
              autoComplete="email"
            />
          </div>

          {botMode === "turnstile" && turnstileSiteKey ? (
            <div>
              <p className="label mb-2">Xác minh chống bot</p>
              <TurnstileWidget
                siteKey={turnstileSiteKey}
                onToken={setTurnstileToken}
                onExpire={() => setTurnstileToken("")}
              />
            </div>
          ) : null}

          {botMode === "dev-captcha" ? (
            <div>
              <label className="label" htmlFor="captcha">
                {t.register.captcha} (dev)
              </label>
              <div className="mb-2 flex items-center gap-3">
                <div className="overflow-hidden rounded-lg border border-mu-gold/30 bg-black/40">
                  {captchaSvg ? (
                    <img
                      src={`data:image/svg+xml;utf8,${encodeURIComponent(captchaSvg)}`}
                      alt="Captcha"
                      width={180}
                      height={56}
                      className="block h-14 w-[180px]"
                    />
                  ) : (
                    <div className="flex h-14 w-[180px] items-center justify-center text-xs text-mu-muted">
                      {captchaLoading ? t.common.loading : t.register.captcha}
                    </div>
                  )}
                </div>
                <button
                  type="button"
                  className="btn-ghost px-3 py-2 text-xs"
                  onClick={() => void loadCaptcha()}
                  disabled={captchaLoading}
                >
                  {t.register.refreshCaptcha}
                </button>
              </div>
              <input
                id="captcha"
                className="input"
                value={captcha}
                onChange={(e) => setCaptcha(e.target.value)}
                required
                inputMode="numeric"
                autoComplete="off"
              />
            </div>
          ) : null}

          <button
            type="submit"
            className="btn-gold w-full"
            disabled={submitDisabled}
          >
            {loading ? t.register.submitting : t.register.submit}
          </button>
        </form>

        <p className="mt-6 text-sm text-mu-muted">
          <Link href="/login" className="text-mu-gold hover:underline">
            {t.register.hasAccount}
          </Link>
        </p>
      </div>
    </div>
  );
}
