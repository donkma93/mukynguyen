# Starts Mu ThangCuoi servers OUTSIDE the current console Job Object
# so they are not killed when the parent shell/agent exits.
$ErrorActionPreference = 'Stop'
$base = if ($PSScriptRoot) { $PSScriptRoot } else { Split-Path -Parent $MyInvocation.MyCommand.Path }
$log = Join-Path $base 'start-all.log'

function Write-StartLog([string]$msg) {
  $line = '{0} {1}' -f (Get-Date -Format 'yyyy-MM-dd HH:mm:ss'), $msg
  Add-Content -Path $log -Value $line -Encoding UTF8
  Write-Host $line
}

function Start-OutsideJob {
  param(
    [Parameter(Mandatory)] [string]$FilePath,
    [string]$WorkingDirectory = '',
    [string]$Arguments = '',
    [ValidateSet('Normal', 'Minimized', 'Hidden')] [string]$WindowStyle = 'Minimized'
  )
  if (-not (Test-Path -LiteralPath $FilePath)) {
    throw "Missing: $FilePath"
  }
  if (-not $WorkingDirectory) {
    $WorkingDirectory = Split-Path -Parent $FilePath
  }

  # WMI Create runs in a fresh session outside the caller's Job Object.
  $cmd = if ($Arguments) {
    'cmd.exe /c start "" /MIN /D "{0}" "{1}" {2}' -f $WorkingDirectory, $FilePath, $Arguments
  } else {
    'cmd.exe /c start "" /MIN /D "{0}" "{1}"' -f $WorkingDirectory, $FilePath
  }
  $r = Invoke-CimMethod -ClassName Win32_Process -MethodName Create -Arguments @{
    CommandLine      = $cmd
    CurrentDirectory = $WorkingDirectory
  }
  if ($r.ReturnValue -ne 0) {
    throw ("Win32_Process.Create failed ({0}) for {1}" -f $r.ReturnValue, $FilePath)
  }
  return $r.ProcessId
}

function Test-Proc([string]$name) {
  return [bool](Get-Process -Name $name -ErrorAction SilentlyContinue)
}

function Ensure-Mu([string]$name, [string]$exe, [string]$wd) {
  if (Test-Proc $name) {
    Write-StartLog "$name already running"
    return
  }
  Write-StartLog "Starting $name"
  [void](Start-OutsideJob -FilePath $exe -WorkingDirectory $wd)
}

function Test-HelperRunning([string]$scriptName) {
  $needle = '-File "{0}"' -f (Join-Path $base $scriptName)
  $needle2 = "-File '{0}'" -f (Join-Path $base $scriptName)
  $needle3 = ('-File {0}' -f (Join-Path $base $scriptName))
  return [bool](Get-CimInstance Win32_Process -Filter "Name='powershell.exe'" | Where-Object {
    $_.CommandLine -and (
      $_.CommandLine.Contains($needle) -or
      $_.CommandLine.Contains($needle2) -or
      $_.CommandLine.Contains($needle3)
    )
  })
}

function Ensure-Helper([string]$scriptName) {
  $path = Join-Path $base $scriptName
  if (Test-HelperRunning $scriptName) {
    Write-StartLog "$scriptName already running"
    return
  }
  Write-StartLog "Starting $scriptName"
  $ps = Join-Path $env:SystemRoot 'System32\WindowsPowerShell\v1.0\powershell.exe'
  $args = '-NoProfile -ExecutionPolicy Bypass -WindowStyle Minimized -File "{0}"' -f $path
  $r = Invoke-CimMethod -ClassName Win32_Process -MethodName Create -Arguments @{
    CommandLine      = ('"{0}" {1}' -f $ps, $args)
    CurrentDirectory = $base
  }
  if ($r.ReturnValue -ne 0) {
    throw ("Failed to start {0}: {1}" -f $scriptName, $r.ReturnValue)
  }
}

Write-StartLog '==> Mu ThangCuoi start-all'
Write-StartLog 'Checking MuThangCuoi on MSSQLLocalDB and refreshing ODBC DSNs...'
& (Join-Path $base 'initialize-localdb-odbc.ps1')
Write-StartLog 'LocalDB connection verified; ODBC DSNs point to the current instance pipe.'

Start-Sleep -Seconds 1

function Ensure-GameServer([string]$label, [string]$exe, [string]$wd) {
  $existing = Get-CimInstance Win32_Process -Filter "Name='GameServer.exe'" -ErrorAction SilentlyContinue | Where-Object {
    $_.ExecutablePath -and ($_.ExecutablePath -ieq $exe)
  }
  if ($existing) {
    Write-StartLog "$label already running"
    return
  }
  Write-StartLog "Starting $label"
  [void](Start-OutsideJob -FilePath $exe -WorkingDirectory $wd)
}

Ensure-Mu 'DataServer'      (Join-Path $base '2.DataServer\DataServer.exe')       (Join-Path $base '2.DataServer')
Start-Sleep -Seconds 3
Ensure-Mu 'JoinServer'      (Join-Path $base '3.JoinServer\JoinServer.exe')       (Join-Path $base '3.JoinServer')
Start-Sleep -Seconds 2
Ensure-Mu 'ConnectServer'   (Join-Path $base '1.ConnectServer\ConnectServer.exe') (Join-Path $base '1.ConnectServer')
Start-Sleep -Seconds 2
Ensure-GameServer 'GameServer (Server 1 Boss Events)'    (Join-Path $base '4.Sub-1\GameServer\GameServer.exe') (Join-Path $base '4.Sub-1\GameServer')
Start-Sleep -Seconds 2
Ensure-GameServer 'GameServer (Server 2 Thường)' (Join-Path $base '4.Sub-2\GameServer\GameServer.exe') (Join-Path $base '4.Sub-2\GameServer')
Start-Sleep -Seconds 2
Ensure-GameServer 'GameServer (Server 3 VIP)' (Join-Path $base '4.Sub-3\GameServer\GameServer.exe') (Join-Path $base '4.Sub-3\GameServer')
Start-Sleep -Seconds 2
Ensure-GameServer 'GameServer (Server 4 Reset 0-190)' (Join-Path $base '4.Sub-4\GameServer\GameServer.exe') (Join-Path $base '4.Sub-4\GameServer')
Start-Sleep -Seconds 2
Ensure-GameServer 'GameServer (Server 5 Non-PvP)' (Join-Path $base '4.Sub-5\GameServer\GameServer.exe') (Join-Path $base '4.Sub-5\GameServer')
Start-Sleep -Seconds 2
Ensure-Mu 'StartAntiServer' (Join-Path $base '6.AntiHack\StartAntiServer.exe')    (Join-Path $base '6.AntiHack')

Ensure-Helper 'watchdog-mu-server.ps1'
Start-Sleep -Seconds 2

Write-StartLog 'Status:'
Get-Process DataServer, JoinServer, ConnectServer, GameServer, StartAntiServer -ErrorAction SilentlyContinue |
  Format-Table ProcessName, Id, StartTime -AutoSize | Out-String | ForEach-Object { Write-StartLog $_.TrimEnd() }

Write-StartLog 'Client: 127.0.0.2:44405 (5 servers: Boss Events, Thường, VIP, Reset 0-190, Non-PvP)'
