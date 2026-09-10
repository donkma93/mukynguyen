/**
 * Player-facing helpers: turn internal schedule / VIP tokens into plain language.
 * Machine tokens stay in API data; UI always runs them through these helpers.
 */

function pad2(n: number): string {
  return String(n).padStart(2, "0");
}

export type ScheduleCopy = {
  anyMinute: string;
  everyHourAt: (minute: number) => string;
  everyHourAnyMinute: string;
  hourAnyMinute: (hour: number) => string;
  clock: (hour: number, minute: number) => string;
  patternHourly: (minute: number) => string;
  patternEven2h: (minute: number) => string;
  patternOdd2h: (minute: number) => string;
  patternEvery4h: (minute: number) => string;
  days: Record<string, string>;
};

export type VipCopy = {
  /** Short column header, e.g. "Thường", "VIP 1" */
  short: (level: 0 | 1 | 2 | 3) => string;
  /** Inline rate list when tiers differ */
  ratePart: (level: 0 | 1 | 2 | 3, value: string) => string;
  /** Enabled/disabled split across VIP tiers */
  statusPart: (level: 0 | 1 | 2 | 3, status: string) => string;
  join: string;
};

/** Turn internal time token (*:50, 14:00, 14:*) into readable clock text. */
export function humanizeScheduleTime(token: string, copy: ScheduleCopy): string {
  if (token === "*:*") return copy.anyMinute;
  const starMin = token.match(/^\*:(\d{2})$/);
  if (starMin) {
    const m = Number(starMin[1]);
    return copy.everyHourAt(m);
  }
  const hourStar = token.match(/^(\d{2}):\*$/);
  if (hourStar) {
    return copy.hourAnyMinute(Number(hourStar[1]));
  }
  const hm = token.match(/^(\d{2}):(\d{2})$/);
  if (hm) {
    return copy.clock(Number(hm[1]), Number(hm[2]));
  }
  return token;
}

/** Turn pattern key (hourly:50, even2h:0, …) into a full sentence. */
export function humanizeSchedulePattern(
  pattern: string | null | undefined,
  copy: ScheduleCopy
): string | null {
  if (!pattern) return null;
  const hourly = pattern.match(/^hourly:(\d+)$/);
  if (hourly) return copy.patternHourly(Number(hourly[1]));
  const even = pattern.match(/^even2h:(\d+)$/);
  if (even) return copy.patternEven2h(Number(even[1]));
  const odd = pattern.match(/^odd2h:(\d+)$/);
  if (odd) return copy.patternOdd2h(Number(odd[1]));
  const every4 = pattern.match(/^every4h:(\d+)$/);
  if (every4) return copy.patternEvery4h(Number(every4[1]));
  // Legacy English strings from older API payloads
  if (pattern.startsWith("every hour @")) {
    const m = pattern.match(/:(\d{2})/);
    return copy.patternHourly(m ? Number(m[1]) : 0);
  }
  if (pattern.includes("(even)")) {
    const m = pattern.match(/:(\d{2})/);
    return copy.patternEven2h(m ? Number(m[1]) : 0);
  }
  if (pattern.includes("(odd)")) {
    const m = pattern.match(/:(\d{2})/);
    return copy.patternOdd2h(m ? Number(m[1]) : 0);
  }
  if (pattern.startsWith("every 4h")) {
    const m = pattern.match(/:(\d{2})/);
    return copy.patternEvery4h(m ? Number(m[1]) : 0);
  }
  return pattern;
}

export function humanizeDayKey(
  key: string | null | undefined,
  copy: ScheduleCopy
): string | null {
  if (!key) return null;
  return copy.days[key] ?? key;
}

export function formatClock(hour: number, minute: number): string {
  return `${pad2(hour)}:${pad2(minute)}`;
}

/** Build ScheduleCopy from dictionary events.scheduleSpeak + events.days. */
export function scheduleCopyFromDict(events: {
  scheduleSpeak: {
    anyMinute: string;
    everyHourAt: string;
    everyHourAnyMinute: string;
    hourAnyMinute: string;
    clock: string;
    patternHourly: string;
    patternEven2h: string;
    patternOdd2h: string;
    patternEvery4h: string;
  };
  days: Record<string, string>;
}): ScheduleCopy {
  const s = events.scheduleSpeak;
  const fill = (tpl: string, vars: Record<string, string | number>) =>
    Object.entries(vars).reduce(
      (out, [k, v]) => out.replaceAll(`{${k}}`, String(v)),
      tpl
    );
  return {
    anyMinute: s.anyMinute,
    everyHourAnyMinute: s.everyHourAnyMinute,
    everyHourAt: (minute) =>
      fill(s.everyHourAt, { minute: pad2(minute), m: pad2(minute) }),
    hourAnyMinute: (hour) =>
      fill(s.hourAnyMinute, { hour: pad2(hour), h: pad2(hour) }),
    clock: (hour, minute) =>
      fill(s.clock, {
        hour: pad2(hour),
        minute: pad2(minute),
        time: formatClock(hour, minute),
      }),
    patternHourly: (minute) =>
      fill(s.patternHourly, { minute: pad2(minute), m: pad2(minute) }),
    patternEven2h: (minute) =>
      fill(s.patternEven2h, { minute: pad2(minute), m: pad2(minute) }),
    patternOdd2h: (minute) =>
      fill(s.patternOdd2h, { minute: pad2(minute), m: pad2(minute) }),
    patternEvery4h: (minute) =>
      fill(s.patternEvery4h, { minute: pad2(minute), m: pad2(minute) }),
    days: events.days,
  };
}
