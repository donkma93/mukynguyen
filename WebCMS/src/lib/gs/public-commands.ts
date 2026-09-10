import {
  type AccountLevel,
  ACCOUNT_LEVELS,
  iniInt,
  memScriptDataLines,
  parseIniValues,
  ratesByAccountLevel,
  tokenizeRow,
} from "@/lib/gs/public-guide-parse";

export type AlRates = Record<AccountLevel, number | null>;

export type PublicCommandRow = {
  cmd: string;
  enabled: Record<AccountLevel, boolean>;
  money: AlRates;
  wcoinC: number | null;
  wcoinP: number | null;
  goblinPoint: number | null;
  delay: number | null;
  comment: string | null;
};

export type PublicCommandsGuide = {
  commands: PublicCommandRow[];
  reset: {
    level: AlRates;
    money: AlRates;
    startLevel: AlRates;
    autoEnable: Record<AccountLevel, boolean | null>;
    pointRate: Record<string, number | null>;
  };
  masterReset: {
    level: AlRates;
    money: AlRates;
    point: AlRates;
  };
  wareNumber: AlRates;
  marryLevel: number | null;
  marryCost: number | null;
  giftLimit: number | null;
  moveNote: "command-disabled-use-move-txt" | "command-enabled";
  moves: { map: string; minLevel: number | null; money: number | null }[];
};

/** Player-facing allowlist — never expose GM/admin cmds even if Enable=1. */
const PLAYER_CMDS = new Set(
  [
    "/post",
    "/addstr",
    "/addagi",
    "/addvit",
    "/addene",
    "/addcmd",
    "/pkclear",
    "/zen",
    "/change",
    "/reset",
    "/mreset",
    "/war",
    "/soccer",
    "/pt",
    "/ctc",
    "/xoado",
    "/marry",
    "/class",
    "/doiten",
    "/nhanqua",
    "/quest",
    "/info",
    "/bau",
    "/homdochung",
    "/move",
  ].map((c) => c.toLowerCase())
);

export function emptyPublicCommandsGuide(): PublicCommandsGuide {
  const emptyAl = (): AlRates => ({ 0: null, 1: null, 2: null, 3: null });
  const emptyBool = (): Record<AccountLevel, boolean | null> => ({
    0: null,
    1: null,
    2: null,
    3: null,
  });
  return {
    commands: [],
    reset: {
      level: emptyAl(),
      money: emptyAl(),
      startLevel: emptyAl(),
      autoEnable: emptyBool(),
      pointRate: {},
    },
    masterReset: {
      level: emptyAl(),
      money: emptyAl(),
      point: emptyAl(),
    },
    wareNumber: emptyAl(),
    marryLevel: null,
    marryCost: null,
    giftLimit: null,
    moveNote: "command-disabled-use-move-txt",
    moves: [],
  };
}

function parseCommandTxt(text: string): PublicCommandRow[] {
  const rows: PublicCommandRow[] = [];
  for (const line of memScriptDataLines(text)) {
    if (line.startsWith("//")) continue;
    const t = tokenizeRow(line);
    // Index Command Enable0-3 Money0-3 MinLv0-3 MaxLv0-3 MinRe0-3 MaxRe0-3 Delay GM WcoinC WcoinP Goblin
    if (t.length < 28) continue;
    const cmd = String(t[1] ?? "").trim();
    if (!cmd.startsWith("/")) continue;
    if (!PLAYER_CMDS.has(cmd.toLowerCase())) continue;

    const enabled = {
      0: t[2] === "1",
      1: t[3] === "1",
      2: t[4] === "1",
      3: t[5] === "1",
    } as Record<AccountLevel, boolean>;

    const money: AlRates = {
      0: Number.isFinite(Number(t[6])) ? Number(t[6]) : null,
      1: Number.isFinite(Number(t[7])) ? Number(t[7]) : null,
      2: Number.isFinite(Number(t[8])) ? Number(t[8]) : null,
      3: Number.isFinite(Number(t[9])) ? Number(t[9]) : null,
    };

    // After MaxReset (cols 10..25 = 16 fields for min/max level/reset), index:
    // 2-5 enable, 6-9 money, 10-13 minLv, 14-17 maxLv, 18-21 minRe, 22-25 maxRe,
    // 26 delay, 27 GM, 28 WcoinC, 29 WcoinP, 30 Goblin
    const delay = Number.isFinite(Number(t[26])) ? Number(t[26]) : null;
    const wcoinC = Number.isFinite(Number(t[28])) ? Number(t[28]) : null;
    const wcoinP = Number.isFinite(Number(t[29])) ? Number(t[29]) : null;
    const goblinPoint = Number.isFinite(Number(t[30])) ? Number(t[30]) : null;

    const commentMatch = line.match(/\/\/\s*(?:<!--)?\/*\s*(.+?)(?:-->)?\s*$/);
    let comment = commentMatch ? commentMatch[1].trim() : null;
    if (comment) {
      comment = comment.replace(/^\/+/, "").replace(/-->$/, "").trim();
    }

    rows.push({
      cmd,
      enabled,
      money,
      wcoinC,
      wcoinP,
      goblinPoint,
      delay,
      comment,
    });
  }
  return rows;
}

function parseMoveTxt(text: string): PublicCommandsGuide["moves"] {
  const moves: PublicCommandsGuide["moves"] = [];
  for (const line of memScriptDataLines(text)) {
    if (line.startsWith("//")) continue;
    const t = tokenizeRow(line);
    // Index Name RequireMoney MinLevel MaxLevel MinReset MaxReset AccountLevel Gate
    if (t.length < 4) continue;
    if (!/^-?\d+$/.test(t[0])) continue;
    const name = t[1];
    const money = Number(t[2]);
    const minLevel = Number(t[3]);
    if (!name || /^-?\d+$/.test(name)) continue;
    moves.push({
      map: name,
      minLevel: Number.isFinite(minLevel) ? minLevel : null,
      money: Number.isFinite(money) ? money : null,
    });
  }
  // Prefer unique map names; keep first occurrence
  const seen = new Set<string>();
  const unique: PublicCommandsGuide["moves"] = [];
  for (const m of moves) {
    const key = m.map.toLowerCase();
    if (seen.has(key)) continue;
    seen.add(key);
    unique.push(m);
    if (unique.length >= 20) break;
  }
  return unique;
}

export function buildPublicCommandsGuide(input: {
  commandIniText: string;
  commandTxtText: string;
  moveTxtText: string | null;
}): PublicCommandsGuide {
  const ini = parseIniValues(input.commandIniText);
  const commands = parseCommandTxt(input.commandTxtText);
  const moveCmd = commands.find((c) => c.cmd.toLowerCase() === "/move");
  const moveEnabled = moveCmd
    ? ACCOUNT_LEVELS.some((al) => moveCmd.enabled[al])
    : false;

  const boolAl = (prefix: string): Record<AccountLevel, boolean | null> => ({
    0: iniInt(ini[`${prefix}_AL0`]) == null ? null : iniInt(ini[`${prefix}_AL0`]) !== 0,
    1: iniInt(ini[`${prefix}_AL1`]) == null ? null : iniInt(ini[`${prefix}_AL1`]) !== 0,
    2: iniInt(ini[`${prefix}_AL2`]) == null ? null : iniInt(ini[`${prefix}_AL2`]) !== 0,
    3: iniInt(ini[`${prefix}_AL3`]) == null ? null : iniInt(ini[`${prefix}_AL3`]) !== 0,
  });

  return {
    commands,
    reset: {
      level: ratesByAccountLevel(ini, "CommandResetLevel"),
      money: ratesByAccountLevel(ini, "CommandResetMoney"),
      startLevel: ratesByAccountLevel(ini, "CommandResetStartLevel"),
      autoEnable: boolAl("CommandResetAutoEnable"),
      pointRate: {
        DW: iniInt(ini.CommandResetPointRateDW),
        DK: iniInt(ini.CommandResetPointRateDK),
        FE: iniInt(ini.CommandResetPointRateFE),
        MG: iniInt(ini.CommandResetPointRateMG),
        DL: iniInt(ini.CommandResetPointRateDL),
        SU: iniInt(ini.CommandResetPointRateSU),
        RF: iniInt(ini.CommandResetPointRateRF),
      },
    },
    masterReset: {
      level: ratesByAccountLevel(ini, "CommandMasterResetLevel"),
      money: ratesByAccountLevel(ini, "CommandMasterResetMoney"),
      point: ratesByAccountLevel(ini, "CommandMasterResetPoint"),
    },
    wareNumber: ratesByAccountLevel(ini, "CommandWareNumber"),
    marryLevel: iniInt(ini.CommandMarryLevel),
    marryCost: iniInt(ini.CommandMarryCost),
    giftLimit: iniInt(ini.CommandGiftLimit),
    moveNote: moveEnabled
      ? "command-enabled"
      : "command-disabled-use-move-txt",
    moves: input.moveTxtText ? parseMoveTxt(input.moveTxtText) : [],
  };
}
