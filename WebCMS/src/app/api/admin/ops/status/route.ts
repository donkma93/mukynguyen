import { NextResponse } from "next/server";
import { requireAdmin } from "@/lib/auth";
import { getOpsStatus } from "@/lib/gs/ops";
import { getOnlineCount } from "@/lib/game";
import { clientSafeError } from "@/lib/security/client-error";

export const dynamic = "force-dynamic";

export async function GET() {
  try {
    await requireAdmin();
    const status = await getOpsStatus();
    let online = 0;
    try {
      online = await getOnlineCount();
    } catch {
      online = 0;
    }
    return NextResponse.json({ ok: true, ...status, online });
  } catch (e) {
    if (e instanceof Error && e.message === "UNAUTHORIZED_ADMIN") {
      return NextResponse.json(
        { ok: false, error: "Bạn không có quyền admin" },
        { status: 401 }
      );
    }
    return NextResponse.json(
      { ok: false, error: clientSafeError(e, "Không lấy được trạng thái") },
      { status: 500 }
    );
  }
}
