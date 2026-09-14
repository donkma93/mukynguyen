import { NextResponse } from "next/server";
import { requireUser } from "@/lib/auth";
import { grantHighlight } from "@/lib/partner/gifts";
import { isActivePartner } from "@/lib/partner/partners";
import {
  consumeRateLimit,
  RATE_LIMITS,
} from "@/lib/security/rate-limit";
import { partnerHighlightSchema } from "@/lib/validators";

export const dynamic = "force-dynamic";

export async function POST(req: Request) {
  try {
    const session = await requireUser();
    if (!(await isActivePartner(session.user.id))) {
      return NextResponse.json(
        { ok: false, error: "Bạn không phải đối tác đang hoạt động" },
        { status: 403 }
      );
    }

    const rl = consumeRateLimit(
      `partner-highlight:${session.user.id}`,
      RATE_LIMITS.partnerHighlight
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
    const parsed = partnerHighlightSchema.safeParse(body);
    if (!parsed.success) {
      const msg = parsed.error.issues[0]?.message || "Dữ liệu không hợp lệ";
      return NextResponse.json({ ok: false, error: msg }, { status: 400 });
    }

    const result = await grantHighlight({
      partnerAccount: session.user.id,
      targetAccount: parsed.data.targetAccount,
      characterName: parsed.data.characterName,
    });

    return NextResponse.json({
      ok: true,
      message: `Đã phát Highlight cho ${result.targetAccount} / ${result.character}`,
      ...result,
    });
  } catch (e) {
    if (e instanceof Error && e.message === "UNAUTHORIZED_USER") {
      return NextResponse.json(
        { ok: false, error: "Bạn cần đăng nhập tài khoản game" },
        { status: 401 }
      );
    }
    const message = e instanceof Error ? e.message : "Phát Highlight thất bại";
    const status =
      message.includes("hạn mức") ||
      message.includes("ngân sách") ||
      message.includes("không tồn tại") ||
      message.includes("không thuộc") ||
      message.includes("chính mình") ||
      message.includes("hôm nay") ||
      message.includes("tối đa")
        ? 400
        : 500;
    return NextResponse.json({ ok: false, error: message }, { status });
  }
}
