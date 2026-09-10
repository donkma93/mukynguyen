import { NextResponse } from "next/server";
import { readTextFile } from "@/lib/gs/files";
import { buildPublicSkillsGuide } from "@/lib/gs/public-skills";

export const dynamic = "force-dynamic";

export async function GET() {
  try {
    const [skillIniText, defaultClassText] = await Promise.all([
      readTextFile("ini", "GameServerInfo - Skill.ini"),
      readTextFile("data", "Character/DefaultClassInfo.txt"),
    ]);
    const data = buildPublicSkillsGuide({ skillIniText, defaultClassText });
    return NextResponse.json({ ok: true, data });
  } catch {
    return NextResponse.json(
      { ok: false, error: "unavailable" },
      { status: 503 }
    );
  }
}
