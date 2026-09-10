import { NextResponse } from "next/server";
import { readTextFile } from "@/lib/gs/files";
import { loadGroupRatesSnapshot } from "@/lib/gs/groups";
import { buildPublicFarmGuideFromGroups } from "@/lib/gs/public-farm";

export const dynamic = "force-dynamic";

export async function GET() {
  try {
    const [snapshot, mapManagerText] = await Promise.all([
      loadGroupRatesSnapshot(),
      readTextFile("data", "MapManager.txt"),
    ]);
    const data = buildPublicFarmGuideFromGroups(snapshot, mapManagerText);
    return NextResponse.json({ ok: true, data });
  } catch {
    return NextResponse.json(
      { ok: false, error: "unavailable" },
      { status: 503 }
    );
  }
}
