import {
  memScriptDataLines,
  tokenizeRow,
} from "@/lib/gs/public-guide-parse";
import type { GroupDropRates, GroupRatesSnapshot } from "@/lib/gs/groups";

export type PublicMapRow = {
  index: number;
  name: string;
  experienceRate: number | null;
  itemDropRate: number | null;
  helperEnable: boolean | null;
};

/** Guide farm DTO: 2 groups (Thường / VIP), not AL0–AL3. */
export type PublicFarmGuide = {
  normal: GroupDropRates;
  vip: GroupDropRates;
  /** Subs present on disk (for source note). */
  availableSubs: { label: string; group: "normal" | "vip" }[];
  missingSubs: { label: string; group: "normal" | "vip" }[];
  maps: PublicMapRow[];
};

export function emptyPublicFarmGuide(): PublicFarmGuide {
  const empty = (): GroupDropRates => ({
    itemDropRate: null,
    moneyDropRate: null,
    jewelDropRate: null,
  });
  return {
    normal: empty(),
    vip: empty(),
    availableSubs: [],
    missingSubs: [],
    maps: [],
  };
}

/** Prefer open-world / farm maps; skip numbered event instance maps for MVP table. */
const EVENT_MAP_NAME_RE =
  /^(Blood Castle|Devil Square|Chaos Castle|Illusion Temple|Kalima|Duel Arena)\b/i;

export function parseMapManagerRows(mapManagerText: string): PublicMapRow[] {
  const maps: PublicMapRow[] = [];

  for (const line of memScriptDataLines(mapManagerText)) {
    if (line.startsWith("//")) continue;
    const t = tokenizeRow(line);
    // Index NonPK ViewRange ExperienceRate ItemDropRate Exc… Set… Socket Helper … Comment
    if (t.length < 5) continue;
    const index = Number(t[0]);
    if (!Number.isFinite(index)) continue;
    const experienceRate = Number(t[3]);
    const itemDropRate = Number(t[4]);
    const helperEnableRaw = t[8];
    const name = t[t.length - 1] || `Map ${index}`;
    if (EVENT_MAP_NAME_RE.test(name)) continue;

    maps.push({
      index,
      name,
      experienceRate: Number.isFinite(experienceRate) ? experienceRate : null,
      itemDropRate: Number.isFinite(itemDropRate) ? itemDropRate : null,
      helperEnable:
        helperEnableRaw === "*" || helperEnableRaw == null
          ? null
          : helperEnableRaw !== "0",
    });
  }

  maps.sort((a, b) => a.index - b.index);
  return maps;
}

export function buildPublicFarmGuideFromGroups(
  snapshot: GroupRatesSnapshot,
  mapManagerText: string
): PublicFarmGuide {
  return {
    normal: snapshot.normal,
    vip: snapshot.vip,
    availableSubs: snapshot.available.map((s) => ({
      label: s.label,
      group: s.group,
    })),
    missingSubs: snapshot.missing.map((s) => ({
      label: s.label,
      group: s.group,
    })),
    maps: parseMapManagerRows(mapManagerText),
  };
}
