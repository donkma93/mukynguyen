import fs from "fs/promises";
import path from "path";
import { getGsDataRoot, getGsIniRoot } from "@/lib/gs/paths";

export type GsRootKind = "ini" | "data";

function rootFor(kind: GsRootKind): string {
  return kind === "ini" ? getGsIniRoot() : getGsDataRoot();
}

export function resolveAllowedPath(kind: GsRootKind, relativePath: string): string {
  const root = path.resolve(rootFor(kind));
  const cleaned = relativePath.replace(/\\/g, "/").replace(/^\/+/, "");
  if (!cleaned || cleaned.includes("\0")) {
    throw new Error("Đường dẫn không hợp lệ");
  }
  if (cleaned.split("/").some((p) => p === "..")) {
    throw new Error("Không cho phép đường dẫn cha (..)");
  }
  const full = path.resolve(root, cleaned);
  const rel = path.relative(root, full);
  if (rel.startsWith("..") || path.isAbsolute(rel)) {
    throw new Error("Đường dẫn nằm ngoài thư mục GS được phép");
  }
  return full;
}

export async function readTextFile(kind: GsRootKind, relativePath: string): Promise<string> {
  const full = resolveAllowedPath(kind, relativePath);
  return fs.readFile(full, "utf8");
}

export async function fileExists(kind: GsRootKind, relativePath: string): Promise<boolean> {
  try {
    const full = resolveAllowedPath(kind, relativePath);
    await fs.access(full);
    return true;
  } catch {
    return false;
  }
}

export async function backupFile(fullPath: string): Promise<string> {
  const stamp = new Date()
    .toISOString()
    .replace(/[:.]/g, "-")
    .replace("T", "_")
    .slice(0, 19);
  const backupPath = `${fullPath}.bak.${stamp}`;
  await fs.copyFile(fullPath, backupPath);
  return backupPath;
}

export async function writeTextFile(
  kind: GsRootKind,
  relativePath: string,
  content: string,
  options?: { createBackup?: boolean }
): Promise<{ fullPath: string; backupPath: string | null }> {
  const fullPath = resolveAllowedPath(kind, relativePath);
  let backupPath: string | null = null;
  try {
    await fs.access(fullPath);
    if (options?.createBackup !== false) {
      backupPath = await backupFile(fullPath);
    }
  } catch {
    // new file — no backup
  }
  await fs.mkdir(path.dirname(fullPath), { recursive: true });
  await fs.writeFile(fullPath, content, "utf8");
  return { fullPath, backupPath };
}

export async function listIniFiles(): Promise<
  { name: string; slug: string; size: number; mtime: string }[]
> {
  const root = getGsIniRoot();
  const entries = await fs.readdir(root, { withFileTypes: true });
  const out: { name: string; slug: string; size: number; mtime: string }[] = [];
  for (const e of entries) {
    if (!e.isFile()) continue;
    if (!/^GameServerInfo - .+\.ini$/i.test(e.name)) continue;
    const st = await fs.stat(path.join(root, e.name));
    const slug = e.name
      .replace(/^GameServerInfo - /i, "")
      .replace(/\.ini$/i, "")
      .toLowerCase();
    out.push({
      name: e.name,
      slug,
      size: st.size,
      mtime: st.mtime.toISOString(),
    });
  }
  return out.sort((a, b) => a.name.localeCompare(b.name));
}
