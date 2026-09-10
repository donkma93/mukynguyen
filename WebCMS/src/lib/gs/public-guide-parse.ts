import { parseIni } from "@/lib/gs/ini";

/** Strip trailing // or ; comments from an INI value. */
export function iniCoreValue(raw: string | undefined): string {
  if (!raw) return "";
  const match = raw.match(/^(.*?)(\s*(?:\/\/|;).*)$/);
  return (match ? match[1] : raw).trim();
}

export function iniInt(raw: string | undefined, fallback: number | null = null): number | null {
  const core = iniCoreValue(raw);
  if (!core) return fallback;
  const n = Number(core);
  return Number.isFinite(n) ? n : fallback;
}

export function iniBool01(raw: string | undefined): boolean | null {
  const n = iniInt(raw);
  if (n == null) return null;
  return n !== 0;
}

export type AccountLevel = 0 | 1 | 2 | 3;

export const ACCOUNT_LEVELS: AccountLevel[] = [0, 1, 2, 3];

export function ratesByAccountLevel(
  values: Record<string, string>,
  keyPrefix: string
): Record<AccountLevel, number | null> {
  return {
    0: iniInt(values[`${keyPrefix}_AL0`]),
    1: iniInt(values[`${keyPrefix}_AL1`]),
    2: iniInt(values[`${keyPrefix}_AL2`]),
    3: iniInt(values[`${keyPrefix}_AL3`]),
  };
}

export function parseIniValues(text: string): Record<string, string> {
  return parseIni(text).values;
}

/** Split MemScript-style lines: skip blank/comment, stop at `end`. */
export function memScriptDataLines(sectionText: string): string[] {
  const out: string[] = [];
  for (const raw of sectionText.split(/\r?\n/)) {
    const trimmed = raw.trim();
    if (!trimmed) continue;
    if (trimmed.startsWith("//") || trimmed.startsWith(";")) continue;
    if (/^end$/i.test(trimmed)) break;
    out.push(trimmed);
  }
  return out;
}

/**
 * Tokenize a MemScript data row, respecting double-quoted strings.
 */
export function tokenizeRow(line: string): string[] {
  const tokens: string[] = [];
  let i = 0;
  while (i < line.length) {
    while (i < line.length && /\s/.test(line[i])) i++;
    if (i >= line.length) break;
    if (line[i] === '"') {
      i++;
      let s = "";
      while (i < line.length && line[i] !== '"') {
        s += line[i];
        i++;
      }
      if (i < line.length && line[i] === '"') i++;
      tokens.push(s);
      continue;
    }
    let s = "";
    while (i < line.length && !/\s/.test(line[i])) {
      s += line[i];
      i++;
    }
    // Drop trailing inline //comment glued without space (rare)
    if (s.startsWith("//")) break;
    tokens.push(s);
  }
  return tokens;
}

/** Extract numbered MemScript sections: `0` … `end`, `1` … `end`, … */
export function splitMemScriptSections(text: string): Map<number, string> {
  const cleaned = text.replace(/^\uFEFF/, "");
  const map = new Map<number, string>();
  const re = /^(\d+)\s*$/gm;
  const starts: { index: number; id: number }[] = [];
  let m: RegExpExecArray | null;
  while ((m = re.exec(cleaned))) {
    starts.push({ index: m.index + m[0].length, id: Number(m[1]) });
  }
  for (let i = 0; i < starts.length; i++) {
    const start = starts[i].index;
    const endMarker = cleaned.slice(start).search(/^end\s*$/im);
    const body =
      endMarker >= 0
        ? cleaned.slice(start, start + endMarker)
        : cleaned.slice(start, starts[i + 1]?.index);
    map.set(starts[i].id, body);
  }
  return map;
}
