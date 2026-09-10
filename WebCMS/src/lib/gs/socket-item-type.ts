import fs from "fs";
import path from "path";
import { getGsDataRoot } from "@/lib/gs/paths";

function getItemIndex(section: number, type: number): number {
  return section * 512 + type;
}

let cache: Map<number, number> | null = null;
let cacheMtime = 0;

function socketTypePath(): string {
  const fromEnv = process.env.GS_SOCKET_ITEM_TYPE?.trim();
  if (fromEnv) return path.resolve(fromEnv);
  return path.join(getGsDataRoot(), "Item", "SocketItemType.txt");
}

function parseSocketItemType(text: string): Map<number, number> {
  const map = new Map<number, number>();
  for (const raw of text.replace(/^\uFEFF/, "").split(/\r?\n/)) {
    const line = raw.trim();
    if (!line || line.startsWith("//") || line.startsWith(";") || /^end$/i.test(line)) {
      continue;
    }
    if (/^section\b/i.test(line)) continue;
    const parts = line.split(/\s+/).filter(Boolean);
    if (parts.length < 3) continue;
    const section = Number(parts[0]);
    const type = Number(parts[1]);
    const maxSocket = Number(parts[2]);
    if (![section, type, maxSocket].every((n) => Number.isFinite(n))) continue;
    map.set(getItemIndex(section, type), Math.max(0, Math.min(5, maxSocket)));
  }
  return map;
}

export function loadSocketItemTypes(force = false): Map<number, number> {
  const file = socketTypePath();
  const st = fs.statSync(file);
  if (!force && cache && st.mtimeMs === cacheMtime) return cache;
  cache = parseSocketItemType(fs.readFileSync(file, "utf8"));
  cacheMtime = st.mtimeMs;
  return cache;
}

/** Max empty sockets allowed for this item index; 0 = not a socket item type. */
export function getMaxSocketForItem(index: number): number {
  return loadSocketItemTypes().get(index) ?? 0;
}

export function isSocketItemType(index: number): boolean {
  return getMaxSocketForItem(index) > 0;
}
