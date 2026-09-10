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
Write-StartLog 'SQL Server Express...'
try {
  $sqlService = Get-Service -Name 'MSSQL$SQLEXPRESS' -ErrorAction Stop
  if ($sqlService.Status -ne 'Running') {
    Start-Service -Name 'MSSQL$SQLEXPRESS' -ErrorAction Stop
    $sqlService.WaitForStatus('Running', [TimeSpan]::FromSeconds(20))
  }
  Write-StartLog 'MSSQL$SQLEXPRESS is running'
} catch {
  Write-StartLog ("SQL Server Express warn: {0}" -f $_.Exception.Message)
}

function Ensure-OdbcDsn([string]$dsnName, [string]$database = 'MuThangCuoi', [string]$server = '.\SQLEXPRESS') {
  try {
    $existing = Get-OdbcDsn -Name $dsnName -DsnType User -ErrorAction SilentlyContinue
    if (-not $existing) {
      Add-OdbcDsn -Name $dsnName -DriverName 'SQL Server' -DsnType User -SetPropertyValue @("Server=$server", "Database=$database", "Trusted_Connection=Yes", "Description=$dsnName") -ErrorAction SilentlyContinue
      Write-StartLog ("Created ODBC User DSN: {0} -> {1} ({2})" -f $dsnName, $server, $database)
    } else {
      Set-OdbcDsn -Name $dsnName -DsnType User -SetPropertyValue @("Server=$server", "Database=$database", "Trusted_Connection=Yes") -ErrorAction SilentlyContinue
    }
  } catch {
    Write-StartLog ("ODBC check notice for {0}: {1}" -f $dsnName, $_.Exception.Message)
  }
}

Write-StartLog 'Checking ODBC DSNs...'
Ensure-OdbcDsn 'MuThangCuoi' 'MuThangCuoi' '.\SQLEXPRESS'
Ensure-OdbcDsn 'MuOnline' 'MuThangCuoi' '.\SQLEXPRESS'

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
Ensure-GameServer 'GameServer (Sub-1 VIP)'    (Join-Path $base '4.Sub-1\GameServer\GameServer.exe') (Join-Path $base '4.Sub-1\GameServer')
Start-Sleep -Seconds 2
Ensure-GameServer 'GameServer (Sub-2 Thường)' (Join-Path $base '4.Sub-2\GameServer\GameServer.exe') (Join-Path $base '4.Sub-2\GameServer')
Start-Sleep -Seconds 2
Ensure-GameServer 'GameServer (Sub-3 Non-PK)' (Join-Path $base '4.Sub-3\GameServer\GameServer.exe') (Join-Path $base '4.Sub-3\GameServer')
Start-Sleep -Seconds 2
Ensure-Mu 'StartAntiServer' (Join-Path $base '6.AntiHack\StartAntiServer.exe')    (Join-Path $base '6.AntiHack')

Ensure-Helper 'watchdog-mu-server.ps1'
Start-Sleep -Seconds 2

Write-StartLog 'Status:'
Get-Process DataServer, JoinServer, ConnectServer, GameServer, StartAntiServer -ErrorAction SilentlyContinue |
  Format-Table ProcessName, Id, StartTime -AutoSize | Out-String | ForEach-Object { Write-StartLog $_.TrimEnd() }

Write-StartLog 'Client: 127.0.0.2:44405 (3 Sub Servers: Sub-1 VIP, Sub-2 Thường, Sub-3 Non-PK)'
