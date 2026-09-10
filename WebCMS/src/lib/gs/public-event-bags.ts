import fs from "fs/promises";
import path from "path";
import { getGsDataRoot } from "@/lib/gs/paths";
import { getItemInfo, getItemIndex, loadItemCatalog } from "@/lib/gs/item-catalog";
import {
  memScriptDataLines,
  splitMemScriptSections,
  tokenizeRow,
} from "@/lib/gs/public-guide-parse";

export type PublicBagReward = {
  name: string;
  rate: number | null;
  minLevel: number | null;
  maxLevel: number | null;
};

export type PublicEventBagSummary = {
  id: string;
  label: string;
  eventName: string | null;
  dropZen: number | null;
  itemDropRate: number | null;
  rewards: PublicBagReward[];
};

/** Curated bags that map clearly to player-facing events. */
/** Labels are fallbacks; EventName inside each bag file wins in the UI. */
const CURATED: {
  id: string;
  label: string;
  filePrefix: string;
}[] = [
  { id: "bc1", label: "Blood Castle 1", filePrefix: "060" },
  { id: "bcVip", label: "Blood Castle VIP", filePrefix: "118" },
  { id: "cc1", label: "Chaos Castle 1", filePrefix: "068" },
  { id: "it1", label: "Illusion Temple 1", filePrefix: "077" },
  { id: "dsVip", label: "Devil Square VIP", filePrefix: "119" },
  { id: "kundun", label: "Kundun", filePrefix: "157" },
];

async function findBagFile(prefix: string): Promise<string | null> {
  const dir = path.join(getGsDataRoot(), "EventItemBag");
  try {
    const entries = await fs.readdir(dir);
    const hit = entries.find(
      (n) => n.startsWith(prefix) && n.toLowerCase().endsWith(".txt")
    );
    return hit ? path.join(dir, hit) : null;
  } catch {
    return null;
  }
}

function resolveItemName(section: number, type: number, comment: string | null): string {
  try {
    loadItemCatalog();
    const info = getItemInfo(getItemIndex(section, type));
    if (info?.name) return info.name;
  } catch {
    // catalog optional
  }
  if (comment) return comment;
  return `Item ${section}:${type}`;
}

function parseBagFile(text: string): {
  eventName: string | null;
  dropZen: number | null;
  itemDropRate: number | null;
  rewards: PublicBagReward[];
} {
  const sections = splitMemScriptSections(text);
  let eventName: string | null = null;
  let dropZen: number | null = null;
  let itemDropRate: number | null = null;

  const metaLine = memScriptDataLines(sections.get(0) ?? "")[0];
  if (metaLine) {
    const t = tokenizeRow(metaLine);
    // EventName DropZen ItemDropRate ItemDropCount ...
    if (t.length >= 3) {
      eventName = t[0] || null;
      dropZen = Number.isFinite(Number(t[1])) ? Number(t[1]) : null;
      itemDropRate = Number.isFinite(Number(t[2])) ? Number(t[2]) : null;
    }
  }

  const rewards: PublicBagReward[] = [];
  for (const line of memScriptDataLines(sections.get(1) ?? "")) {
    const t = tokenizeRow(line);
    // Section Type MinLevel MaxLevel Skill Luck Option Excellent SetOption Rate
    if (t.length < 10) continue;
    const section = Number(t[0]);
    const type = Number(t[1]);
    if (!Number.isFinite(section) || !Number.isFinite(type)) continue;
    const minLevel = Number(t[2]);
    const maxLevel = Number(t[3]);
    const rate = Number(t[9]);
    const commentMatch = line.match(/\/\/\s*(.+)$/);
    rewards.push({
      name: resolveItemName(
        section,
        type,
        commentMatch ? commentMatch[1].trim() : null
      ),
      rate: Number.isFinite(rate) ? rate : null,
      minLevel: Number.isFinite(minLevel) ? minLevel : null,
      maxLevel: Number.isFinite(maxLevel) ? maxLevel : null,
    });
    if (rewards.length >= 12) break;
  }

  return { eventName, dropZen, itemDropRate, rewards };
}

export async function buildPublicEventBagSummaries(): Promise<
  PublicEventBagSummary[]
> {
  const out: PublicEventBagSummary[] = [];
  for (const bag of CURATED) {
    const full = await findBagFile(bag.filePrefix);
    if (!full) continue;
    try {
      const text = await fs.readFile(full, "utf8");
      const parsed = parseBagFile(text);
      out.push({
        id: bag.id,
        label: bag.label,
        eventName: parsed.eventName,
        dropZen: parsed.dropZen,
        itemDropRate: parsed.itemDropRate,
        rewards: parsed.rewards,
      });
    } catch {
      // skip unreadable bag
    }
  }
  return out;
}
