import fs from "fs";
import path from "path";
import { getGsDataRoot } from "@/lib/gs/paths";

export type GameItem = {
  section: number;
  type: number;
  index: number;
  name: string;
  maxSocket: number;
};

function itemDataPath() {
  const candidate = path.join(getGsDataRoot(), "Item", "Item.txt");
  return fs.existsSync(candidate) ? candidate : null;
}

function searchKey(value: string) {
  return value
    .normalize("NFD")
    .replace(/[\u0300-\u036f]/g, "")
    .toLocaleLowerCase("vi-VN");
}

export function readGameItems(): GameItem[] {
  const source = itemDataPath();
  if (!source) return [];

  const socketItems = new Map<number, number>();
  const socketPath = source.replace(/Item\.txt$/i, "SocketItemType.txt");
  if (fs.existsSync(socketPath)) {
    for (const line of fs.readFileSync(socketPath, "utf8").split(/\r?\n/)) {
      const socket = line.match(/^\s*(\d+)\s+(\d+)\s+(\d+)\s*$/);
      if (!socket) continue;
      const section = Number(socket[1]);
      const type = Number(socket[2]);
      const maxSocket = Number(socket[3]);
      if (section >= 0 && section <= 15 && type >= 0 && type <= 511 && maxSocket > 0) {
        socketItems.set(section * 512 + type, Math.min(5, maxSocket));
      }
    }
  }

  const items: GameItem[] = [];
  let section = -1;
  for (const line of fs.readFileSync(source, "utf8").split(/\r?\n/)) {
    const group = line.match(/^\s*(\d+)\s*$/);
    if (group) {
      section = Number(group[1]);
      continue;
    }

    const item = line.match(/^\s*(\d+)\s+.*?"([^"]+)"/);
    if (!item || section < 0 || section > 15) continue;
    const type = Number(item[1]);
    if (type < 0 || type > 511) continue;
    const index = section * 512 + type;
    items.push({ section, type, index, name: item[2].trim(), maxSocket: socketItems.get(index) ?? 0 });
  }
  return items;
}

export function getGameItemByIndex(index: number): GameItem | null {
  return readGameItems().find((item) => item.index === index) ?? null;
}

export function searchGameItems(query: string, limit = 60): GameItem[] {
  const normalized = searchKey(query.trim());
  if (!normalized) return [];
  return readGameItems()
    .filter((item) => searchKey(item.name).includes(normalized))
    .slice(0, Math.max(1, Math.min(100, limit)));
}
