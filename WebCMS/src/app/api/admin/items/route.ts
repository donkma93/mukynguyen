import { NextResponse } from "next/server";
import { requireAdmin } from "@/lib/auth";
import { getItemInfo, searchItems } from "@/lib/gs/item-catalog";

export const dynamic = "force-dynamic";

export async function GET(req: Request) {
  try {
    await requireAdmin();
    const { searchParams } = new URL(req.url);
    const q = (searchParams.get("q") || "").trim();
    const index = searchParams.get("index");

    if (index !== null && index !== "") {
      const n = Number(index);
      if (!Number.isFinite(n)) {
        return NextResponse.json(
          { ok: false, error: "index không hợp lệ" },
          { status: 400 }
        );
      }
      const item = getItemInfo(n);
      if (!item) {
        return NextResponse.json(
          { ok: false, error: "Không tìm thấy item" },
          { status: 404 }
        );
      }
      return NextResponse.json({ ok: true, item });
    }

    if (!q) return NextResponse.json({ ok: true, items: [] });
    const items = searchItems(q, 50);
    return NextResponse.json({ ok: true, items });
  } catch (e) {
    if (e instanceof Error && e.message === "UNAUTHORIZED_ADMIN") {
      return NextResponse.json(
        { ok: false, error: "Bạn không có quyền admin" },
        { status: 401 }
      );
    }
    const message = e instanceof Error ? e.message : "Không tìm được item";
    return NextResponse.json({ ok: false, error: message }, { status: 500 });
  }
}
