import { NextResponse } from "next/server";
import { readTextFile } from "@/lib/gs/files";
import { buildPublicChaosMix } from "@/lib/gs/public-chaos-mix";

export const dynamic = "force-dynamic";

export async function GET() {
  try {
    const [chaosMixText, commonText] = await Promise.all([
      readTextFile("ini", "GameServerInfo - ChaosMix.ini"),
      readTextFile("ini", "GameServerInfo - Common.ini"),
    ]);
    const data = buildPublicChaosMix(chaosMixText, commonText);
    return NextResponse.json({ ok: true, data });
  } catch {
    return NextResponse.json(
      { ok: false, error: "unavailable" },
      { status: 503 }
    );
  }
}
