export type TurnstileVerifyResult =
  | { ok: true }
  | { ok: false; reason: string };

export function isProductionRuntime() {
  return process.env.NODE_ENV === "production";
}

export function getTurnstileSiteKey(): string | null {
  const k = process.env.TURNSTILE_SITE_KEY?.trim() || process.env.NEXT_PUBLIC_TURNSTILE_SITE_KEY?.trim();
  return k || null;
}

export function getTurnstileSecretKey(): string | null {
  const k = process.env.TURNSTILE_SECRET_KEY?.trim();
  return k || null;
}

/** Dev-only math captcha fallback when Turnstile keys are absent. */
export function allowDevCaptchaFallback(): boolean {
  if (isProductionRuntime()) return false;
  const flag = (process.env.ALLOW_DEV_CAPTCHA || "1").trim().toLowerCase();
  return flag === "1" || flag === "true" || flag === "yes";
}

export function isTurnstileConfigured(): boolean {
  return Boolean(getTurnstileSiteKey() && getTurnstileSecretKey());
}

/**
 * Verify a Turnstile token with Cloudflare.
 * In production, missing secret → fail closed.
 */
export async function verifyTurnstileToken(
  token: string | undefined | null,
  remoteip?: string
): Promise<TurnstileVerifyResult> {
  const secret = getTurnstileSecretKey();
  if (!secret) {
    if (isProductionRuntime()) {
      return { ok: false, reason: "Turnstile chưa được cấu hình trên máy chủ" };
    }
    return { ok: false, reason: "Turnstile secret missing" };
  }

  const trimmed = String(token || "").trim();
  if (!trimmed) {
    return { ok: false, reason: "Vui lòng xác minh bạn không phải robot (Turnstile)" };
  }

  try {
    const body = new URLSearchParams();
    body.set("secret", secret);
    body.set("response", trimmed);
    if (remoteip && remoteip !== "unknown") body.set("remoteip", remoteip);

    const res = await fetch(
      "https://challenges.cloudflare.com/turnstile/v0/siteverify",
      {
        method: "POST",
        headers: { "Content-Type": "application/x-www-form-urlencoded" },
        body,
        cache: "no-store",
      }
    );
    const data = (await res.json().catch(() => null)) as {
      success?: boolean;
      "error-codes"?: string[];
    } | null;

    if (!data?.success) {
      return {
        ok: false,
        reason: "Xác minh Turnstile thất bại. Hãy thử lại.",
      };
    }
    return { ok: true };
  } catch {
    return {
      ok: false,
      reason: "Không thể xác minh Turnstile. Thử lại sau.",
    };
  }
}
