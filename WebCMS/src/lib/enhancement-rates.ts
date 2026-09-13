import { readFile, writeFile } from "node:fs/promises";
import path from "node:path";
import { getIniEntryLabel } from "@/lib/gs/ini-labels";
import { getGsIniRoot } from "@/lib/gs/paths";

export type RateSource = "chaos" | "common";

export type EnhancementRate = {
  source: RateSource;
  key: string;
  label: string;
  rates: [number, number, number, number];
};

export type EnhancementRateSection = {
  id: RateSource;
  label: string;
  description: string;
  entries: EnhancementRate[];
};

export type EnhancementRateUpdate = {
  source: RateSource;
  key: string;
  rates: unknown;
};

const rateFileName: Record<RateSource, string> = {
  chaos: "GameServerInfo - ChaosMix.ini",
  common: "GameServerInfo - Common.ini",
};

function rateFile(source: RateSource): string {
  return path.join(getGsIniRoot(), rateFileName[source]);
}

const sourceInfo: Record<RateSource, Omit<EnhancementRateSection, "entries">> = {
  chaos: {
    id: "chaos",
    label: "Chaos Mix",
    description: "Toàn bộ tỉ lệ ép Chaos: +10 đến +15, cánh, sói, vé sự kiện, socket và các công thức khác.",
  },
  common: {
    id: "common",
    label: "Ngọc & Luck",
    description: "Tỉ lệ đập Soul, Life, Harmony, đá tinh luyện, Luck và trái cây.",
  },
};

function cleanComment(line: string) {
  const value = line
    .replace(/^\s*;+\s*/, "")
    .replace(/[=\-_*]+/g, " ")
    .trim();
  if (!value || /^(facebook|zalo|sđt|name\s*:|phiên bản|hỗ trợ)/i.test(value)) return "";
  return value;
}

function friendlyKey(key: string) {
  return key
    .replace(/([a-z])([A-Z])/g, "$1 $2")
    .replace(/(Rate)(\d+)/g, "$1 $2");
}

function isRateKey(source: RateSource, key: string) {
  if (source === "chaos") return /MixRate(?:\d+)?$/.test(key);
  return /SuccessRate\d*$/.test(key);
}

function parseRates(source: RateSource, content: string): EnhancementRate[] {
  const found = new Map<string, { label: string; rates: Array<number | undefined> }>();
  let latestComment = "";

  for (const line of content.split(/\r?\n/)) {
    if (/^\s*;/.test(line)) {
      const comment = cleanComment(line);
      if (comment) latestComment = comment;
      continue;
    }

    const match = line.match(/^\s*([A-Za-z][A-Za-z0-9]*)_AL([0-3])\s*=\s*(-?\d+)/);
    if (!match || !isRateKey(source, match[1])) continue;

    const [, key, levelText, valueText] = match;
    const level = Number(levelText);
    const existing = found.get(key) ?? {
      label: getIniEntryLabel(key, latestComment || friendlyKey(key)),
      rates: [undefined, undefined, undefined, undefined],
    };
    existing.rates[level] = Number(valueText);
    found.set(key, existing);
  }

  return [...found.entries()]
    .filter(([, value]) => value.rates.every((rate) => rate !== undefined))
    .map(([key, value]) => ({
      source,
      key,
      label: value.label,
      rates: value.rates as [number, number, number, number],
    }));
}

export async function getEnhancementRates(): Promise<EnhancementRateSection[]> {
  const [chaos, common] = await Promise.all([
    readFile(rateFile("chaos"), "utf8"),
    readFile(rateFile("common"), "utf8"),
  ]);

  return (["chaos", "common"] as const).map((source) => ({
    ...sourceInfo[source],
    entries: parseRates(source, source === "chaos" ? chaos : common),
  }));
}

function validatedRates(value: unknown): [number, number, number, number] | null {
  if (!Array.isArray(value) || value.length !== 4) return null;
  const rates = value.map(Number);
  if (rates.some((rate) => !Number.isInteger(rate) || rate < -1 || rate > 100)) return null;
  return rates as [number, number, number, number];
}

export async function saveEnhancementRates(updates: EnhancementRateUpdate[]) {
  if (!Array.isArray(updates) || updates.length === 0) {
    throw new Error("Không có tỉ lệ để lưu");
  }

  const current = await getEnhancementRates();
  const allowed = new Set(
    current.flatMap((section) => section.entries.map((entry) => `${entry.source}:${entry.key}`))
  );
  const changes = new Map<RateSource, Map<string, [number, number, number, number]>>([
    ["chaos", new Map()],
    ["common", new Map()],
  ]);

  for (const update of updates) {
    if (update.source !== "chaos" && update.source !== "common") {
      throw new Error("Nguồn cấu hình không hợp lệ");
    }
    if (typeof update.key !== "string" || !allowed.has(`${update.source}:${update.key}`)) {
      throw new Error("Mục tỉ lệ không hợp lệ");
    }
    const rates = validatedRates(update.rates);
    if (!rates) throw new Error("Tỉ lệ phải là số nguyên từ -1 đến 100");
    changes.get(update.source)!.set(update.key, rates);
  }

  await Promise.all(
    (["chaos", "common"] as const).map(async (source) => {
      const sourceChanges = changes.get(source)!;
      if (sourceChanges.size === 0) return;

      const content = await readFile(rateFile(source), "utf8");
      const lineEnding = content.includes("\r\n") ? "\r\n" : "\n";
      const next = content
        .split(/\r?\n/)
        .map((line) => {
          const match = line.match(/^(\s*)([A-Za-z][A-Za-z0-9]*)_AL([0-3])(\s*=\s*)(-?\d+)(.*)$/);
          if (!match) return line;
          const [, indent, key, levelText, equals, , suffix] = match;
          const rates = sourceChanges.get(key);
          if (!rates) return line;
          return `${indent}${key}_AL${levelText}${equals}${rates[Number(levelText)]}${suffix}`;
        })
        .join(lineEnding);

      await writeFile(rateFile(source), next, "utf8");
    })
  );

  return getEnhancementRates();
}
