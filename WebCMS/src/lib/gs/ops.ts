import { execFile } from "child_process";
import fs from "fs/promises";
import path from "path";
import { promisify } from "util";
import { getMuServerRoot } from "@/lib/gs/paths";

const execFileAsync = promisify(execFile);

export type ProcessStatus = {
  name: string;
  running: boolean;
  pid: number | null;
  startTime: string | null;
};

export type OpsStatus = {
  opsEnabled: boolean;
  processes: ProcessStatus[];
  onlineHint: string;
};

function opsEnabled(): boolean {
  const v = (process.env.GS_OPS_ENABLED || "true").trim().toLowerCase();
  return v !== "0" && v !== "false" && v !== "no";
}

async function queryProcesses(
  names: string[]
): Promise<Record<string, { pid: number; startTime: string | null }>> {
  const ps = `
$names = @(${names.map((n) => `'${n}'`).join(",")})
Get-Process -Name $names -ErrorAction SilentlyContinue |
  Select-Object ProcessName, Id, StartTime |
  ConvertTo-Json -Compress
`.trim();

  try {
    const { stdout } = await execFileAsync(
      "powershell.exe",
      ["-NoProfile", "-ExecutionPolicy", "Bypass", "-Command", ps],
      { windowsHide: true, timeout: 15000, maxBuffer: 1024 * 1024 }
    );
    const raw = stdout.trim();
    if (!raw) return {};
    const parsed = JSON.parse(raw) as
      | { ProcessName: string; Id: number; StartTime?: string }
      | { ProcessName: string; Id: number; StartTime?: string }[];
    const list = Array.isArray(parsed) ? parsed : [parsed];
    const map: Record<string, { pid: number; startTime: string | null }> = {};
    for (const row of list) {
      map[row.ProcessName] = {
        pid: Number(row.Id),
        startTime: row.StartTime ? new Date(row.StartTime).toISOString() : null,
      };
    }
    return map;
  } catch {
    return {};
  }
}

export async function getOpsStatus(): Promise<OpsStatus> {
  const names = [
    "GameServer",
    "DataServer",
    "JoinServer",
    "ConnectServer",
    "StartAntiServer",
  ];
  const found = await queryProcesses(names);
  return {
    opsEnabled: opsEnabled(),
    processes: names.map((name) => ({
      name,
      running: Boolean(found[name]),
      pid: found[name]?.pid ?? null,
      startTime: found[name]?.startTime ?? null,
    })),
    onlineHint: `${(process.env.NEXT_PUBLIC_SERVER_IP || "127.0.0.2").trim()}:${(process.env.NEXT_PUBLIC_SERVER_PORT || "44405").trim()}`,
  };
}

async function stopProcessByName(name: string): Promise<void> {
  const ps = `
Get-Process -Name '${name}' -ErrorAction SilentlyContinue | ForEach-Object {
  Stop-Process -Id $_.Id -Force -ErrorAction SilentlyContinue
}
Start-Sleep -Milliseconds 500
`.trim();
  await execFileAsync(
    "powershell.exe",
    ["-NoProfile", "-ExecutionPolicy", "Bypass", "-Command", ps],
    { windowsHide: true, timeout: 30000 }
  );
}

async function runMuScript(scriptName: string, timeoutMs = 120000): Promise<string> {
  const scriptPath = path.join(getMuServerRoot(), scriptName);
  await fs.access(scriptPath);
  const { stdout, stderr } = await execFileAsync(
    "powershell.exe",
    [
      "-NoProfile",
      "-ExecutionPolicy",
      "Bypass",
      "-File",
      scriptPath,
    ],
    { windowsHide: true, timeout: timeoutMs, maxBuffer: 2 * 1024 * 1024 }
  );
  return `${stdout || ""}${stderr ? `\n${stderr}` : ""}`.trim();
}

/** Menu command: Reload → Character (GameServer.cpp / resource.h). */
const IDM_RELOAD_RELOADCHARACTER = 32778;
const WM_COMMAND = 0x0111;

/**
 * Ask the running GameServer window to reload Character.ini in-memory
 * (same as menu Reload → Character). No process restart.
 */
export async function reloadCharacterIni(): Promise<{
  ok: boolean;
  message: string;
}> {
  if (!opsEnabled()) {
    return { ok: false, message: "GS ops đang tắt (GS_OPS_ENABLED=false)" };
  }

  const ps = `
$ErrorActionPreference = 'Stop'
Add-Type @"
using System;
using System.Text;
using System.Runtime.InteropServices;
public static class GsReload {
  public delegate bool EnumProc(IntPtr hWnd, IntPtr lParam);
  [DllImport("user32.dll")] public static extern bool EnumWindows(EnumProc cb, IntPtr lp);
  [DllImport("user32.dll")] public static extern uint GetWindowThreadProcessId(IntPtr hWnd, out uint pid);
  [DllImport("user32.dll", CharSet=CharSet.Auto)] public static extern int GetClassName(IntPtr hWnd, StringBuilder sb, int n);
  [DllImport("user32.dll")] public static extern bool PostMessage(IntPtr hWnd, uint Msg, IntPtr wParam, IntPtr lParam);
}
"@
$proc = Get-Process -Name 'GameServer' -ErrorAction SilentlyContinue | Select-Object -First 1
if (-not $proc) { Write-Output 'NO_PROCESS'; exit 0 }
$targetPid = [uint32]$proc.Id
$script:hwnd = [IntPtr]::Zero
[GsReload]::EnumWindows({
  param($h, $l)
  $pidOut = [uint32]0
  [void][GsReload]::GetWindowThreadProcessId($h, [ref]$pidOut)
  if ($pidOut -ne $targetPid) { return $true }
  $cls = New-Object System.Text.StringBuilder 256
  [void][GsReload]::GetClassName($h, $cls, 256)
  if ($cls.ToString() -eq 'GAMESERVER') {
    $script:hwnd = $h
    return $false
  }
  return $true
}, [IntPtr]::Zero) | Out-Null
if ($script:hwnd -eq [IntPtr]::Zero) { Write-Output 'NO_WINDOW'; exit 0 }
$ok = [GsReload]::PostMessage($script:hwnd, ${WM_COMMAND}, [IntPtr]${IDM_RELOAD_RELOADCHARACTER}, [IntPtr]::Zero)
if ($ok) { Write-Output 'OK' } else { Write-Output 'POST_FAILED' }
`.trim();

  try {
    const { stdout } = await execFileAsync(
      "powershell.exe",
      ["-NoProfile", "-ExecutionPolicy", "Bypass", "-Command", ps],
      { windowsHide: true, timeout: 15000, maxBuffer: 256 * 1024 }
    );
    const code = (stdout || "").trim().split(/\r?\n/).pop() || "";
    if (code === "OK") {
      return {
        ok: true,
        message: "Đã gửi Reload Character tới GameServer (áp dụng trong game).",
      };
    }
    if (code === "NO_PROCESS") {
      return { ok: false, message: "GameServer chưa chạy — lưu file OK, cần Start GS." };
    }
    if (code === "NO_WINDOW") {
      return {
        ok: false,
        message: "Không tìm thấy cửa sổ GameServer để reload — thử Restart GS.",
      };
    }
    return { ok: false, message: `Reload Character thất bại (${code || "unknown"}).` };
  } catch {
    return {
      ok: false,
      message: "Reload Character thất bại",
    };
  }
}

export type RestartScope = "gameserver" | "all";

export async function restartServers(scope: RestartScope): Promise<{
  scope: RestartScope;
  log: string;
  status: OpsStatus;
}> {
  if (!opsEnabled()) {
    throw new Error("GS ops đang tắt (GS_OPS_ENABLED=false)");
  }

  const lines: string[] = [];
  if (scope === "gameserver") {
    lines.push("Stopping all GameServer processes...");
    await stopProcessByName("GameServer");
    lines.push("Running start-all.ps1 to start every GameServer...");
    lines.push(await runMuScript("start-all.ps1", 180000));
  } else {
    lines.push("Running stop-all.ps1...");
    try {
      lines.push(await runMuScript("stop-all.ps1", 90000));
    } catch (e) {
      lines.push(
        e instanceof Error ? `stop-all warn: ${e.message}` : "stop-all warn"
      );
    }
    await new Promise((r) => setTimeout(r, 1500));
    lines.push("Running start-all.ps1...");
    lines.push(await runMuScript("start-all.ps1", 180000));
  }

  const status = await getOpsStatus();
  return { scope, log: lines.filter(Boolean).join("\n"), status };
}
