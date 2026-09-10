import { NextResponse } from "next/server";
import { requireAdmin } from "@/lib/auth";
import {
  getEnhancementRates,
  saveEnhancementRates,
  type EnhancementRateUpdate,
} from "@/lib/enhancement-rates";
import { clientSafeError } from "@/lib/security/client-error";

export const dynamic = "force-dynamic";

function authError(error: unknown) {
  if (error instanceof Error && error.message === "UNAUTHORIZED_ADMIN") {
    return NextResponse.json({ ok: false, error: "Bạn không có quyền admin" }, { status: 401 });
  }
  return null;
}

export async function GET() {
  try {
    await requireAdmin();
    return NextResponse.json({ ok: true, sections: await getEnhancementRates() });
  } catch (error) {
    const unauthorized = authError(error);
    if (unauthorized) return unauthorized;
    return NextResponse.json(
      { ok: false, error: clientSafeError(error, "Không tải được tỉ lệ đập đồ") },
      { status: 500 }
    );
  }
}

export async function PUT(request: Request) {
  try {
    await requireAdmin();
    const body = await request.json();
    const sections = await saveEnhancementRates(body.items as EnhancementRateUpdate[]);
    return NextResponse.json({
      ok: true,
      sections,
      message: "Đã lưu tỉ lệ. Hãy khởi động lại GameServer để áp dụng.",
    });
  } catch (error) {
    const unauthorized = authError(error);
    if (unauthorized) return unauthorized;
    return NextResponse.json(
      { ok: false, error: clientSafeError(error, "Không thể lưu tỉ lệ đập đồ") },
      { status: 400 }
    );
  }
}
