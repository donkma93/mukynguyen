# Stop Mu ThangCuoi servers + helper scripts (watchdog / keep-localdb).
$ErrorActionPreference = 'Continue'
$base = if ($PSScriptRoot) { $PSScriptRoot } else { Split-Path -Parent $MyInvocation.MyCommand.Path }
$log = Join-Path $base 'start-all.log'

function Write-StopLog([string]$msg) {
  $line = '{0} {1}' -f (Get-Date -Format 'yyyy-MM-dd HH:mm:ss'), $msg
  Add-Content -Path $log -Value $line -Encoding UTF8
  Write-Host $line
}

Write-StopLog '==> Mu ThangCuoi stop-all'

# Stop helpers first so watchdog cannot restart servers.
$helperNeedles = @(
  'watchdog-mu-server.ps1',
  'keep-localdb-alive.ps1',
  'start-all.ps1'
)
Get-CimInstance Win32_Process -Filter "Name='powershell.exe'" | ForEach-Object {
  $cmd = $_.CommandLine
  if (-not $cmd) { return }
  $hit = $false
  foreach ($n in $helperNeedles) {
    if ($cmd.Contains($n)) { $hit = $true; break }
  }
  if (-not $hit) { return }
  Write-StopLog ("Stopping helper PID {0}" -f $_.ProcessId)
  Stop-Process -Id $_.ProcessId -Force -ErrorAction SilentlyContinue
}

# GameServer first, then the rest.
$order = @('GameServer', 'StartAntiServer', 'ConnectServer', 'JoinServer', 'DataServer')
foreach ($name in $order) {
  Get-Process -Name $name -ErrorAction SilentlyContinue | ForEach-Object {
    Write-StopLog ("Stopping {0} PID {1}" -f $name, $_.Id)
    Stop-Process -Id $_.Id -Force -ErrorAction SilentlyContinue
  }
}

Start-Sleep -Seconds 1

$left = Get-Process DataServer, JoinServer, ConnectServer, GameServer, StartAntiServer -ErrorAction SilentlyContinue
if ($left) {
  Write-StopLog 'Still running:'
  $left | ForEach-Object { Write-StopLog ("  {0} PID {1}" -f $_.ProcessName, $_.Id) }
} else {
  Write-StopLog 'All Mu server processes stopped.'
}
