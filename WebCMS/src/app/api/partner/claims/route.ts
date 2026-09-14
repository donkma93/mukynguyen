import { NextResponse } from "next/server";
import { requireUser } from "@/lib/auth";
import { listPartnerClaims } from "@/lib/partner/gifts";
import { isActivePartner } from "@/lib/partner/partners";

export const dynamic = "force-dynamic";

export async function GET() {
  try {
    const session = await requireUser();
    if (!(await isActivePartner(session.user.id))) {
      return NextResponse.json(
        { ok: false, error: "Bạn không phải đối tác đang hoạt động" },
        { status: 403 }
      );
    }
    const items = await listPartnerClaims(session.user.id, 50);
    return NextResponse.json({ ok: true, items });
  } catch (e) {
    if (e instanceof Error && e.message === "UNAUTHORIZED_USER") {
      return NextResponse.json(
        { ok: false, error: "Bạn cần đăng nhập tài khoản game" },
        { status: 401 }
      );
    }
    const message = e instanceof Error ? e.message : "Không tải được lịch sử";
    return NextResponse.json({ ok: false, error: message }, { status: 500 });
  }
}
