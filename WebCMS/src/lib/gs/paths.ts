import fs from "fs";
import path from "path";

function envPath(key: string): string | null {
  const value = process.env[key]?.trim();
  return value ? path.resolve(value) : null;
}

function isMuServerRoot(dir: string): boolean {
  try {
    const hasSub = fs.existsSync(path.join(dir, "4.Sub-1"));
    const hasDs = fs.existsSync(path.join(dir, "2.DataServer"));
    const hasCs = fs.existsSync(path.join(dir, "1.ConnectServer"));
    return hasSub && (hasDs || hasCs);
  } catch {
    return false;
  }
}

/**
 * Resolve the Mu Server folder from env or by walking up from process.cwd().
 * Never hardcode a machine drive/path — the web must run on any host.
 */
export function getMuServerRoot(): string {
  const fromEnv = envPath("MU_SERVER_ROOT");
  if (fromEnv) return fromEnv;

  const cwd = path.resolve(process.cwd());
  const candidates: string[] = [
    path.join(cwd, "Mu Server"),
    path.join(cwd, "..", "Mu Server"),
  ];
  let cursor = cwd;
  for (let i = 0; i < 6; i++) {
    candidates.push(path.join(cursor, "Mu Server"));
    candidates.push(cursor);
    const parent = path.dirname(cursor);
    if (parent === cursor) break;
    cursor = parent;
  }

  const seen = new Set<string>();
  for (const candidate of candidates) {
    const resolved = path.resolve(candidate);
    if (seen.has(resolved)) continue;
    seen.add(resolved);
    if (isMuServerRoot(resolved)) return resolved;
  }

  return path.resolve(cwd, "..", "Mu Server");
}

export function getGsIniRoot(): string {
  return (
    envPath("GS_INI_ROOT") ??
    path.join(getMuServerRoot(), "4.Sub-1", "GameServer", "Data")
  );
}

export function getGsDataRoot(): string {
  return (
    envPath("GS_DATA_ROOT") ??
    path.join(getMuServerRoot(), "4.Sub-1", "Data")
  );
}

export function getGameServerExeDir(): string {
  return path.join(getMuServerRoot(), "4.Sub-1", "GameServer");
}

export function getGameServerExe(): string {
  return path.join(getGameServerExeDir(), "GameServer.exe");
}

export function getGameServerLogRoot(): string {
  return path.join(getGameServerExeDir(), "LOG");
}

export function getAntiHackRoot(): string {
  return path.join(getMuServerRoot(), "6.AntiHack");
}

export const GS_INI_FILES = [
  "GameServerInfo - Common.ini",
  "GameServerInfo - ChaosMix.ini",
  "GameServerInfo - Character.ini",
  "GameServerInfo - Command.ini",
  "GameServerInfo - Custom.ini",
  "GameServerInfo - Event.ini",
  "GameServerInfo - Skill.ini",
] as const;

export type GsIniName = (typeof GS_INI_FILES)[number];

export function isGsIniName(name: string): name is GsIniName {
  return (GS_INI_FILES as readonly string[]).includes(name);
}

export function gsIniSlugToFile(slug: string): GsIniName | null {
  const normalized = slug.trim().toLowerCase();
  const map: Record<string, GsIniName> = {
    common: "GameServerInfo - Common.ini",
    chaosmix: "GameServerInfo - ChaosMix.ini",
    character: "GameServerInfo - Character.ini",
    command: "GameServerInfo - Command.ini",
    custom: "GameServerInfo - Custom.ini",
    event: "GameServerInfo - Event.ini",
    skill: "GameServerInfo - Skill.ini",
  };
  return map[normalized] ?? null;
}

export function gsIniFileToSlug(fileName: GsIniName): string {
  return fileName
    .replace(/^GameServerInfo - /i, "")
    .replace(/\.ini$/i, "")
    .toLowerCase();
}

/** Keys that require full stack restart (ports / DS / JS / identity). */
export const STARTUP_ONLY_KEYS = new Set([
  "ServerPort",
  "ServerCode",
  "ServerName",
  "ServerVersion",
  "ServerSerial",
  "DataServerAddress",
  "DataServerPort",
  "JoinServerAddress",
  "JoinServerPort",
  "ConnectServerAddress",
  "ConnectServerPort",
  "MaxUserCount",
]);
