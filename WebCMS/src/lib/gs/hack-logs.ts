import fs from "fs/promises";
import path from "path";
import {
  getAntiHackRoot,
  getGameServerLogRoot,
  getGsDataRoot,
} from "@/lib/gs/paths";
import { publicFileName } from "@/lib/security/client-error";

export type HackLogSource = "gs" | "antihack";

export type HackLogEntry = {
  source: HackLogSource;
  date: string;
  time: string;
  account: string | null;
  character: string | null;
  ip: string | null;
  hwid: string | null;
  kind: string;
  message: string;
  raw: string;
  file: string;
};

export type BlackListId = "antihack" | "gameserver";

export type BlackListSnapshot = {
  id: BlackListId;
  label: string;
  ips: string[];
  hwids: string[];
};

export type HackScanResult = {
  entries: HackLogEntry[];
  scannedFiles: number;
  days: number;
  query: string;
  sources: HackLogSource[];
  blacklists: BlackListSnapshot[];
};

function dateKey(d: Date): string {
  const y = d.getFullYear();
  const m = String(d.getMonth() + 1).padStart(2, "0");
  const day = String(d.getDate()).padStart(2, "0");
  return `${y}-${m}-${day}`;
}

function recentDateKeys(days: number): string[] {
  const out: string[] = [];
  const now = new Date();
  for (let i = 0; i < days; i++) {
    const d = new Date(now);
    d.setDate(now.getDate() - i);
    out.push(dateKey(d));
  }
  return out;
}

async function readTextMaybe(filePath: string): Promise<string | null> {
  try {
    const buf = await fs.readFile(filePath);
    // AntiHack/GS logs on Windows often use ANSI/UTF-16-ish; try utf8 then latin1.
    const utf8 = buf.toString("utf8");
    if (!utf8.includes("\uFFFD")) return utf8;
    return buf.toString("latin1");
  } catch {
    return null;
  }
}

function extractIp(text: string): string | null {
  const m = text.match(/\b(\d{1,3}(?:\.\d{1,3}){3})\b/);
  return m?.[1] ?? null;
}

function extractTagged(text: string): {
  kind: string;
  account: string | null;
  character: string | null;
  rest: string;
} {
  // [Kind][Account][Name] rest
  let m = text.match(/^\[([^\]]+)\]\[([^\]]+)\]\[([^\]]+)\]\s*(.*)$/);
  if (m) {
    return {
      kind: m[1],
      account: m[2] || null,
      character: m[3] || null,
      rest: m[4] || "",
    };
  }
  // [Account][Name] rest  (Protocol.cpp style)
  m = text.match(/^\[([^\]]+)\]\[([^\]]+)\]\s*(.*)$/);
  if (m) {
    const rest = m[3] || "";
    const kind =
      rest.match(/\b(Speed Hack|Latency Hack|CheckSum|checksum)\b/i)?.[0] ||
      "HackDetect";
    return {
      kind,
      account: m[1] || null,
      character: m[2] || null,
      rest,
    };
  }
  return { kind: "Log", account: null, character: null, rest: text };
}

function parseGsHackLine(
  date: string,
  line: string,
  file: string
): HackLogEntry | null {
  const trimmed = line.trim();
  if (!trimmed) return null;
  const m = trimmed.match(/^(\d{2}:\d{2}:\d{2})\s+(.*)$/);
  if (!m) return null;
  const time = m[1];
  const body = m[2];
  const tagged = extractTagged(body);
  return {
    source: "gs",
    date,
    time,
    account: tagged.account,
    character: tagged.character,
    ip: extractIp(body),
    hwid: null,
    kind: tagged.kind,
    message: tagged.rest || body,
    raw: trimmed,
    file,
  };
}

function parseAntiHackLine(
  date: string,
  line: string,
  file: string,
  pending?: { ip?: string | null; hwid?: string | null }
): HackLogEntry | null {
  const trimmed = line.trim();
  if (!trimmed) return null;
  const m = trimmed.match(/^(\d{2}:\d{2}:\d{2})\s+(.*)$/);
  if (!m) return null;
  const time = m[1];
  const body = m[2];

  const ipHw = body.match(
    /^IP:\s*([^\s/]+)\s*\/\s*ID:\s*(.+)$/i
  );
  if (ipHw) {
    if (pending) {
      pending.ip = ipHw[1];
      pending.hwid = ipHw[2].trim();
    }
    return {
      source: "antihack",
      date,
      time,
      account: null,
      character: null,
      ip: ipHw[1],
      hwid: ipHw[2].trim(),
      kind: "ClientInfo",
      message: body,
      raw: trimmed,
      file,
    };
  }

  const noise =
    /khởi động|tải cấu hình|đã được tải|hệ thống chống|watchlist|badwords|internallist|internalist|checksumlist|dumplist|windowlist|blacklist|port \d+|thành công|thanh cong|da duoc tai|khoi dong/i.test(
      body
    );
  if (noise) return null;

  let kind = "AntiHack";
  if (/ngắt kết nối/i.test(body)) kind = "Disconnect";
  else if (/đã kết nối/i.test(body)) kind = "Connect";
  else if (/hack|detect|phát hiện|block|khóa|dump|window|checksum/i.test(body))
    kind = "Detect";

  // Keep connect/disconnect only when searching, but still parse them.
  return {
    source: "antihack",
    date,
    time,
    account: null,
    character: null,
    ip: pending?.ip ?? extractIp(body),
    hwid: pending?.hwid ?? null,
    kind,
    message: body,
    raw: trimmed,
    file,
  };
}

function matchesQuery(entry: HackLogEntry, q: string): boolean {
  if (!q) {
    // Default view: skip noisy connect/info unless searching.
    if (entry.source === "antihack") {
      return !["Connect", "Disconnect", "ClientInfo"].includes(entry.kind);
    }
    return true;
  }
  const needle = q.toLowerCase();
  const hay = [
    entry.account,
    entry.character,
    entry.ip,
    entry.hwid,
    entry.kind,
    entry.message,
    entry.raw,
  ]
    .filter(Boolean)
    .join(" ")
    .toLowerCase();
  return hay.includes(needle);
}

async function collectFromDir(
  dir: string,
  source: HackLogSource,
  dates: string[],
  q: string,
  limit: number,
  out: HackLogEntry[]
): Promise<number> {
  let scanned = 0;
  for (const date of dates) {
    if (out.length >= limit) break;
    const file = path.join(dir, `${date}.txt`);
    const text = await readTextMaybe(file);
    if (text == null) continue;
    scanned += 1;
    const lines = text.split(/\r?\n/);
    const pending: { ip?: string | null; hwid?: string | null } = {};
    // Newest first within file
    for (let i = lines.length - 1; i >= 0; i--) {
      if (out.length >= limit) break;
      const line = lines[i];
      const publicName = publicFileName(file);
      const entry =
        source === "gs"
          ? parseGsHackLine(date, line, publicName)
          : parseAntiHackLine(date, line, publicName, pending);
      if (!entry) continue;
      if (!matchesQuery(entry, q)) continue;
      out.push(entry);
    }
  }
  return scanned;
}

function parseBlackListSections(content: string): {
  ips: string[];
  hwids: string[];
} {
  const ips: string[] = [];
  const hwids: string[] = [];
  let section: "ip" | "hwid" | null = null;
  for (const raw of content.split(/\r?\n/)) {
    const line = raw.trim();
    if (!line || line.startsWith("//") || line.startsWith(";")) {
      const lower = line.toLowerCase();
      if (lower.includes("ip")) section = "ip";
      if (lower.includes("hwid") || lower.includes("hardware")) section = "hwid";
      continue;
    }
    if (/^end$/i.test(line)) {
      section = null;
      continue;
    }
    if (/^\d+$/.test(line)) {
      // section marker 0 = IP, 1 = HWID in this project's files
      section = line === "0" ? "ip" : line === "1" ? "hwid" : section;
      continue;
    }
    if (section === "ip") ips.push(line);
    else if (section === "hwid") hwids.push(line);
  }
  return { ips, hwids };
}

async function loadBlackList(
  filePath: string,
  id: BlackListId,
  label: string
): Promise<BlackListSnapshot | null> {
  const text = await readTextMaybe(filePath);
  if (text == null) return null;
  const { ips, hwids } = parseBlackListSections(text);
  return { id, label, ips, hwids };
}

export async function scanHackLogs(options?: {
  q?: string;
  days?: number;
  limit?: number;
  sources?: HackLogSource[];
}): Promise<HackScanResult> {
  const q = (options?.q || "").trim();
  const days = Math.min(Math.max(options?.days ?? 14, 1), 60);
  const limit = Math.min(Math.max(options?.limit ?? 200, 1), 1000);
  const sources = options?.sources?.length
    ? options.sources
    : (["gs", "antihack"] as HackLogSource[]);

  const dates = recentDateKeys(days);
  const entries: HackLogEntry[] = [];
  let scannedFiles = 0;

  const gsHackDir = path.join(getGameServerLogRoot(), "HACK_LOG");
  const ahLogDir = path.join(getAntiHackRoot(), "LOG");
  const ahHackDir = path.join(ahLogDir, "HACK_LOG");

  if (sources.includes("gs")) {
    scannedFiles += await collectFromDir(
      gsHackDir,
      "gs",
      dates,
      q,
      limit,
      entries
    );
  }
  if (sources.includes("antihack")) {
    scannedFiles += await collectFromDir(
      ahHackDir,
      "antihack",
      dates,
      q,
      limit,
      entries
    );
    // AntiHack runtime log often holds connect + detect lines
    if (entries.length < limit) {
      scannedFiles += await collectFromDir(
        ahLogDir,
        "antihack",
        dates,
        q,
        limit,
        entries
      );
    }
  }

  // Sort newest first by date+time
  entries.sort((a, b) => {
    const ka = `${a.date} ${a.time}`;
    const kb = `${b.date} ${b.time}`;
    return kb.localeCompare(ka);
  });

  const blacklists = (
    await Promise.all([
      loadBlackList(
        path.join(getAntiHackRoot(), "BlackList.txt"),
        "antihack",
        "AntiHack BlackList"
      ),
      loadBlackList(
        path.join(getGsDataRoot(), "BlackList.txt"),
        "gameserver",
        "GameServer BlackList"
      ),
    ])
  ).filter((x): x is BlackListSnapshot => Boolean(x));

  return {
    entries: entries.slice(0, limit),
    scannedFiles,
    days,
    query: q,
    sources,
    blacklists,
  };
}
