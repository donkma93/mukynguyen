import fs from "fs/promises";
import path from "path";
import { backupFile } from "@/lib/gs/files";
import { getMuServerRoot } from "@/lib/gs/paths";

const SUB_NAMES = ["4.Sub-1", "4.Sub-2", "4.Sub-3", "4.Sub-4", "4.Sub-5"] as const;
const EVENT_INI = path.join("GameServer", "Data", "GameServerInfo - Event.ini");
const MOSS_DATA = path.join("Data", "Event", "MossMerchant.dat");
const TEST_MARKER = "// WebCMS TEST";

export type MossSchedule = {
  year: string;
  month: string;
  day: string;
  dayOfWeek: string;
  hour: number;
  minute: number;
  second: number;
  isDaily: boolean;
};

type MossFiles = { iniPath: string; dataPath: string };

function subFiles(subName: string): MossFiles {
  const root = getMuServerRoot();
  return {
    iniPath: path.join(root, subName, EVENT_INI),
    dataPath: path.join(root, subName, MOSS_DATA),
  };
}

function normalizeNewlines(text: string): string {
  return text.replace(/\r?\n/g, "\r\n");
}

function replaceIniNumber(text: string, key: string, value: number): string {
  const expression = new RegExp(`^(\\s*${key}\\s*=\\s*)\\d+(\\s*(?:[;#].*)?)$`, "m");
  if (!expression.test(text)) {
    throw new Error(`Không tìm thấy ${key} trong GameServerInfo - Event.ini`);
  }
  return text.replace(expression, `$1${value}$2`);
}

function getIniNumber(text: string, key: string): number {
  const match = text.match(new RegExp(`^\\s*${key}\\s*=\\s*(\\d+)`, "m"));
  if (!match) throw new Error(`Không tìm thấy ${key} trong GameServerInfo - Event.ini`);
  return Number(match[1]);
}

function scheduleSection(text: string): { start: number; end: number; body: string } {
  const match = /^0\s*\r?\n([\s\S]*?)^end\s*(?:\r?\n|$)/m.exec(text);
  if (!match || match.index === undefined) {
    throw new Error("MossMerchant.dat không có phần lịch chạy hợp lệ");
  }
  const start = match.index;
  const end = start + match[0].length;
  return { start, end, body: match[1] };
}

function replaceScheduleSection(text: string, body: string): string {
  const section = scheduleSection(text);
  const normalizedBody = normalizeNewlines(body).replace(/(?:\r\n)+$/, "");
  const block = `0\r\n${normalizedBody}\r\nend\r\n`;
  return `${text.slice(0, section.start)}${block}${text.slice(section.end)}`;
}

function parseSchedule(text: string): MossSchedule[] {
  const { body } = scheduleSection(text);
  return body
    .split(/\r?\n/)
    .map((line) => line.trim())
    .filter((line) => line && !line.startsWith("//"))
    .map((line) => line.split(/\s+/).slice(0, 7))
    .filter((parts) => parts.length === 7 && parts.every((part) => part === "*" || /^\d+$/.test(part)))
    .map(([year, month, day, dayOfWeek, hour, minute, second]) => ({
      year,
      month,
      day,
      dayOfWeek,
      hour: Number(hour),
      minute: Number(minute),
      second: Number(second),
      isDaily: year === "*" && month === "*" && day === "*" && dayOfWeek === "*",
    }));
}

function validateDailyTimes(times: string[]): { hour: number; minute: number }[] {
  const values = [...new Set(times.map((time) => time.trim()).filter(Boolean))];
  if (!values.length || values.length > 12) {
    throw new Error("Cần từ 1 đến 12 mốc giờ chạy mỗi ngày");
  }

  return values
    .map((time) => {
      const match = /^(\d{1,2}):(\d{2})$/.exec(time);
      if (!match) throw new Error(`Giờ chạy không hợp lệ: ${time}`);
      const hour = Number(match[1]);
      const minute = Number(match[2]);
      if (hour > 23 || minute > 59) throw new Error(`Giờ chạy không hợp lệ: ${time}`);
      return { hour, minute };
    })
    .sort((a, b) => a.hour - b.hour || a.minute - b.minute);
}

async function saveFile(fullPath: string, content: string): Promise<string> {
  await fs.access(fullPath);
  const backup = await backupFile(fullPath);
  await fs.writeFile(fullPath, normalizeNewlines(content), "utf8");
  return backup;
}

export async function getMossMerchantSettings() {
  const rows = await Promise.all(
    SUB_NAMES.map(async (subName) => {
      const files = subFiles(subName);
      const [ini, data] = await Promise.all([
        fs.readFile(files.iniPath, "utf8"),
        fs.readFile(files.dataPath, "utf8"),
      ]);
      return {
        subName,
        enabled: getIniNumber(ini, "MossMerchantEvent") === 1,
        durationSeconds: getIniNumber(ini, "MossMerchantEventTime"),
        schedule: parseSchedule(data),
      };
    })
  );

  const first = rows[0];
  const dailyTimes = first.schedule
    .filter((item) => item.isDaily)
    .map((item) => `${String(item.hour).padStart(2, "0")}:${String(item.minute).padStart(2, "0")}`);

  return {
    enabled: rows.every((row) => row.enabled),
    durationSeconds: first.durationSeconds,
    dailyTimes,
    schedules: first.schedule,
    synchronized: rows.every(
      (row) =>
        row.enabled === first.enabled &&
        row.durationSeconds === first.durationSeconds
    ),
    subCount: rows.length,
  };
}

export async function saveMossMerchantSettings(input: {
  enabled: boolean;
  durationSeconds: number;
  dailyTimes: string[];
}): Promise<{ backups: string[] }> {
  if (!Number.isInteger(input.durationSeconds) || input.durationSeconds < 60 || input.durationSeconds > 86400) {
    throw new Error("Thời lượng phải từ 60 giây đến 24 giờ");
  }
  const times = validateDailyTimes(input.dailyTimes);
  const scheduleLines = times.map(
    ({ hour, minute }) => `*        *       *     *     ${String(hour).padStart(2, " ")}     ${String(minute).padStart(2, "0")}       0`
  );
  const scheduleBody = [
    "//Year   Month   Day   DoW   Hour   Minute   Second",
    ...scheduleLines,
  ].join("\r\n");

  const backups: string[] = [];
  for (const subName of SUB_NAMES) {
    const files = subFiles(subName);
    const [ini, data] = await Promise.all([
      fs.readFile(files.iniPath, "utf8"),
      fs.readFile(files.dataPath, "utf8"),
    ]);
    const nextIni = replaceIniNumber(
      replaceIniNumber(ini, "MossMerchantEvent", input.enabled ? 1 : 0),
      "MossMerchantEventTime",
      input.durationSeconds
    );
    const nextData = replaceScheduleSection(data, scheduleBody);
    backups.push(await saveFile(files.iniPath, nextIni));
    backups.push(await saveFile(files.dataPath, nextData));
  }
  return { backups };
}

export async function scheduleMossMerchantTest(delayMinutes: number): Promise<{
  scheduledAt: string;
  backups: string[];
}> {
  if (!Number.isInteger(delayMinutes) || delayMinutes < 1 || delayMinutes > 60) {
    throw new Error("Thời gian chờ để test phải từ 1 đến 60 phút");
  }

  const scheduled = new Date();
  scheduled.setSeconds(0, 0);
  scheduled.setMinutes(scheduled.getMinutes() + delayMinutes);
  const line = `${scheduled.getFullYear()}     ${scheduled.getMonth() + 1}       ${scheduled.getDate()}    *     ${String(scheduled.getHours()).padStart(2, "0")}     ${String(scheduled.getMinutes()).padStart(2, "0")}       0     ${TEST_MARKER}`;

  const backups: string[] = [];
  for (const subName of SUB_NAMES) {
    const files = subFiles(subName);
    const [ini, data] = await Promise.all([
      fs.readFile(files.iniPath, "utf8"),
      fs.readFile(files.dataPath, "utf8"),
    ]);
    const nextIni = replaceIniNumber(ini, "MossMerchantEvent", 1);
    const section = scheduleSection(data);
    const bodyWithoutOldTests = section.body
      .split(/\r?\n/)
      .filter((entry) => !entry.includes(TEST_MARKER))
      .join("\r\n")
      .replace(/\s*$/, "");
    const nextData = replaceScheduleSection(data, `${bodyWithoutOldTests}\r\n${line}`);
    backups.push(await saveFile(files.iniPath, nextIni));
    backups.push(await saveFile(files.dataPath, nextData));
  }
  return { scheduledAt: scheduled.toISOString(), backups };
}
