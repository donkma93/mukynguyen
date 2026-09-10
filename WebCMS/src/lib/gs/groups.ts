import fs from "fs/promises";
import path from "path";
import { applyIniUpdates, parseIni } from "@/lib/gs/ini";
import { iniInt } from "@/lib/gs/public-guide-parse";
import { getMuServerRoot } from "@/lib/gs/paths";

/** Server group: all members share the same drop rates. */
export type GsGroupId = "normal" | "vip";

export type GsSubId =
  | "sub1"
  | "sub2"
  | "sub3"
  | "sub4"
  | "sub5"
  | "sub6";

export type GsSubDef = {
  id: GsSubId;
  /** Folder under Mu Server, e.g. 4.Sub-1 */
  folder: string;
  label: string;
  group: GsGroupId;
  serverCode: number;
  serverPort: number;
};

/**
 * 6 GameServers in 2 groups:
 * - Thường: Sub-1 / Sub-2 / Sub-3 (same rates)
 * - VIP:    Sub-4 / Sub-5 / Sub-6 (same rates)
 *
 * Only folders that exist on disk are written; missing subs are skipped.
 */
export const GS_SUBS: readonly GsSubDef[] = [
  { id: "sub1", folder: "4.Sub-1", label: "Sub-1", group: "normal", serverCode: 0, serverPort: 55901 },
  { id: "sub2", folder: "4.Sub-2", label: "Sub-2", group: "normal", serverCode: 1, serverPort: 55902 },
  { id: "sub3", folder: "4.Sub-3", label: "Sub-3", group: "normal", serverCode: 2, serverPort: 55903 },
  { id: "sub4", folder: "4.Sub-4", label: "Sub-4", group: "vip", serverCode: 3, serverPort: 55904 },
  { id: "sub5", folder: "4.Sub-5", label: "Sub-5", group: "vip", serverCode: 4, serverPort: 55905 },
  { id: "sub6", folder: "4.Sub-6", label: "Sub-6", group: "vip", serverCode: 5, serverPort: 55906 },
] as const;

export const GS_GROUPS: readonly { id: GsGroupId; labelVi: string; labelEn: string }[] = [
  { id: "normal", labelVi: "Thường", labelEn: "Normal" },
  { id: "vip", labelVi: "VIP", labelEn: "VIP" },
] as const;

/** Drop rates for one group (item/jewel share ItemDropRate per 3A). */
export type GroupDropRates = {
  itemDropRate: number | null;
  moneyDropRate: number | null;
  /** Same as itemDropRate until GS has a separate jewel key. */
  jewelDropRate: number | null;
};

export type GroupRatesSnapshot = {
  normal: GroupDropRates;
  vip: GroupDropRates;
  /** Subs that exist on disk and were read. */
  available: { id: GsSubId; label: string; group: GsGroupId }[];
  /** Subs planned but folder missing. */
  missing: { id: GsSubId; label: string; group: GsGroupId; folder: string }[];
};

function commonIniRelative(): string {
  return path.join("GameServer", "Data", "GameServerInfo - Common.ini");
}

/**
 * Pending rates for groups whose Sub folders are not created yet.
 * Written under Mu Server so VIP rates survive before Sub-4/5/6 exist.
 */
function pendingRatesPath(): string {
  return path.join(getMuServerRoot(), "WebCMS-GroupRates.json");
}

export function getSubRoot(sub: GsSubDef): string {
  return path.join(getMuServerRoot(), sub.folder);
}

export function getSubCommonIniPath(sub: GsSubDef): string {
  return path.join(getSubRoot(sub), commonIniRelative());
}

export async function subExists(sub: GsSubDef): Promise<boolean> {
  try {
    await fs.access(getSubCommonIniPath(sub));
    return true;
  } catch {
    return false;
  }
}

export function subsInGroup(group: GsGroupId): GsSubDef[] {
  return GS_SUBS.filter((s) => s.group === group);
}

/** Read one uniform rate from AL0–AL3 (1A: all tiers equal on a GS). Prefers AL0. */
function readUniformAlRate(
  values: Record<string, string>,
  keyPrefix: string
): number | null {
  const al0 = iniInt(values[`${keyPrefix}_AL0`]);
  if (al0 != null) return al0;
  for (const al of [1, 2, 3] as const) {
    const v = iniInt(values[`${keyPrefix}_AL${al}`]);
    if (v != null) return v;
  }
  return null;
}

async function readSubRates(sub: GsSubDef): Promise<GroupDropRates | null> {
  try {
    const text = await fs.readFile(getSubCommonIniPath(sub), "utf8");
    const values = parseIni(text).values;
    const item = readUniformAlRate(values, "ItemDropRate");
    const money = readUniformAlRate(values, "MoneyAmountDropRate");
    return {
      itemDropRate: item,
      moneyDropRate: money,
      jewelDropRate: item,
    };
  } catch {
    return null;
  }
}

async function readPendingRates(): Promise<
  Partial<Record<GsGroupId, GroupDropRates>>
> {
  try {
    const raw = await fs.readFile(pendingRatesPath(), "utf8");
    const json = JSON.parse(raw) as Partial<
      Record<GsGroupId, { itemDropRate?: number | null; moneyDropRate?: number | null }>
    >;
    const out: Partial<Record<GsGroupId, GroupDropRates>> = {};
    for (const id of ["normal", "vip"] as const) {
      const row = json[id];
      if (!row) continue;
      const item =
        row.itemDropRate == null || !Number.isFinite(Number(row.itemDropRate))
          ? null
          : Number(row.itemDropRate);
      const money =
        row.moneyDropRate == null || !Number.isFinite(Number(row.moneyDropRate))
          ? null
          : Number(row.moneyDropRate);
      out[id] = {
        itemDropRate: item,
        moneyDropRate: money,
        jewelDropRate: item,
      };
    }
    return out;
  } catch {
    return {};
  }
}

async function writePendingRates(
  group: GsGroupId,
  updates: GroupRateUpdates
): Promise<void> {
  const current = await readPendingRates();
  const prev = current[group] ?? {
    itemDropRate: null,
    moneyDropRate: null,
    jewelDropRate: null,
  };
  const next: GroupDropRates = {
    itemDropRate:
      updates.itemDropRate != null ? updates.itemDropRate : prev.itemDropRate,
    moneyDropRate:
      updates.moneyDropRate != null ? updates.moneyDropRate : prev.moneyDropRate,
    jewelDropRate: null,
  };
  next.jewelDropRate = next.itemDropRate;
  current[group] = next;
  const payload = {
    normal: current.normal
      ? {
          itemDropRate: current.normal.itemDropRate,
          moneyDropRate: current.normal.moneyDropRate,
        }
      : undefined,
    vip: current.vip
      ? {
          itemDropRate: current.vip.itemDropRate,
          moneyDropRate: current.vip.moneyDropRate,
        }
      : undefined,
  };
  await fs.writeFile(pendingRatesPath(), JSON.stringify(payload, null, 2), "utf8");
}

/**
 * Build guide/admin snapshot: one rate set per group from the first available sub.
 * If a group has no folder yet, use WebCMS-GroupRates.json pending rates.
 */
export async function loadGroupRatesSnapshot(): Promise<GroupRatesSnapshot> {
  const available: GroupRatesSnapshot["available"] = [];
  const missing: GroupRatesSnapshot["missing"] = [];
  const byGroup: Partial<Record<GsGroupId, GroupDropRates>> = {};

  for (const sub of GS_SUBS) {
    if (await subExists(sub)) {
      available.push({
        id: sub.id,
        label: sub.label,
        group: sub.group,
      });
      if (!byGroup[sub.group]) {
        const rates = await readSubRates(sub);
        if (rates) byGroup[sub.group] = rates;
      }
    } else {
      missing.push({
        id: sub.id,
        label: sub.label,
        group: sub.group,
        folder: sub.folder,
      });
    }
  }

  const pending = await readPendingRates();
  for (const id of ["normal", "vip"] as const) {
    if (!byGroup[id] && pending[id]) {
      byGroup[id] = pending[id];
    }
  }

  const empty = (): GroupDropRates => ({
    itemDropRate: null,
    moneyDropRate: null,
    jewelDropRate: null,
  });

  return {
    normal: byGroup.normal ?? empty(),
    vip: byGroup.vip ?? empty(),
    available,
    missing,
  };
}

export type GroupRateUpdates = {
  itemDropRate?: number;
  moneyDropRate?: number;
};

/**
 * Write the same rates to every existing Common.ini in the group.
 * Sets AL0–AL3 to the same value (1A: one rate per GS).
 * Jewel uses ItemDropRate (3A).
 */
export async function writeGroupRates(
  group: GsGroupId,
  updates: GroupRateUpdates
): Promise<{
  written: { id: GsSubId; label: string; folder: string; changed: string[] }[];
  skipped: { id: GsSubId; label: string; reason: string }[];
}> {
  const written: {
    id: GsSubId;
    label: string;
    folder: string;
    changed: string[];
  }[] = [];
  const skipped: { id: GsSubId; label: string; reason: string }[] = [];

  const keyUpdates: Record<string, string> = {};
  if (updates.itemDropRate != null) {
    const v = String(Math.max(0, Math.floor(updates.itemDropRate)));
    for (const al of [0, 1, 2, 3]) {
      keyUpdates[`ItemDropRate_AL${al}`] = v;
    }
  }
  if (updates.moneyDropRate != null) {
    const v = String(Math.max(0, Math.floor(updates.moneyDropRate)));
    for (const al of [0, 1, 2, 3]) {
      keyUpdates[`MoneyAmountDropRate_AL${al}`] = v;
    }
  }

  if (!Object.keys(keyUpdates).length) {
    return { written, skipped };
  }

  let anyExisting = false;
  for (const sub of subsInGroup(group)) {
    const full = getSubCommonIniPath(sub);
    if (!(await subExists(sub))) {
      skipped.push({
        id: sub.id,
        label: sub.label,
        reason: `Chưa có thư mục ${sub.folder}`,
      });
      continue;
    }
    anyExisting = true;
    try {
      const current = await fs.readFile(full, "utf8");
      const applied = applyIniUpdates(current, keyUpdates);
      if (!applied.changed.length) {
        skipped.push({
          id: sub.id,
          label: sub.label,
          reason: "Không có thay đổi",
        });
        continue;
      }
      const stamp = new Date()
        .toISOString()
        .replace(/[:.]/g, "-")
        .replace("T", "_")
        .slice(0, 19);
      await fs.copyFile(full, `${full}.bak.${stamp}`);
      let next = applied.text;
      if (!next.endsWith("\r\n") && !next.endsWith("\n")) next += "\r\n";
      await fs.writeFile(full, next, "utf8");
      written.push({
        id: sub.id,
        label: sub.label,
        folder: sub.folder,
        changed: applied.changed,
      });
    } catch {
      skipped.push({
        id: sub.id,
        label: sub.label,
        reason: "Lỗi ghi file",
      });
    }
  }

  // Keep pending overlay so guide/admin still show VIP rates before Sub-4/5/6 exist.
  if (!anyExisting) {
    await writePendingRates(group, updates);
    written.push({
      id: subsInGroup(group)[0].id,
      label: `pending:${group}`,
      folder: "pending",
      changed: Object.keys(keyUpdates),
    });
  } else {
    // Mirror last known rates for the group (useful if some members still missing).
    await writePendingRates(group, updates);
  }

  return { written, skipped };
}
