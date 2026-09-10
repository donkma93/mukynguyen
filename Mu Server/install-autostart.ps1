# Registers a logon Scheduled Task via PowerShell API (handles spaces in path).
$ErrorActionPreference = 'Stop'
$base = if ($PSScriptRoot) { $PSScriptRoot } else { Split-Path -Parent $MyInvocation.MyCommand.Path }
$taskName = 'MuThangCuoi-StartServers'
$ps = Join-Path $env:SystemRoot 'System32\WindowsPowerShell\v1.0\powershell.exe'
$startAll = Join-Path $base 'start-all.ps1'
$userId = if ($env:USERDOMAIN) { "$env:USERDOMAIN\$env:USERNAME" } else { $env:USERNAME }

Write-Host "Register scheduled task: $taskName"
Write-Host "User: $userId"
Write-Host "Script: $startAll"

$action = New-ScheduledTaskAction -Execute $ps -Argument "-NoProfile -ExecutionPolicy Bypass -WindowStyle Minimized -File `"$startAll`"" -WorkingDirectory $base
$trigger = New-ScheduledTaskTrigger -AtLogOn
$settings = New-ScheduledTaskSettingsSet -AllowStartIfOnBatteries -DontStopIfGoingOnBatteries -StartWhenAvailable -ExecutionTimeLimit ([TimeSpan]::Zero)
$principal = New-ScheduledTaskPrincipal -UserId $userId -LogonType Interactive -RunLevel Limited

$taskOk = $false
try {
  Register-ScheduledTask -TaskName $taskName -Action $action -Trigger $trigger -Settings $settings -Principal $principal -Force | Out-Null
  Start-ScheduledTask -TaskName $taskName
  $taskOk = $true
  $info = Get-ScheduledTaskInfo -TaskName $taskName
  Write-Host ("ScheduledTask OK. LastTaskResult={0} LastRunTime={1}" -f $info.LastTaskResult, $info.LastRunTime)
} catch {
  Write-Host ("ScheduledTask skipped (need Admin?): {0}" -f $_.Exception.Message) -ForegroundColor DarkYellow
}

# Startup-folder shortcut — works without elevation
$startup = [Environment]::GetFolderPath('Startup')
$lnkPath = Join-Path $startup 'MuThangCuoi-StartServers.lnk'
$w = New-Object -ComObject WScript.Shell
$lnk = $w.CreateShortcut($lnkPath)
$lnk.TargetPath = $ps
$lnk.Arguments = "-NoProfile -ExecutionPolicy Bypass -WindowStyle Minimized -File `"$startAll`""
$lnk.WorkingDirectory = $base
$lnk.WindowStyle = 7
$lnk.Description = 'Start Mu ThangCuoi servers + watchdog'
$lnk.Save()
Write-Host "Startup shortcut: $lnkPath"

if ($taskOk) {
  Write-Host "Manual start: Start-ScheduledTask -TaskName $taskName"
}
Write-Host "Manual start: `"$base\start-all.ps1`""