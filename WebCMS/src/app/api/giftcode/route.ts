import { NextResponse } from "next/server";
import { requireUser } from "@/lib/auth";
import { claimGiftcode } from "@/lib/cms";
import {
  consumeRateLimit,
  RATE_LIMITS,
} from "@/lib/security/rate-limit";
import { giftcodeSchema } from "@/lib/validators";

export const dynamic = "force-dynamic";

export async function POST(req: Request) {
  try {
    const session = await requireUser();
    const rl = consumeRateLimit(
      `giftcode:${session.user.id}`,
      RATE_LIMITS.giftcode
    );
    if (!rl.ok) {
      return NextResponse.json(
        { ok: false, error: rl.reason },
        {
          status: 429,
          headers: { "Retry-After": String(rl.retryAfterSec) },
        }
      );
    }

    const body = await req.json();
    const parsed = giftcodeSchema.safeParse(body);
    if (!parsed.success) {
      const msg = parsed.error.issues[0]?.message || "Giftcode không hợp lệ";
      return NextResponse.json({ ok: false, error: msg }, { status: 400 });
    }

    const result = await claimGiftcode(session.user.id, parsed.data.code);
    return NextResponse.json({ ok: true, ...result });
  } catch (e) {
    if (e instanceof Error && e.message === "UNAUTHORIZED_USER") {
      return NextResponse.json(
        { ok: false, error: "Bạn cần đăng nhập tài khoản game" },
        { status: 401 }
      );
    }
    const message = e instanceof Error ? e.message : "Nhận giftcode thất bại";
    const status =
      message.includes("không tồn tại") ||
      message.includes("hết hạn") ||
      message.includes("vô hiệu") ||
      message.includes("hết lượt") ||
      message.includes("đã nhận")
        ? 400
        : 500;
    return NextResponse.json({ ok: false, error: message }, { status });
  }
}
