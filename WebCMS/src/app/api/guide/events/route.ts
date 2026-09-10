import { NextResponse } from "next/server";
import { fileExists, readTextFile } from "@/lib/gs/files";
import { buildPublicEventBagSummaries } from "@/lib/gs/public-event-bags";
import { EVENT_SCHEDULE_FILES } from "@/lib/gs/public-event-schedule";
import { buildPublicEventsGuide } from "@/lib/gs/public-events";

export const dynamic = "force-dynamic";

async function readDataOptional(relativePath: string): Promise<string | null> {
  if (!(await fileExists("data", relativePath))) return null;
  return readTextFile("data", relativePath);
}

export async function GET() {
  try {
    const [commonText, eventText, customText] = await Promise.all([
      readTextFile("ini", "GameServerInfo - Common.ini"),
      readTextFile("ini", "GameServerInfo - Event.ini"),
      readTextFile("ini", "GameServerInfo - Custom.ini"),
    ]);

    const dropPath = "Custom/CustomEventDrop.txt";
    const ctcPath = "Event/CTCMini/CongThanhChien.xml";
    const crywolfPath = "Event/Crywolf.dat";
    const invasionPath = "Event/InvasionManager.dat";

    const [hasDrop, hasCtc, hasCrywolf, hasInvasion] = await Promise.all([
      fileExists("data", dropPath),
      fileExists("data", ctcPath),
      fileExists("data", crywolfPath),
      fileExists("data", invasionPath),
    ]);

    const classicTextsEntries = await Promise.all(
      EVENT_SCHEDULE_FILES.map(async ({ id, file }) => {
        const text = await readDataOptional(file);
        return [id, text] as const;
      })
    );
    const classicTexts = Object.fromEntries(classicTextsEntries) as Record<
      string,
      string | null
    >;

    const [customEventDropText, ctcMiniXml, bags, crywolfText, invasionText] =
      await Promise.all([
        hasDrop ? readTextFile("data", dropPath) : Promise.resolve(null),
        hasCtc ? readTextFile("data", ctcPath) : Promise.resolve(null),
        buildPublicEventBagSummaries().catch(() => []),
        hasCrywolf ? readTextFile("data", crywolfPath) : Promise.resolve(null),
        hasInvasion ? readTextFile("data", invasionPath) : Promise.resolve(null),
      ]);

    const data = buildPublicEventsGuide({
      commonText,
      eventText,
      customText,
      customEventDropText,
      ctcMiniXml,
      bags,
      classicTexts,
      crywolfText,
      invasionText,
    });
    return NextResponse.json({ ok: true, data });
  } catch {
    return NextResponse.json(
      { ok: false, error: "unavailable" },
      { status: 503 }
    );
  }
}
