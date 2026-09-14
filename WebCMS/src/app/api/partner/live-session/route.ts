import { NextResponse } from "next/server";
import { requireUser } from "@/lib/auth";
import { createLiveSession } from "@/lib/partner/gifts";
import { isActivePartner } from "@/lib/partner/partners";

export const dynamic = "force-dynamic";

export async function POST() {
  try {
    const session = await requireUser();
    if (!(await isActivePartner(session.user.id))) {
      return NextResponse.json(
        { ok: false, error: "Bạn không phải đối tác đang hoạt động" },
        { status: 403 }
      );
    }

    const result = await createLiveSession(session.user.id);
    return NextResponse.json({
      ok: true,
      message: "Đã tạo mã Live Drop",
      code: result.session.code,
      expiresAt: result.session.expiresAt,
      session: result.session,
    });
  } catch (e) {
    if (e instanceof Error && e.message === "UNAUTHORIZED_USER") {
      return NextResponse.json(
        { ok: false, error: "Bạn cần đăng nhập tài khoản game" },
        { status: 401 }
      );
    }
    const message = e instanceof Error ? e.message : "Tạo mã Live Drop thất bại";
    const status =
      message.includes("hạn mức") || message.includes("ngân sách") ? 400 : 500;
    return NextResponse.json({ ok: false, error: message }, { status });
  }
}
