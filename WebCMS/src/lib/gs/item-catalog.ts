import fs from "fs";
import path from "path";
import { getGsDataRoot } from "@/lib/gs/paths";
import { getMaxSocketForItem } from "@/lib/gs/socket-item-type";

export type ItemCatalogEntry = {
  index: number;
  section: number;
  type: number;
  slot: number;
  skill: number;
  width: number;
  height: number;
  name: string;
  /** Base durability from Item.txt (weapons already include MagicDur). */
  baseDurability: number;
  /** From SocketItemType.txt; 0 = item không hỗ trợ socket. */
  maxSocket: number;
};

let cache: Map<number, ItemCatalogEntry> | null = null;
let cacheMtime = 0;

function tokenizeLine(line: string): string[] {
  const tokens: string[] = [];
  let i = 0;
  while (i < line.length) {
    while (i < line.length && /\s/.test(line[i])) i++;
    if (i >= line.length) break;
    if (line[i] === '"') {
      i++;
      let s = "";
      while (i < line.length && line[i] !== '"') {
        s += line[i++];
      }
      if (i < line.length && line[i] === '"') i++;
      tokens.push(s);
      continue;
    }
    let s = "";
    while (i < line.length && !/\s/.test(line[i])) s += line[i++];
    tokens.push(s);
  }
  return tokens;
}

function itemTxtPath(): string {
  const fromEnv = process.env.GS_ITEM_TXT?.trim();
  if (fromEnv) return path.resolve(fromEnv);
  return path.join(getGsDataRoot(), "Item", "Item.txt");
}

function parseItemTxt(text: string): Map<number, ItemCatalogEntry> {
  const map = new Map<number, ItemCatalogEntry>();
  let section: number | null = null;
  const lines = text.replace(/^\uFEFF/, "").split(/\r?\n/);

  for (const raw of lines) {
    const line = raw.trim();
    if (!line || line.startsWith("//") || line.startsWith(";")) continue;
    if (/^end$/i.test(line)) {
      section = null;
      continue;
    }

    const tokens = tokenizeLine(line);
    if (!tokens.length) continue;

    // Section header: a single integer line (0..15)
    if (tokens.length === 1 && /^-?\d+$/.test(tokens[0])) {
      const n = Number(tokens[0]);
      if (n >= 0 && n <= 15) section = n;
      continue;
    }

    if (section === null) continue;
    if (tokens.length < 9) continue;

    const type = Number(tokens[0]);
    const slot = Number(tokens[1]);
    const skill = Number(tokens[2]);
    const width = Number(tokens[3]);
    const height = Number(tokens[4]);
    if (![type, slot, skill, width, height].every((n) => Number.isFinite(n))) {
      continue;
    }

    // Name is the first non-numeric-looking token after drop flag, or quoted already captured
    let nameIdx = 8;
    // tokens: type slot skill x y serial option drop name ...
    const name = tokens[nameIdx] ?? `Item ${section}:${type}`;

    let baseDurability = 1;
    if (section >= 0 && section <= 5) {
      // name, level, dmin, dmax, attspeed, dur, magicDur, ...
      const dur = Number(tokens[nameIdx + 5] ?? 0);
      const magicDur = Number(tokens[nameIdx + 6] ?? 0);
      baseDurability = (Number.isFinite(dur) ? dur : 0) + (Number.isFinite(magicDur) ? magicDur : 0);
    } else if (section >= 6 && section <= 11) {
      // name, level, def[, extra], durability, ...
      // section 6: level, defense, defenseSuccess, durability
      // 7-9: level, defense, magicDefense, durability
      // 10: level, defense, attackSpeed, durability
      // 11: level, defense, walkSpeed, durability
      const dur = Number(tokens[nameIdx + 4] ?? 1);
      baseDurability = Number.isFinite(dur) ? dur : 1;
    } else if (section === 12) {
      const dur = Number(tokens[nameIdx + 3] ?? 1);
      baseDurability = Number.isFinite(dur) ? dur : 1;
    } else if (section === 13) {
      const dur = Number(tokens[nameIdx + 2] ?? 1);
      baseDurability = Number.isFinite(dur) ? dur : 1;
    } else if (section === 14) {
      baseDurability = 1;
    } else if (section === 15) {
      baseDurability = 1;
    }

    const index = section * 512 + type;
    map.set(index, {
      index,
      section,
      type,
      slot,
      skill,
      width: Math.max(1, width),
      height: Math.max(1, height),
      name,
      baseDurability: Math.max(0, Math.min(255, baseDurability || 1)),
      maxSocket: 0,
    });
  }

  // Attach socket max from SocketItemType.txt
  for (const [index, entry] of map) {
    entry.maxSocket = getMaxSocketForItem(index);
  }

  return map;
}

export function loadItemCatalog(force = false): Map<number, ItemCatalogEntry> {
  const file = itemTxtPath();
  const st = fs.statSync(file);
  if (!force && cache && st.mtimeMs === cacheMtime) return cache;
  const text = fs.readFileSync(file, "utf8");
  cache = parseItemTxt(text);
  cacheMtime = st.mtimeMs;
  return cache;
}

export function getItemInfo(index: number): ItemCatalogEntry | null {
  return loadItemCatalog().get(index) ?? null;
}

export function searchItems(query: string, limit = 40): ItemCatalogEntry[] {
  const q = query.trim().toLowerCase();
  if (!q) return [];
  const all = [...loadItemCatalog().values()];
  const asNum = Number(q);
  const out: ItemCatalogEntry[] = [];
  for (const it of all) {
    if (
      it.name.toLowerCase().includes(q) ||
      String(it.index) === q ||
      (Number.isFinite(asNum) && it.index === asNum) ||
      `${it.section}:${it.type}` === q ||
      `${it.section} ${it.type}` === q
    ) {
      out.push(it);
      if (out.length >= limit) break;
    }
  }
  return out;
}

export function getItemIndex(section: number, type: number): number {
  return section * 512 + type;
}

/** Port of CItemManager::GetItemDurability (simplified common cases). */
export function calcItemDurability(
  info: ItemCatalogEntry,
  level: number,
  excellent: number,
  setOption: number
): number {
  const index = info.index;
  // Special consumables / event items → 1
  const forceOne = new Set([
    getItemIndex(14, 21),
    getItemIndex(14, 29),
    getItemIndex(14, 100),
    getItemIndex(14, 215),
    getItemIndex(13, 18),
    getItemIndex(13, 29),
    getItemIndex(13, 51),
    getItemIndex(14, 19),
    getItemIndex(14, 102),
    getItemIndex(14, 109),
    getItemIndex(14, 110),
  ]);
  if (forceOne.has(index) || info.section === 14) return 1;

  let dur = 0;
  if (level >= 5) {
    if (level === 10) dur = info.baseDurability + (level * 2 - 3);
    else if (level === 11) dur = info.baseDurability + (level * 2 - 1);
    else if (level === 12) dur = info.baseDurability + (level * 2 + 2);
    else if (level === 13) dur = info.baseDurability + (level * 2 + 6);
    else if (level === 14) dur = info.baseDurability + (level * 2 + 11);
    else if (level === 15) dur = info.baseDurability + (level * 2 + 17);
    else dur = info.baseDurability + (level * 2 - 4);
  } else {
    dur = info.baseDurability + level;
  }

  const archangel = new Set([
    getItemIndex(0, 19),
    getItemIndex(2, 13),
    getItemIndex(4, 18),
    getItemIndex(5, 10),
    getItemIndex(5, 36),
  ]);
  if (!archangel.has(index) && info.slot !== 7) {
    if (setOption) dur += 20;
    else if (excellent) dur += 15;
  }

  return Math.min(255, Math.max(0, dur));
}
