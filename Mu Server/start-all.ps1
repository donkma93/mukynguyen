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
Write-StartLog 'SQL LocalDB...'
try {
  $localDb = Get-Command sqllocaldb -ErrorAction Stop
  & $localDb.Source start MSSQLLocalDB | Out-Null
  $state = (& $localDb.Source info MSSQLLocalDB | Select-String '^\s*State:\s*(.+)$').Matches.Groups[1].Value.Trim()
  Write-StartLog ("MSSQLLocalDB state: {0}" -f $state)
} catch {
  Write-StartLog ("LocalDB warn: {0}" -f $_.Exception.Message)
}

function Set-UserOdbcDsnLocalDb([string]$dsnName, [string]$database = 'MuThangCuoi') {
  $server = '(localdb)\MSSQLLocalDB'
  $driverName = 'ODBC Driver 17 for SQL Server'
  $driverDll64 = 'C:\Windows\System32\msodbcsql17.dll'
  $driverDll32 = 'C:\Windows\SysWOW64\msodbcsql17.dll'
  try {
    $existing = Get-OdbcDsn -Name $dsnName -DsnType User -ErrorAction SilentlyContinue
    if (-not $existing) {
      Add-OdbcDsn -Name $dsnName -DriverName $driverName -DsnType User -SetPropertyValue @(
        "Server=$server",
        "Database=$database",
        "Trusted_Connection=Yes",
        "Description=$dsnName LocalDB"
      ) -ErrorAction Stop
      Write-StartLog ("Created ODBC User DSN: {0} -> {1} ({2})" -f $dsnName, $server, $database)
    } else {
      Set-OdbcDsn -Name $dsnName -DsnType User -SetPropertyValue @(
        "Server=$server",
        "Database=$database",
        "Trusted_Connection=Yes"
      ) -ErrorAction SilentlyContinue
      Write-StartLog ("Updated ODBC User DSN: {0} -> {1} ({2})" -f $dsnName, $server, $database)
    }
  } catch {
    Write-StartLog ("ODBC check notice for {0}: {1}" -f $dsnName, $_.Exception.Message)
  }

  # DataServer/JoinServer are 32-bit; keep Wow6432Node User DSN in sync.
  try {
    $wowIni = 'HKCU:\Software\Wow6432Node\ODBC\ODBC.INI'
    $wowDsn = Join-Path $wowIni $dsnName
    $wowSources = Join-Path $wowIni 'ODBC Data Sources'
    if (-not (Test-Path $wowIni)) { New-Item $wowIni -Force | Out-Null }
    if (-not (Test-Path $wowSources)) { New-Item $wowSources -Force | Out-Null }
    if (-not (Test-Path $wowDsn)) { New-Item $wowDsn -Force | Out-Null }
    $dll = if (Test-Path $driverDll32) { $driverDll32 } else { $driverDll64 }
    New-ItemProperty -Path $wowDsn -Name 'Driver' -Value $dll -PropertyType String -Force | Out-Null
    New-ItemProperty -Path $wowDsn -Name 'Server' -Value $server -PropertyType String -Force | Out-Null
    New-ItemProperty -Path $wowDsn -Name 'Database' -Value $database -PropertyType String -Force | Out-Null
    New-ItemProperty -Path $wowDsn -Name 'Trusted_Connection' -Value 'Yes' -PropertyType String -Force | Out-Null
    New-ItemProperty -Path $wowDsn -Name 'Description' -Value "$dsnName LocalDB" -PropertyType String -Force | Out-Null
    New-ItemProperty -Path $wowSources -Name $dsnName -Value $driverName -PropertyType String -Force | Out-Null
  } catch {
    Write-StartLog ("ODBC 32-bit DSN notice for {0}: {1}" -f $dsnName, $_.Exception.Message)
  }
}

Write-StartLog 'Checking ODBC DSNs (LocalDB)...'
Set-UserOdbcDsnLocalDb 'MuThangCuoi' 'MuThangCuoi'
Set-UserOdbcDsnLocalDb 'MuOnline' 'MuThangCuoi'

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
