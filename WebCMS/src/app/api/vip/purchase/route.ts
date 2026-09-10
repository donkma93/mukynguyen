import { NextResponse } from "next/server";
import { requireUser } from "@/lib/auth";
import { purchaseVipWithWcoin } from "@/lib/game";
import {
  consumeRateLimit,
  RATE_LIMITS,
} from "@/lib/security/rate-limit";
import { getVipPackage } from "@/lib/vip-shop";

export const dynamic = "force-dynamic";

export async function POST(req: Request) {
  try {
    const session = await requireUser();
    const rl = consumeRateLimit(
      `vip:${session.user.id}`,
      RATE_LIMITS.vipPurchase
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

    const body = await req.json().catch(() => ({}));
    const packageId = String(body.packageId ?? body.id ?? "").trim();
    if (!getVipPackage(packageId)) {
      return NextResponse.json(
        { ok: false, error: "Gói VIP không hợp lệ (vip7 / vip15 / vip30)" },
        { status: 400 }
      );
    }

    // Account is always taken from the session — ignore any spoofed body.account
    const result = await purchaseVipWithWcoin(session.user.id, packageId);
    const { ok: _ok, expireDate, ...rest } = result;
    return NextResponse.json({
      ok: true,
      ...rest,
      expireDate: expireDate ? new Date(expireDate).toISOString() : null,
      message: `Đã kích hoạt VIP ${result.days} ngày. WCoin còn lại: ${result.wcLeft}`,
    });
  } catch (e) {
    if (e instanceof Error && e.message === "UNAUTHORIZED_USER") {
      return NextResponse.json(
        { ok: false, error: "Bạn cần đăng nhập tài khoản game" },
        { status: 401 }
      );
    }
    const message = e instanceof Error ? e.message : "Mua VIP thất bại";
    const status =
      message.includes("Không đủ") || message.includes("không hợp lệ") ? 400 : 500;
    return NextResponse.json({ ok: false, error: message }, { status });
  }
}
