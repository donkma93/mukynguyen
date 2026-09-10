import { cookies } from "next/headers";
import { NextResponse } from "next/server";
import { CAPTCHA_COOKIE, verifyCaptchaCookie } from "@/lib/captcha";
import { registerAccount } from "@/lib/game";
import { getClientIp } from "@/lib/security/client-ip";
import { getRegisterBotMode } from "@/lib/security/env-guards";
import {
  consumeRateLimit,
  RATE_LIMITS,
} from "@/lib/security/rate-limit";
import { verifyTurnstileToken } from "@/lib/security/turnstile";
import { registerSchema } from "@/lib/validators";

export const dynamic = "force-dynamic";

export async function POST(req: Request) {
  try {
    const ip = getClientIp(req);
    const rl = consumeRateLimit(`register:${ip}`, RATE_LIMITS.register);
    if (!rl.ok) {
      return NextResponse.json(
        { ok: false, error: rl.reason },
        {
          status: 429,
          headers: { "Retry-After": String(rl.retryAfterSec) },
        }
      );
    }

    const body = await req.json().catch(() => ({}));
    const parsed = registerSchema.safeParse(body);
    if (!parsed.success) {
      const msg = parsed.error.issues[0]?.message || "Dữ liệu không hợp lệ";
      return NextResponse.json({ ok: false, error: msg }, { status: 400 });
    }

    const mode = getRegisterBotMode();
    if (mode === "blocked") {
      return NextResponse.json(
        {
          ok: false,
          error:
            "Đăng ký tạm khóa: máy chủ chưa cấu hình Turnstile. Liên hệ admin.",
        },
        { status: 503 }
      );
    }

    if (mode === "turnstile") {
      const ts = await verifyTurnstileToken(
        parsed.data.turnstileToken,
        ip
      );
      if (!ts.ok) {
        return NextResponse.json({ ok: false, error: ts.reason }, { status: 400 });
      }
    } else {
      // Dev math captcha fallback
      if (!parsed.data.captcha?.trim()) {
        return NextResponse.json(
          { ok: false, error: "Vui lòng nhập mã captcha" },
          { status: 400 }
        );
      }
      const jar = await cookies();
      const token = jar.get(CAPTCHA_COOKIE)?.value;
      const captcha = verifyCaptchaCookie(token, parsed.data.captcha);
      if (!captcha.ok) {
        const res = NextResponse.json(
          { ok: false, error: captcha.reason },
          { status: 400 }
        );
        res.cookies.set(CAPTCHA_COOKIE, "", {
          httpOnly: true,
          path: "/",
          maxAge: 0,
        });
        return res;
      }
    }

    const result = await registerAccount({
      account: parsed.data.account,
      password: parsed.data.password,
      email: parsed.data.email || undefined,
    });

    const res = NextResponse.json({ ok: true, account: result.account });
    res.cookies.set(CAPTCHA_COOKIE, "", { httpOnly: true, path: "/", maxAge: 0 });
    return res;
  } catch (e) {
    const message = e instanceof Error ? e.message : "Đăng ký thất bại";
    const status = message.includes("tồn tại") ? 409 : 500;
    return NextResponse.json({ ok: false, error: message }, { status });
  }
}
