import {
  memScriptDataLines,
  splitMemScriptSections,
  tokenizeRow,
} from "@/lib/gs/public-guide-parse";

/** MemScript `*` → -1 in GS; we keep null for wildcards. */
export type ScheduleField = number | null;

export type PublicScheduleSlot = {
  year: ScheduleField;
  month: ScheduleField;
  day: ScheduleField;
  /** GS CTime: 1=Sun … 7=Sat; null = any. */
  dayOfWeek: ScheduleField;
  hour: ScheduleField;
  minute: ScheduleField;
  second: ScheduleField;
};

export type PublicEventTiming = {
  warningMinutes: number | null;
  notifyMinutes: number | null;
  eventMinutes: number | null;
  closeMinutes: number | null;
};

export type PublicClassicEventSchedule = {
  id: string;
  /** Relative path under Data/ for provenance. */
  source: string;
  loaded: boolean;
  empty: boolean;
  timing: PublicEventTiming | null;
  slots: PublicScheduleSlot[];
  /** Deduped HH:MM (or :MM / *:* patterns) for UI. */
  times: string[];
  /** Short pattern hint when slots form an obvious daily rhythm. */
  pattern: string | null;
  /** Extra note (e.g. Castle Deep schedule commented out). */
  note: string | null;
  durationMinutes: number | null;
};

export type PublicInvasionBossSchedule = {
  index: number;
  name: string;
  times: string[];
  pattern: string | null;
  slots: PublicScheduleSlot[];
};

export type PublicCrywolfSchedule = {
  loaded: boolean;
  empty: boolean;
  source: string;
  entries: {
    dayOfWeek: ScheduleField;
    hour: number;
    minute: number;
    continuanceMinutes: number | null;
    timeLabel: string;
    dayLabel: string | null;
  }[];
};

export type PublicEventSchedules = {
  classic: PublicClassicEventSchedule[];
  crywolf: PublicCrywolfSchedule | null;
  invasions: PublicInvasionBossSchedule[];
};

/** Stable day keys for UI i18n (GS DoW: 1=Sun … 7=Sat). */
export const DOW_KEYS = [
  null,
  "sun",
  "mon",
  "tue",
  "wed",
  "thu",
  "fri",
  "sat",
] as const;

function parseField(token: string | undefined): ScheduleField {
  if (token == null || token === "" || token === "*") return null;
  const n = Number(token);
  return Number.isFinite(n) ? n : null;
}

function parseIntOrNull(token: string | undefined): number | null {
  if (token == null || token === "" || token === "*") return null;
  const n = Number(token);
  return Number.isFinite(n) ? n : null;
}

function pad2(n: number): string {
  return String(n).padStart(2, "0");
}

/**
 * Internal time token for sorting / pattern detection.
 * UI must run these through humanizeScheduleTime — never show raw *:MM to players.
 */
export function formatScheduleTime(slot: Pick<PublicScheduleSlot, "hour" | "minute">): string {
  if (slot.hour == null && slot.minute == null) return "*:*";
  if (slot.hour == null) return `*:${pad2(slot.minute ?? 0)}`;
  if (slot.minute == null) return `${pad2(slot.hour)}:*`;
  return `${pad2(slot.hour)}:${pad2(slot.minute)}`;
}

function dowKey(dayOfWeek: ScheduleField): string | null {
  if (dayOfWeek == null) return null;
  if (dayOfWeek >= 1 && dayOfWeek <= 7) return DOW_KEYS[dayOfWeek];
  return null;
}

/**
 * Detect simple daily patterns. Returns machine keys for i18n, e.g. hourly:50, even2h:0.
 */
export function detectDailyPattern(times: string[]): string | null {
  const starMinute = times.find((t) => t.startsWith("*:") && /^\*:\d{2}$/.test(t));
  const concrete = times.filter((t) => /^\d{2}:\d{2}$/.test(t));

  // Mixed wildcard + fixed slots (e.g. TvT *:50 plus 14:05) — don't invent a false cadence.
  if (starMinute && concrete.length > 0) {
    return null;
  }
  if (starMinute && concrete.length === 0) {
    return `hourly:${Number(starMinute.slice(2))}`;
  }

  if (concrete.length < 2) return null;

  const parsed = concrete
    .map((t) => {
      const [h, m] = t.split(":").map(Number);
      return { h, m, t };
    })
    .sort((a, b) => a.h * 60 + a.m - (b.h * 60 + b.m));

  const minutes = new Set(parsed.map((p) => p.m));
  const hours = parsed.map((p) => p.h);

  if (minutes.size === 1) {
    const m = parsed[0].m;
    const even = hours.every((h) => h % 2 === 0);
    const odd = hours.every((h) => h % 2 === 1);
    const step2 =
      hours.length >= 3 &&
      hours.every((h, i) => i === 0 || h - hours[i - 1] === 2);
    if (even && step2 && hours[0] === 0 && hours[hours.length - 1] === 22) {
      return `even2h:${m}`;
    }
    if (odd && step2 && hours[0] === 1 && hours[hours.length - 1] === 23) {
      return `odd2h:${m}`;
    }
    if (
      hours.length >= 3 &&
      hours.every((h, i) => i === 0 || h - hours[i - 1] === 4)
    ) {
      return `every4h:${m}`;
    }
  }

  return null;
}

function uniqueSortedTimes(slots: PublicScheduleSlot[]): string[] {
  const set = new Set<string>();
  for (const s of slots) {
    set.add(formatScheduleTime(s));
  }
  return [...set].sort((a, b) => {
    const score = (t: string) => {
      if (t.startsWith("*:")) return 1000 + Number(t.slice(2) || 0);
      const [h, m] = t.split(":").map((x) => (x === "*" ? -1 : Number(x)));
      return (h < 0 ? 2000 : h) * 60 + (m < 0 ? 0 : m);
    };
    return score(a) - score(b);
  });
}

function parseStandardTiming(section0: string): PublicEventTiming | null {
  const line = memScriptDataLines(section0)[0];
  if (!line) return null;
  const t = tokenizeRow(line);
  if (t.length < 4) return null;
  return {
    warningMinutes: parseIntOrNull(t[0]),
    notifyMinutes: parseIntOrNull(t[1]),
    eventMinutes: parseIntOrNull(t[2]),
    closeMinutes: parseIntOrNull(t[3]),
  };
}

/** Year Month Day DoW Hour Minute Second */
function parseStandardSlots(section1: string): PublicScheduleSlot[] {
  const slots: PublicScheduleSlot[] = [];
  for (const line of memScriptDataLines(section1)) {
    const t = tokenizeRow(line);
    if (t.length < 7) continue;
    slots.push({
      year: parseField(t[0]),
      month: parseField(t[1]),
      day: parseField(t[2]),
      dayOfWeek: parseField(t[3]),
      hour: parseField(t[4]),
      minute: parseField(t[5]),
      second: parseField(t[6]),
    });
  }
  return slots;
}

function buildClassicFromText(
  id: string,
  source: string,
  text: string | null
): PublicClassicEventSchedule {
  if (text == null) {
    return {
      id,
      source,
      loaded: false,
      empty: true,
      timing: null,
      slots: [],
      times: [],
      pattern: null,
      note: null,
      durationMinutes: null,
    };
  }

  const sections = splitMemScriptSections(text);

  // CastleDeep: section 0 is the schedule (no Warning/Notify block); rows may be commented out.
  if (id === "castleDeep") {
    const slots = parseStandardSlots(sections.get(0) ?? "");
    const times = uniqueSortedTimes(slots);
    const empty = slots.length === 0;
    return {
      id,
      source,
      loaded: true,
      empty,
      timing: null,
      slots,
      times,
      pattern: empty ? null : detectDailyPattern(times),
      note: empty ? "scheduleEmpty" : null,
      durationMinutes: null,
    };
  }

  const timing = parseStandardTiming(sections.get(0) ?? "");
  const slots = parseStandardSlots(sections.get(1) ?? "");
  const times = uniqueSortedTimes(slots);
  const empty = slots.length === 0;

  return {
    id,
    source,
    loaded: true,
    empty,
    timing,
    slots,
    times,
    pattern: empty ? null : detectDailyPattern(times),
    note: null,
    durationMinutes: timing?.eventMinutes ?? null,
  };
}

/** TvT/GvG share BC-like layout (section 0 = Alarm/Stand/Event/Close). */
function buildTeamEventFromText(
  id: string,
  source: string,
  text: string | null
): PublicClassicEventSchedule {
  return buildClassicFromText(id, source, text);
}

export function parseCrywolfSchedule(text: string | null): PublicCrywolfSchedule {
  const source = "Event/Crywolf.dat";
  if (text == null) {
    return { loaded: false, empty: true, source, entries: [] };
  }
  const sections = splitMemScriptSections(text);
  const entries: PublicCrywolfSchedule["entries"] = [];
  for (const line of memScriptDataLines(sections.get(0) ?? "")) {
    const t = tokenizeRow(line);
    // Mode State Month Day DayOfWeek Hour Minute ContinuanceTime
    if (t.length < 8) continue;
    const hour = parseIntOrNull(t[5]);
    const minute = parseIntOrNull(t[6]);
    if (hour == null || minute == null) continue;
    const dayOfWeek = parseField(t[4]);
    const continuance = parseIntOrNull(t[7]);
    entries.push({
      dayOfWeek,
      hour,
      minute,
      continuanceMinutes: continuance,
      timeLabel: `${pad2(hour)}:${pad2(minute)}`,
      dayLabel: dowKey(dayOfWeek),
    });
  }
  return {
    loaded: true,
    empty: entries.length === 0,
    source,
    entries,
  };
}

export function parseInvasionSchedules(text: string | null): PublicInvasionBossSchedule[] {
  if (text == null) return [];
  const sections = splitMemScriptSections(text);

  const names = new Map<number, string>();
  for (const line of memScriptDataLines(sections.get(1) ?? "")) {
    const t = tokenizeRow(line);
    // Index BossIndex BossMessage InvasionTime AlarmTime "Event Name"
    if (t.length < 6) continue;
    const index = Number(t[0]);
    if (!Number.isFinite(index)) continue;
    const name = (t[5] || `Boss #${index}`).trim();
    names.set(index, name);
  }

  const byIndex = new Map<number, PublicScheduleSlot[]>();
  for (const line of memScriptDataLines(sections.get(0) ?? "")) {
    const t = tokenizeRow(line);
    // Index Year Month Day DoW Hour Minute Second
    if (t.length < 8) continue;
    const index = Number(t[0]);
    if (!Number.isFinite(index)) continue;
    const slot: PublicScheduleSlot = {
      year: parseField(t[1]),
      month: parseField(t[2]),
      day: parseField(t[3]),
      dayOfWeek: parseField(t[4]),
      hour: parseField(t[5]),
      minute: parseField(t[6]),
      second: parseField(t[7]),
    };
    const list = byIndex.get(index) ?? [];
    list.push(slot);
    byIndex.set(index, list);
  }

  const indices = [...new Set([...byIndex.keys(), ...names.keys()])].sort(
    (a, b) => a - b
  );

  return indices.map((index) => {
    const slots = byIndex.get(index) ?? [];
    const times = uniqueSortedTimes(slots);
    return {
      index,
      name: names.get(index) ?? `Boss #${index}`,
      times,
      pattern: detectDailyPattern(times),
      slots,
    };
  });
}

const CLASSIC_FILES: { id: string; file: string }[] = [
  { id: "bloodCastle", file: "Event/BloodCastle.dat" },
  { id: "devilSquare", file: "Event/DevilSquare.dat" },
  { id: "chaosCastle", file: "Event/ChaosCastle.dat" },
  { id: "illusionTemple", file: "Event/IllusionTemple.dat" },
  { id: "castleDeep", file: "Event/CastleDeepEvent.dat" },
  { id: "tvt", file: "Event/TvTEvent.dat" },
  { id: "gvg", file: "Event/GvGEvent.dat" },
];

export function buildPublicEventSchedules(input: {
  classicTexts: Record<string, string | null>;
  crywolfText: string | null;
  invasionText: string | null;
}): PublicEventSchedules {
  const classic = CLASSIC_FILES.map(({ id, file }) => {
    const text = input.classicTexts[id] ?? null;
    if (id === "tvt" || id === "gvg") {
      return buildTeamEventFromText(id, file, text);
    }
    return buildClassicFromText(id, file, text);
  });

  return {
    classic,
    crywolf: parseCrywolfSchedule(input.crywolfText),
    invasions: parseInvasionSchedules(input.invasionText),
  };
}

export { CLASSIC_FILES as EVENT_SCHEDULE_FILES };
