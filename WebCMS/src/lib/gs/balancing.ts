export const BALANCE_CLASS_CODES = [
  "DW",
  "DK",
  "FE",
  "MG",
  "DL",
  "SU",
  "RF",
] as const;

export type BalanceClassCode = (typeof BALANCE_CLASS_CODES)[number];

export const BALANCE_CLASS_META: Record<
  BalanceClassCode,
  { short: string; full: string }
> = {
  DW: { short: "DW", full: "Dark Wizard" },
  DK: { short: "DK", full: "Dark Knight" },
  FE: { short: "FE", full: "Fairy Elf" },
  MG: { short: "MG", full: "Magic Gladiator" },
  DL: { short: "DL", full: "Dark Lord" },
  SU: { short: "SU", full: "Summoner" },
  RF: { short: "RF", full: "Rage Fighter" },
};

export type BalancingMatrix = Record<
  BalanceClassCode,
  Record<BalanceClassCode, number | null>
>;

export type BalancingData = {
  matrix: BalancingMatrix;
  pvp: Record<BalanceClassCode, number | null>;
  pvm: Record<BalanceClassCode, number | null>;
  globals: {
    generalPvP: number | null;
    generalPvM: number | null;
    duel: number | null;
    gens: number | null;
    chaosCastle: number | null;
  };
  missingKeys: string[];
};

/** Split INI value into numeric/text core and trailing // or ; comment (with leading whitespace). */
export function splitIniValue(value: string): { core: string; suffix: string } {
  const match = value.match(/^(.*?)(\s*(?:\/\/|;).*)$/);
  if (match) {
    return { core: match[1].trimEnd(), suffix: match[2] };
  }
  return { core: value.trim(), suffix: "" };
}

export function parseIniNumber(raw: string | undefined | null): number | null {
  if (raw == null) return null;
  const { core } = splitIniValue(String(raw));
  if (!core) return null;
  const n = Number.parseInt(core, 10);
  return Number.isFinite(n) ? n : null;
}

function emptyMatrix(): BalancingMatrix {
  const matrix = {} as BalancingMatrix;
  for (const atk of BALANCE_CLASS_CODES) {
    matrix[atk] = {} as Record<BalanceClassCode, number | null>;
    for (const def of BALANCE_CLASS_CODES) {
      matrix[atk][def] = null;
    }
  }
  return matrix;
}

function emptyClassMap(): Record<BalanceClassCode, number | null> {
  const out = {} as Record<BalanceClassCode, number | null>;
  for (const code of BALANCE_CLASS_CODES) out[code] = null;
  return out;
}

export function matrixKey(atk: BalanceClassCode, def: BalanceClassCode): string {
  return `${atk}DamageRateTo${def}`;
}

export function pvpKey(cls: BalanceClassCode): string {
  return `${cls}DamageRatePvP`;
}

export function pvmKey(cls: BalanceClassCode): string {
  return `${cls}DamageRatePvM`;
}

export function allBalancingKeys(): string[] {
  const keys: string[] = [];
  for (const atk of BALANCE_CLASS_CODES) {
    keys.push(pvpKey(atk), pvmKey(atk));
    for (const def of BALANCE_CLASS_CODES) {
      keys.push(matrixKey(atk, def));
    }
  }
  return keys;
}

const BALANCING_KEY_SET = new Set(allBalancingKeys());

export function isBalancingKey(key: string): boolean {
  return BALANCING_KEY_SET.has(key);
}

/**
 * Read class balancing from a GameServerInfo - Character.ini document values map
 * (or any key→rawValue map). Missing keys become null and are listed in missingKeys.
 */
export function readBalancingFromValues(
  values: Record<string, string>
): BalancingData {
  const matrix = emptyMatrix();
  const pvp = emptyClassMap();
  const pvm = emptyClassMap();
  const missingKeys: string[] = [];

  for (const atk of BALANCE_CLASS_CODES) {
    const pk = pvpKey(atk);
    const mk = pvmKey(atk);
    if (!(pk in values)) missingKeys.push(pk);
    if (!(mk in values)) missingKeys.push(mk);
    pvp[atk] = parseIniNumber(values[pk]);
    pvm[atk] = parseIniNumber(values[mk]);

    for (const def of BALANCE_CLASS_CODES) {
      const key = matrixKey(atk, def);
      if (!(key in values)) missingKeys.push(key);
      matrix[atk][def] = parseIniNumber(values[key]);
    }
  }

  return {
    matrix,
    pvp,
    pvm,
    globals: {
      generalPvP: parseIniNumber(values.GeneralDamageRatePvP),
      generalPvM: parseIniNumber(values.GeneralDamageRatePvM),
      duel: parseIniNumber(values.DuelDamageRate),
      gens: parseIniNumber(values.GensDamageRate),
      chaosCastle: parseIniNumber(values.ChaosCastleDamageRate),
    },
    missingKeys,
  };
}

export function readBalancingFromIniText(text: string): BalancingData {
  // Lazy import avoided — parse via simple line scan so this module stays free of cycles.
  const values: Record<string, string> = {};
  for (const raw of text.replace(/^\uFEFF/, "").split(/\r?\n/)) {
    const trimmed = raw.trim();
    if (!trimmed || trimmed.startsWith(";") || trimmed.startsWith("#")) continue;
    if (/^\[.+]$/.test(trimmed)) continue;
    const eq = raw.indexOf("=");
    if (eq === -1) continue;
    const key = raw.slice(0, eq).trim();
    const value = raw.slice(eq + 1).trim();
    if (key && !(key in values)) values[key] = value;
  }
  return readBalancingFromValues(values);
}

export type BalancingUpdateInput = {
  matrix?: Partial<
    Record<BalanceClassCode, Partial<Record<BalanceClassCode, number>>>
  >;
  pvp?: Partial<Record<BalanceClassCode, number>>;
  pvm?: Partial<Record<BalanceClassCode, number>>;
};

const MIN_RATE = 0;
const MAX_RATE = 500;

export function clampBalanceRate(n: number): number {
  if (!Number.isFinite(n)) return 0;
  return Math.max(MIN_RATE, Math.min(MAX_RATE, Math.round(n)));
}

/** Build whitelist-only INI key updates from structured balancing input. */
export function buildBalancingUpdates(
  input: BalancingUpdateInput
): Record<string, string> {
  const updates: Record<string, string> = {};

  if (input.matrix) {
    for (const atk of BALANCE_CLASS_CODES) {
      const row = input.matrix[atk];
      if (!row) continue;
      for (const def of BALANCE_CLASS_CODES) {
        const v = row[def];
        if (v == null || !Number.isFinite(v)) continue;
        updates[matrixKey(atk, def)] = String(clampBalanceRate(v));
      }
    }
  }

  if (input.pvp) {
    for (const cls of BALANCE_CLASS_CODES) {
      const v = input.pvp[cls];
      if (v == null || !Number.isFinite(v)) continue;
      updates[pvpKey(cls)] = String(clampBalanceRate(v));
    }
  }

  if (input.pvm) {
    for (const cls of BALANCE_CLASS_CODES) {
      const v = input.pvm[cls];
      if (v == null || !Number.isFinite(v)) continue;
      updates[pvmKey(cls)] = String(clampBalanceRate(v));
    }
  }

  return updates;
}

/** Filter an arbitrary updates object down to known balancing keys with numeric values. */
export function sanitizeBalancingKeyUpdates(
  updates: Record<string, string>
): Record<string, string> {
  const out: Record<string, string> = {};
  for (const [key, raw] of Object.entries(updates)) {
    if (!isBalancingKey(key)) continue;
    const n = parseIniNumber(raw);
    if (n == null) continue;
    out[key] = String(clampBalanceRate(n));
  }
  return out;
}
