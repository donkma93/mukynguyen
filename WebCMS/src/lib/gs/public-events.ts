import {
  iniBool01,
  memScriptDataLines,
  parseIniValues,
  splitMemScriptSections,
  tokenizeRow,
} from "@/lib/gs/public-guide-parse";
import type { PublicEventSchedules } from "@/lib/gs/public-event-schedule";
import { buildPublicEventSchedules } from "@/lib/gs/public-event-schedule";

export type PublicEventFlag = {
  id: string;
  enabled: boolean | null;
};

export type PublicCtcMini = {
  hour: number | null;
  minute: number | null;
  enabled: boolean | null;
  prepareMinutes: number | null;
  eventMinutes: number | null;
  gateRewardWCoin: number | null;
  towerRewardWCoin: number | null;
  guildWinWCoin: number | null;
};

export type PublicCustomEventDrop = {
  switchOn: boolean | null;
  name: string | null;
  map: number | null;
  x: number | null;
  y: number | null;
  alarmMinutes: number | null;
  durationMinutes: number | null;
  times: { hour: number; minute: number }[];
  drops: {
    itemIndex: number;
    itemLevel: number;
    dropCount: number;
    comment: string | null;
  }[];
};

export type PublicEventBagSummary = {
  id: string;
  label: string;
  eventName: string | null;
  dropZen: number | null;
  itemDropRate: number | null;
  rewards: {
    name: string;
    rate: number | null;
    minLevel: number | null;
    maxLevel: number | null;
  }[];
};

export type PublicEventsGuide = {
  flags: PublicEventFlag[];
  ctcMini: PublicCtcMini | null;
  customEventDrop: PublicCustomEventDrop | null;
  bags: PublicEventBagSummary[];
  /** Live MemScript schedules from Data/Event/*.dat */
  schedules: PublicEventSchedules;
  /** @deprecated Phase 3 reads .dat as MemScript text — kept for older clients. */
  classicScheduleNote: "live" | "encoded";
};

export function emptyPublicEventsGuide(): PublicEventsGuide {
  return {
    flags: [],
    ctcMini: null,
    customEventDrop: null,
    bags: [],
    schedules: { classic: [], crywolf: null, invasions: [] },
    classicScheduleNote: "live",
  };
}

function attr(tag: string, name: string): string | null {
  const re = new RegExp(`${name}\\s*=\\s*"([^"]*)"`, "i");
  const m = tag.match(re);
  return m ? m[1] : null;
}

function parseCtcMiniXml(xml: string): PublicCtcMini {
  const timeTag = xml.match(/<Time\b[^/]*\/?>/i)?.[0] ?? "";
  const configTag = xml.match(/<CauHinhTime>[\s\S]*?<Config\b[^/]*\/?>/i)?.[0] ?? "";
  const phaCong = xml.match(/<PhaCong>[\s\S]*?<Config\b[^/]*\/?>/i)?.[0] ?? "";
  const phaTru = xml.match(/<PhaTru>[\s\S]*?<Config\b[^/]*\/?>/i)?.[0] ?? "";
  const guildWin = xml.match(/<GuidWin>[\s\S]*?<Config\b[^/]*\/?>/i)?.[0] ?? "";

  const hour = Number(attr(timeTag, "Hour"));
  const minute = Number(attr(timeTag, "Minute"));
  const enabledRaw = attr(configTag, "Enabled");
  const prepare = Number(attr(configTag, "TimeChuanBi"));
  const duration = Number(attr(configTag, "TimeSuKien"));
  const gateW = Number(attr(phaCong, "WCoin"));
  const towerW = Number(attr(phaTru, "WCoin"));
  const winW = Number(attr(guildWin, "WCoin"));

  return {
    hour: Number.isFinite(hour) ? hour : null,
    minute: Number.isFinite(minute) ? minute : null,
    enabled: enabledRaw == null ? null : enabledRaw !== "0",
    prepareMinutes: Number.isFinite(prepare) ? prepare : null,
    eventMinutes: Number.isFinite(duration) ? duration : null,
    gateRewardWCoin: Number.isFinite(gateW) ? gateW : null,
    towerRewardWCoin: Number.isFinite(towerW) ? towerW : null,
    guildWinWCoin: Number.isFinite(winW) ? winW : null,
  };
}

function parseCustomEventDrop(
  text: string,
  switchOn: boolean | null
): PublicCustomEventDrop {
  const sections = splitMemScriptSections(text);
  const times: { hour: number; minute: number }[] = [];
  for (const line of memScriptDataLines(sections.get(0) ?? "")) {
    const t = tokenizeRow(line);
    // Index Year Month Day DoW Hour Minute Second
    if (t.length < 7) continue;
    const hour = Number(t[5]);
    const minute = Number(t[6]);
    if (Number.isFinite(hour) && Number.isFinite(minute)) {
      times.push({ hour, minute });
    }
  }

  let name: string | null = null;
  let map: number | null = null;
  let x: number | null = null;
  let y: number | null = null;
  let alarmMinutes: number | null = null;
  let durationMinutes: number | null = null;
  const infoLine = memScriptDataLines(sections.get(1) ?? "")[0];
  if (infoLine) {
    const t = tokenizeRow(infoLine);
    // Index Name DropMap DropX DropY DropRange AlarmTime EventTime
    if (t.length >= 8) {
      name = t[1] || null;
      map = Number.isFinite(Number(t[2])) ? Number(t[2]) : null;
      x = Number.isFinite(Number(t[3])) ? Number(t[3]) : null;
      y = Number.isFinite(Number(t[4])) ? Number(t[4]) : null;
      alarmMinutes = Number.isFinite(Number(t[6])) ? Number(t[6]) : null;
      durationMinutes = Number.isFinite(Number(t[7])) ? Number(t[7]) : null;
    }
  }

  const drops: PublicCustomEventDrop["drops"] = [];
  for (const line of memScriptDataLines(sections.get(2) ?? "")) {
    const t = tokenizeRow(line);
    if (t.length < 4) continue;
    const itemIndex = Number(t[1]);
    const itemLevel = Number(t[2]);
    const dropCount = Number(t[3]);
    if (!Number.isFinite(itemIndex)) continue;
    const commentMatch = line.match(/\/\/\s*(.+)$/);
    drops.push({
      itemIndex,
      itemLevel: Number.isFinite(itemLevel) ? itemLevel : 0,
      dropCount: Number.isFinite(dropCount) ? dropCount : 0,
      comment: commentMatch ? commentMatch[1].trim() : null,
    });
  }

  return {
    switchOn,
    name,
    map,
    x,
    y,
    alarmMinutes,
    durationMinutes,
    times,
    drops,
  };
}

const FLAG_KEYS: { id: string; sources: ("common" | "event" | "custom")[]; key: string }[] = [
  { id: "bloodCastle", sources: ["common"], key: "BloodCastleEvent" },
  { id: "devilSquare", sources: ["common"], key: "DevilSquareEvent" },
  { id: "chaosCastle", sources: ["common"], key: "ChaosCastleEvent" },
  { id: "crywolf", sources: ["event"], key: "CrywolfEvent" },
  { id: "illusionTemple", sources: ["event"], key: "IllusionTempleEvent" },
  { id: "kanturu", sources: ["event"], key: "KanturuEvent" },
  { id: "castleDeep", sources: ["event"], key: "CastleDeepEvent" },
  { id: "mossMerchant", sources: ["event"], key: "MossMerchantEvent" },
  { id: "tvt", sources: ["event"], key: "EventTvtSwitch" },
  { id: "gvg", sources: ["event"], key: "EventGvGSwitch" },
];

export function buildPublicEventsGuide(input: {
  commonText: string;
  eventText: string;
  customText: string;
  customEventDropText: string | null;
  ctcMiniXml: string | null;
  bags?: PublicEventBagSummary[];
  classicTexts?: Record<string, string | null>;
  crywolfText?: string | null;
  invasionText?: string | null;
}): PublicEventsGuide {
  const common = parseIniValues(input.commonText);
  const event = parseIniValues(input.eventText);
  const custom = parseIniValues(input.customText);
  const bags: Record<"common" | "event" | "custom", Record<string, string>> = {
    common,
    event,
    custom,
  };

  const flags: PublicEventFlag[] = FLAG_KEYS.map((f) => {
    let enabled: boolean | null = null;
    for (const src of f.sources) {
      if (f.key in bags[src]) {
        enabled = iniBool01(bags[src][f.key]);
        break;
      }
    }
    return { id: f.id, enabled };
  });

  const switchOn = iniBool01(custom.CustomEventDropSwitch);
  const customEventDrop =
    input.customEventDropText != null
      ? parseCustomEventDrop(input.customEventDropText, switchOn)
      : {
          switchOn,
          name: null,
          map: null,
          x: null,
          y: null,
          alarmMinutes: null,
          durationMinutes: null,
          times: [],
          drops: [],
        };

  const schedules = buildPublicEventSchedules({
    classicTexts: input.classicTexts ?? {},
    crywolfText: input.crywolfText ?? null,
    invasionText: input.invasionText ?? null,
  });

  return {
    flags,
    ctcMini: input.ctcMiniXml ? parseCtcMiniXml(input.ctcMiniXml) : null,
    customEventDrop,
    bags: input.bags ?? [],
    schedules,
    classicScheduleNote: "live",
  };
}

export function formatHm(hour: number | null, minute: number | null): string | null {
  if (hour == null || minute == null) return null;
  return `${String(hour).padStart(2, "0")}:${String(minute).padStart(2, "0")}`;
}
