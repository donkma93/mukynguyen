import { NextResponse } from "next/server";
import { requireAdmin } from "@/lib/auth";
import { listIniFiles } from "@/lib/gs/files";
import { clientSafeError } from "@/lib/security/client-error";

export const dynamic = "force-dynamic";

export async function GET() {
  try {
    await requireAdmin();
    const items = await listIniFiles();
    return NextResponse.json({
      ok: true,
      items,
    });
  } catch (e) {
    if (e instanceof Error && e.message === "UNAUTHORIZED_ADMIN") {
      return NextResponse.json(
        { ok: false, error: "Bạn không có quyền admin" },
        { status: 401 }
      );
    }
    return NextResponse.json(
      { ok: false, error: clientSafeError(e, "Không liệt kê được INI") },
      { status: 500 }
    );
  }
}
