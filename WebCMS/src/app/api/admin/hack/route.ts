import { NextResponse } from "next/server";
import { requireAdmin } from "@/lib/auth";
import { scanHackLogs, type HackLogSource } from "@/lib/gs/hack-logs";
import { clientSafeError } from "@/lib/security/client-error";

export const dynamic = "force-dynamic";

export async function GET(req: Request) {
  try {
    await requireAdmin();
    const { searchParams } = new URL(req.url);
    const q = (searchParams.get("q") || "").trim();
    const days = Number(searchParams.get("days") || 14);
    const limit = Number(searchParams.get("limit") || 200);
    const sourceParam = (searchParams.get("source") || "all").toLowerCase();

    let sources: HackLogSource[] | undefined;
    if (sourceParam === "gs") sources = ["gs"];
    else if (sourceParam === "antihack" || sourceParam === "ah")
      sources = ["antihack"];
    else sources = ["gs", "antihack"];

    const result = await scanHackLogs({
      q,
      days: Number.isFinite(days) ? days : 14,
      limit: Number.isFinite(limit) ? limit : 200,
      sources,
    });

    return NextResponse.json({ ok: true, ...result });
  } catch (e) {
    if (e instanceof Error && e.message === "UNAUTHORIZED_ADMIN") {
      return NextResponse.json(
        { ok: false, error: "Bạn không có quyền admin" },
        { status: 401 }
      );
    }
    return NextResponse.json(
      { ok: false, error: clientSafeError(e, "Không đọc được hack log") },
      { status: 500 }
    );
  }
}
