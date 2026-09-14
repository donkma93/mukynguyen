import { NextResponse } from "next/server";
import { requireUser } from "@/lib/auth";
import { claimPartnerCode } from "@/lib/partner/gifts";
import {
  consumeRateLimit,
  RATE_LIMITS,
} from "@/lib/security/rate-limit";
import { partnerClaimSchema } from "@/lib/validators";

export const dynamic = "force-dynamic";

export async function POST(req: Request) {
  try {
    const session = await requireUser();
    const rl = consumeRateLimit(
      `partner-claim:${session.user.id}`,
      RATE_LIMITS.partnerClaim
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
    const parsed = partnerClaimSchema.safeParse(body);
    if (!parsed.success) {
      const msg = parsed.error.issues[0]?.message || "Dữ liệu không hợp lệ";
      return NextResponse.json({ ok: false, error: msg }, { status: 400 });
    }

    const result = await claimPartnerCode({
      claimerAccount: session.user.id,
      code: parsed.data.code,
      characterName: parsed.data.characterName,
    });

    const coinParts = [
      result.reward.wc ? `${result.reward.wc} WC` : null,
      result.reward.wp ? `${result.reward.wp} WP` : null,
      result.reward.wg ? `${result.reward.wg} WG` : null,
    ].filter(Boolean);
    const itemParts = result.items.map((i) => `${i.label} x${i.quantity}`);

    return NextResponse.json({
      ok: true,
      message: `Nhận quà ${result.packageId} thành công${
        coinParts.length ? `: ${coinParts.join(", ")}` : ""
      }. Ngọc/bùa sẽ vào nhân vật khi vào game.`,
      itemSummary: itemParts,
      ...result,
    });
  } catch (e) {
    if (e instanceof Error && e.message === "UNAUTHORIZED_USER") {
      return NextResponse.json(
        { ok: false, error: "Bạn cần đăng nhập tài khoản game" },
        { status: 401 }
      );
    }
    const message = e instanceof Error ? e.message : "Nhận quà đối tác thất bại";
    const status =
      message.includes("không tồn tại") ||
      message.includes("hết hạn") ||
      message.includes("vô hiệu") ||
      message.includes("hết lượt") ||
      message.includes("đã nhận") ||
      message.includes("1 lần") ||
      message.includes("7 ngày") ||
      message.includes("không thuộc") ||
      message.includes("ngân sách") ||
      message.includes("hạn mức") ||
      message.includes("không còn hoạt động")
        ? 400
        : 500;
    return NextResponse.json({ ok: false, error: message }, { status });
  }
}
