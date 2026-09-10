import { NextResponse } from "next/server";
import {
  CAPTCHA_COOKIE,
  createCaptchaChallenge,
  encodeCaptchaCookie,
} from "@/lib/captcha";
import { getClientIp } from "@/lib/security/client-ip";
import { allowDevCaptchaFallback } from "@/lib/security/turnstile";
import {
  consumeRateLimit,
  RATE_LIMITS,
} from "@/lib/security/rate-limit";

export const dynamic = "force-dynamic";

export async function GET(req: Request) {
  if (!allowDevCaptchaFallback()) {
    return NextResponse.json(
      {
        ok: false,
        error: "Math captcha chỉ dùng khi ALLOW_DEV_CAPTCHA=1 (không production).",
      },
      { status: 403 }
    );
  }

  const ip = getClientIp(req);
  const rl = consumeRateLimit(`captcha:${ip}`, RATE_LIMITS.captcha);
  if (!rl.ok) {
    return NextResponse.json(
      { ok: false, error: rl.reason },
      {
        status: 429,
        headers: { "Retry-After": String(rl.retryAfterSec) },
      }
    );
  }

  const challenge = createCaptchaChallenge();
  const token = encodeCaptchaCookie(challenge.answer, challenge.id);
  const secure =
    (process.env.NEXTAUTH_URL || "").startsWith("https://") ||
    process.env.NODE_ENV === "production";
  const res = NextResponse.json({
    ok: true,
    svg: challenge.svg,
  });
  res.cookies.set(CAPTCHA_COOKIE, token, {
    httpOnly: true,
    sameSite: "lax",
    path: "/",
    maxAge: 10 * 60,
    secure,
  });
  return res;
}
