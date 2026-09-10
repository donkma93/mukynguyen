import { NextResponse } from "next/server";
import { fileExists, readTextFile } from "@/lib/gs/files";
import { buildPublicCommandsGuide } from "@/lib/gs/public-commands";

export const dynamic = "force-dynamic";

export async function GET() {
  try {
    const [commandIniText, commandTxtText] = await Promise.all([
      readTextFile("ini", "GameServerInfo - Command.ini"),
      readTextFile("data", "Command.txt"),
    ]);
    const movePath = "Move/Move.txt";
    const hasMove = await fileExists("data", movePath);
    const moveTxtText = hasMove ? await readTextFile("data", movePath) : null;

    const data = buildPublicCommandsGuide({
      commandIniText,
      commandTxtText,
      moveTxtText,
    });
    return NextResponse.json({ ok: true, data });
  } catch {
    return NextResponse.json(
      { ok: false, error: "unavailable" },
      { status: 503 }
    );
  }
}
