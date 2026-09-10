import {
  iniInt,
  memScriptDataLines,
  parseIniValues,
  tokenizeRow,
} from "@/lib/gs/public-guide-parse";

export type ClassCode = "DW" | "DK" | "FE" | "MG" | "DL" | "SU" | "RF";

export const CLASS_CODES: ClassCode[] = [
  "DW",
  "DK",
  "FE",
  "MG",
  "DL",
  "SU",
  "RF",
];

export type PublicClassBase = {
  code: ClassCode;
  strength: number | null;
  dexterity: number | null;
  vitality: number | null;
  energy: number | null;
  leadership: number | null;
  maxLife: number | null;
  maxMana: number | null;
};

export type PublicSkillTip = {
  id: string;
  classCodes: ClassCode[];
  /** Formula / cap numbers from Skill.ini */
  values: Record<string, number | null>;
};

export type PublicSkillsGuide = {
  bases: PublicClassBase[];
  tips: PublicSkillTip[];
};

const CLASS_BY_INDEX: ClassCode[] = ["DW", "DK", "FE", "MG", "DL", "SU", "RF"];

export function emptyPublicSkillsGuide(): PublicSkillsGuide {
  return { bases: [], tips: [] };
}

function parseDefaultClassInfo(text: string): PublicClassBase[] {
  const bases: PublicClassBase[] = [];
  for (const line of memScriptDataLines(text)) {
    if (line.startsWith("//")) continue;
    const t = tokenizeRow(line);
    if (t.length < 8) continue;
    const idx = Number(t[0]);
    if (!Number.isFinite(idx) || idx < 0 || idx > 6) continue;
    bases.push({
      code: CLASS_BY_INDEX[idx],
      strength: Number.isFinite(Number(t[1])) ? Number(t[1]) : null,
      dexterity: Number.isFinite(Number(t[2])) ? Number(t[2]) : null,
      vitality: Number.isFinite(Number(t[3])) ? Number(t[3]) : null,
      energy: Number.isFinite(Number(t[4])) ? Number(t[4]) : null,
      leadership: Number.isFinite(Number(t[5])) ? Number(t[5]) : null,
      maxLife: Number.isFinite(Number(t[6])) ? Number(t[6]) : null,
      maxMana: Number.isFinite(Number(t[7])) ? Number(t[7]) : null,
    });
  }
  return bases;
}

function rateMap(
  values: Record<string, string>,
  prefix: string
): Record<string, number | null> {
  const out: Record<string, number | null> = {};
  for (const code of CLASS_CODES) {
    out[`rate${code}`] = iniInt(values[`${prefix}Rate${code}`]);
  }
  return out;
}

export function buildPublicSkillsGuide(input: {
  skillIniText: string;
  defaultClassText: string;
}): PublicSkillsGuide {
  const skill = parseIniValues(input.skillIniText);
  const bases = parseDefaultClassInfo(input.defaultClassText);

  const tips: PublicSkillTip[] = [
    {
      id: "manaShield",
      classCodes: ["DW"],
      values: {
        constA: iniInt(skill.ManaShieldConstA),
        constB: iniInt(skill.ManaShieldConstB),
        constC: iniInt(skill.ManaShieldConstC),
        maxRate: iniInt(skill.ManaShieldMaxRate),
        timeA: iniInt(skill.ManaShieldTimeConstA),
        timeB: iniInt(skill.ManaShieldTimeConstB),
        ...rateMap(skill, "ManaShield"),
      },
    },
    {
      id: "greaterDefense",
      classCodes: ["FE"],
      values: {
        constA: iniInt(skill.GreaterDefenseConstA),
        constB: iniInt(skill.GreaterDefenseConstB),
        timeA: iniInt(skill.GreaterDefenseTimeConstA),
        ...rateMap(skill, "GreaterDefense"),
      },
    },
    {
      id: "greaterDamage",
      classCodes: ["FE"],
      values: {
        constA: iniInt(skill.GreaterDamageConstA),
        constB: iniInt(skill.GreaterDamageConstB),
        timeA: iniInt(skill.GreaterDamageTimeConstA),
        ...rateMap(skill, "GreaterDamage"),
      },
    },
    {
      id: "heal",
      classCodes: ["FE"],
      values: {
        constA: iniInt(skill.HealConstA),
        constB: iniInt(skill.HealConstB),
      },
    },
    {
      id: "greaterLife",
      classCodes: ["DK"],
      values: {
        constA: iniInt(skill.GreaterLifeConstA),
        constB: iniInt(skill.GreaterLifeConstB),
        constC: iniInt(skill.GreaterLifeConstC),
        maxRate: iniInt(skill.GreaterLifeMaxRate),
        ...rateMap(skill, "GreaterLife"),
      },
    },
    {
      id: "reflectDamage",
      classCodes: ["SU"],
      values: {
        constA: iniInt(skill.ReflectDamageConstA),
        constB: iniInt(skill.ReflectDamageConstB),
        maxRate: iniInt(skill.ReflectDamageMaxRate),
        ...rateMap(skill, "ReflectDamage"),
      },
    },
    {
      id: "infinityArrow",
      classCodes: ["FE"],
      values: {
        timeA: iniInt(skill.InfinityArrowTimeConstA),
      },
    },
    {
      id: "greaterCritical",
      classCodes: ["DL"],
      values: {
        constA: iniInt(skill.GreaterCriticalDamageConstA),
        constB: iniInt(skill.GreaterCriticalDamageConstB),
        timeA: iniInt(skill.GreaterCriticalDamageTimeConstA),
        timeB: iniInt(skill.GreaterCriticalDamageTimeConstB),
      },
    },
  ];

  return { bases, tips };
}
