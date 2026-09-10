$base = if ($PSScriptRoot) { $PSScriptRoot } else { Split-Path -Parent $MyInvocation.MyCommand.Path }
$log = Join-Path $base 'watchdog.log'
$heartbeatEvery = 30
$loop = 0

function Write-Log([string]$msg) {
  $line = '{0} {1}' -f (Get-Date -Format 'yyyy-MM-dd HH:mm:ss'), $msg
  Add-Content -Path $log -Value $line -Encoding UTF8
}

function Start-OutsideJob([string]$FilePath, [string]$WorkingDirectory, [string]$Arguments = '') {
  if (-not (Test-Path -LiteralPath $FilePath)) { return $false }
  if ($Arguments) {
    $cmd = 'cmd.exe /c start "" /MIN /D "{0}" "{1}" {2}' -f $WorkingDirectory, $FilePath, $Arguments
  } else {
    $cmd = 'cmd.exe /c start "" /MIN /D "{0}" "{1}"' -f $WorkingDirectory, $FilePath
  }
  $r = Invoke-CimMethod -ClassName Win32_Process -MethodName Create -Arguments @{
    CommandLine      = $cmd
    CurrentDirectory = $WorkingDirectory
  }
  return ($r.ReturnValue -eq 0)
}

function Test-HelperRunning([string]$scriptName) {
  $full = Join-Path $base $scriptName
  $needle = '-File "{0}"' -f $full
  $needle2 = "-File '{0}'" -f $full
  $needle3 = ('-File {0}' -f $full)
  return [bool](Get-CimInstance Win32_Process -Filter "Name='powershell.exe'" | Where-Object {
    $_.CommandLine -and (
      $_.CommandLine.Contains($needle) -or
      $_.CommandLine.Contains($needle2) -or
      $_.CommandLine.Contains($needle3)
    )
  })
}

function Ensure-SqlExpress {
  $sqlService = Get-Service -Name 'MSSQL$SQLEXPRESS' -ErrorAction SilentlyContinue
  if ($sqlService -and $sqlService.Status -ne 'Running') {
    Write-Log 'Starting MSSQL$SQLEXPRESS'
    Start-Service -Name 'MSSQL$SQLEXPRESS' -ErrorAction SilentlyContinue
  }
}

function Ensure-Proc([string]$name, [string]$exe, [string]$wd) {
  if (Get-Process -Name $name -ErrorAction SilentlyContinue) { return }
  if (-not (Test-Path -LiteralPath $exe)) { return }
  Write-Log ("Restart {0}" -f $name)
  [void](Start-OutsideJob -FilePath $exe -WorkingDirectory $wd)
  Start-Sleep -Seconds 2
}

function Ensure-GameServerProc([string]$label, [string]$exe, [string]$wd) {
  if (-not (Test-Path -LiteralPath $exe)) { return }
  $existing = Get-CimInstance Win32_Process -Filter "Name='GameServer.exe'" -ErrorAction SilentlyContinue | Where-Object {
    $_.ExecutablePath -and ($_.ExecutablePath -ieq $exe)
  }
  if ($existing) { return }
  Write-Log ("Restart {0}" -f $label)
  [void](Start-OutsideJob -FilePath $exe -WorkingDirectory $wd)
  Start-Sleep -Seconds 2
}

Write-Log 'Watchdog started'
while ($true) {
  try {
    Ensure-SqlExpress
    Ensure-Proc 'DataServer'      (Join-Path $base '2.DataServer\DataServer.exe')       (Join-Path $base '2.DataServer')
    Ensure-Proc 'JoinServer'      (Join-Path $base '3.JoinServer\JoinServer.exe')       (Join-Path $base '3.JoinServer')
    Ensure-Proc 'ConnectServer'   (Join-Path $base '1.ConnectServer\ConnectServer.exe') (Join-Path $base '1.ConnectServer')
    Ensure-GameServerProc 'GameServer (Sub-1 VIP)'    (Join-Path $base '4.Sub-1\GameServer\GameServer.exe') (Join-Path $base '4.Sub-1\GameServer')
    Ensure-GameServerProc 'GameServer (Sub-2 Thường)' (Join-Path $base '4.Sub-2\GameServer\GameServer.exe') (Join-Path $base '4.Sub-2\GameServer')
    Ensure-GameServerProc 'GameServer (Sub-3 Non-PK)' (Join-Path $base '4.Sub-3\GameServer\GameServer.exe') (Join-Path $base '4.Sub-3\GameServer')
    Ensure-Proc 'StartAntiServer' (Join-Path $base '6.AntiHack\StartAntiServer.exe')    (Join-Path $base '6.AntiHack')
  } catch {
    Write-Log ("Watchdog error: {0}" -f $_.Exception.Message)
  }

  $loop++
  if (($loop % $heartbeatEvery) -eq 0) {
    $gsCount = @(Get-CimInstance Win32_Process -Filter "Name='GameServer.exe'" -EA SilentlyContinue).Count
    $other = @(
      (Get-Process DataServer -EA SilentlyContinue),
      (Get-Process JoinServer -EA SilentlyContinue),
      (Get-Process ConnectServer -EA SilentlyContinue),
      (Get-Process StartAntiServer -EA SilentlyContinue)
    ) | Where-Object { $_ }
    $totalProcs = $other.Count + $gsCount
    Write-Log ("Heartbeat OK procs={0}/7 (GS={1}/3)" -f $totalProcs, $gsCount)
  }
  Start-Sleep -Seconds 8
}
