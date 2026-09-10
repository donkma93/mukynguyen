# Mu ThangCuoi Server Manager — GUI Start / Stop / Status
# Double-click MuServerManager.bat or this file (via the .bat wrapper).
Add-Type -AssemblyName System.Windows.Forms
Add-Type -AssemblyName System.Drawing
[System.Windows.Forms.Application]::EnableVisualStyles()

$ErrorActionPreference = 'Continue'
$base = if ($PSScriptRoot) { $PSScriptRoot } else { Split-Path -Parent $MyInvocation.MyCommand.Path }
$startAll = Join-Path $base 'start-all.ps1'
$stopAll  = Join-Path $base 'stop-all.ps1'
$psExe    = Join-Path $env:SystemRoot 'System32\WindowsPowerShell\v1.0\powershell.exe'

$serverDefs = @(
  @{ Name = 'DataServer';         Exe = (Join-Path $base '2.DataServer\DataServer.exe') },
  @{ Name = 'JoinServer';         Exe = (Join-Path $base '3.JoinServer\JoinServer.exe') },
  @{ Name = 'ConnectServer';      Exe = (Join-Path $base '1.ConnectServer\ConnectServer.exe') },
  @{ Name = 'Sub-1 (VIP)';        Exe = (Join-Path $base '4.Sub-1\GameServer\GameServer.exe') },
  @{ Name = 'Sub-2 (Thường)';     Exe = (Join-Path $base '4.Sub-2\GameServer\GameServer.exe') },
  @{ Name = 'Sub-3 (Non-PK)';     Exe = (Join-Path $base '4.Sub-3\GameServer\GameServer.exe') },
  @{ Name = 'StartAntiServer';    Exe = (Join-Path $base '6.AntiHack\StartAntiServer.exe') }
)

function Test-HelperRunning([string]$scriptName) {
  $path = Join-Path $base $scriptName
  return [bool](Get-CimInstance Win32_Process -Filter "Name='powershell.exe'" | Where-Object {
    $_.CommandLine -and $_.CommandLine.Contains($scriptName) -and $_.CommandLine.Contains($base)
  })
}

function Get-StatusRows {
  $rows = @()
  foreach ($s in $serverDefs) {
    $p = Get-CimInstance Win32_Process -ErrorAction SilentlyContinue | Where-Object {
      $_.ExecutablePath -and ($_.ExecutablePath -ieq $s.Exe)
    } | Select-Object -First 1
    $rows += [pscustomobject]@{
      Service = $s.Name
      Status  = if ($p) { 'RUNNING' } else { 'STOPPED' }
      PID     = if ($p) { $p.ProcessId } else { '-' }
      Started = if ($p -and $p.CreationDate) {
        try { [System.Management.ManagementDateTimeConverter]::ToDateTime($p.CreationDate).ToString('HH:mm:ss') } catch { 'RUNNING' }
      } else { '-' }
    }
  }
  $rows += [pscustomobject]@{
    Service = 'watchdog'
    Status  = if (Test-HelperRunning 'watchdog-mu-server.ps1') { 'RUNNING' } else { 'STOPPED' }
    PID     = '-'
    Started = '-'
  }
  $rows += [pscustomobject]@{
    Service = 'keep-localdb'
    Status  = if (Test-HelperRunning 'keep-localdb-alive.ps1') { 'RUNNING' } else { 'STOPPED' }
    PID     = '-'
    Started = '-'
  }
  return $rows
}

function Write-UiLog([string]$msg) {
  $line = '[{0}] {1}' -f (Get-Date -Format 'HH:mm:ss'), $msg
  if ($txtLog.Text.Length -gt 12000) {
    $txtLog.Text = $txtLog.Text.Substring($txtLog.Text.Length - 8000)
  }
  $txtLog.AppendText($line + [Environment]::NewLine)
}

function Refresh-Status {
  $grid.Rows.Clear()
  $running = 0
  foreach ($r in (Get-StatusRows)) {
    $idx = $grid.Rows.Add($r.Service, $r.Status, $r.PID, $r.Started)
    if ($r.Status -eq 'RUNNING') {
      $running++
      $grid.Rows[$idx].DefaultCellStyle.ForeColor = [System.Drawing.Color]::DarkGreen
    } else {
      $grid.Rows[$idx].DefaultCellStyle.ForeColor = [System.Drawing.Color]::Firebrick
    }
  }
  $lblSummary.Text = "Dang chay: $running / $($serverDefs.Count + 2)  |  Client: 127.0.0.2:44405"
}

function Invoke-ScriptOutside([string]$scriptPath, [string]$label) {
  if (-not (Test-Path -LiteralPath $scriptPath)) {
    [System.Windows.Forms.MessageBox]::Show("Khong tim thay:`n$scriptPath", 'Loi', 'OK', 'Error') | Out-Null
    return
  }
  Write-UiLog "$label..."
  $args = '-NoProfile -ExecutionPolicy Bypass -File "{0}"' -f $scriptPath
  $r = Invoke-CimMethod -ClassName Win32_Process -MethodName Create -Arguments @{
    CommandLine      = ('"{0}" {1}' -f $psExe, $args)
    CurrentDirectory = $base
  }
  if ($r.ReturnValue -ne 0) {
    Write-UiLog ("Loi chay {0}: code {1}" -f $label, $r.ReturnValue)
    return
  }
  Write-UiLog ("Da goi {0} (PID wrapper {1})" -f $label, $r.ProcessId)
}

# --- UI ---
$form = New-Object System.Windows.Forms.Form
$form.Text = 'MU Ky Nguyen — Server Manager'
$form.Size = New-Object System.Drawing.Size(640, 520)
$form.StartPosition = 'CenterScreen'
$form.FormBorderStyle = 'FixedDialog'
$form.MaximizeBox = $false
$form.MinimizeBox = $true
$form.Font = New-Object System.Drawing.Font('Segoe UI', 9)

$lblTitle = New-Object System.Windows.Forms.Label
$lblTitle.Text = 'Quan ly server Mu ThangCuoi'
$lblTitle.Font = New-Object System.Drawing.Font('Segoe UI', 12, [System.Drawing.FontStyle]::Bold)
$lblTitle.Location = New-Object System.Drawing.Point(16, 12)
$lblTitle.AutoSize = $true
$form.Controls.Add($lblTitle)

$lblSummary = New-Object System.Windows.Forms.Label
$lblSummary.Location = New-Object System.Drawing.Point(18, 42)
$lblSummary.Size = New-Object System.Drawing.Size(580, 20)
$form.Controls.Add($lblSummary)

$grid = New-Object System.Windows.Forms.DataGridView
$grid.Location = New-Object System.Drawing.Point(18, 68)
$grid.Size = New-Object System.Drawing.Size(586, 200)
$grid.ReadOnly = $true
$grid.AllowUserToAddRows = $false
$grid.AllowUserToDeleteRows = $false
$grid.AllowUserToResizeRows = $false
$grid.SelectionMode = 'FullRowSelect'
$grid.MultiSelect = $false
$grid.RowHeadersVisible = $false
$grid.AutoSizeColumnsMode = 'Fill'
$grid.BackgroundColor = [System.Drawing.Color]::White
[void]$grid.Columns.Add('Service', 'Service')
[void]$grid.Columns.Add('Status', 'Status')
[void]$grid.Columns.Add('PID', 'PID')
[void]$grid.Columns.Add('Started', 'Started')
$grid.Columns[0].FillWeight = 140
$grid.Columns[1].FillWeight = 90
$grid.Columns[2].FillWeight = 70
$grid.Columns[3].FillWeight = 80
$form.Controls.Add($grid)

$btnStart = New-Object System.Windows.Forms.Button
$btnStart.Text = 'BAT TAT CA'
$btnStart.Location = New-Object System.Drawing.Point(18, 280)
$btnStart.Size = New-Object System.Drawing.Size(140, 36)
$btnStart.BackColor = [System.Drawing.Color]::FromArgb(46, 125, 50)
$btnStart.ForeColor = [System.Drawing.Color]::White
$btnStart.FlatStyle = 'Flat'
$form.Controls.Add($btnStart)

$btnStop = New-Object System.Windows.Forms.Button
$btnStop.Text = 'DUNG TAT CA'
$btnStop.Location = New-Object System.Drawing.Point(170, 280)
$btnStop.Size = New-Object System.Drawing.Size(140, 36)
$btnStop.BackColor = [System.Drawing.Color]::FromArgb(183, 28, 28)
$btnStop.ForeColor = [System.Drawing.Color]::White
$btnStop.FlatStyle = 'Flat'
$form.Controls.Add($btnStop)

$btnRefresh = New-Object System.Windows.Forms.Button
$btnRefresh.Text = 'Lam moi'
$btnRefresh.Location = New-Object System.Drawing.Point(322, 280)
$btnRefresh.Size = New-Object System.Drawing.Size(120, 36)
$form.Controls.Add($btnRefresh)

$btnOpenFolder = New-Object System.Windows.Forms.Button
$btnOpenFolder.Text = 'Mo thu muc'
$btnOpenFolder.Location = New-Object System.Drawing.Point(454, 280)
$btnOpenFolder.Size = New-Object System.Drawing.Size(150, 36)
$form.Controls.Add($btnOpenFolder)

$txtLog = New-Object System.Windows.Forms.TextBox
$txtLog.Multiline = $true
$txtLog.ScrollBars = 'Vertical'
$txtLog.ReadOnly = $true
$txtLog.Location = New-Object System.Drawing.Point(18, 328)
$txtLog.Size = New-Object System.Drawing.Size(586, 140)
$txtLog.Font = New-Object System.Drawing.Font('Consolas', 9)
$form.Controls.Add($txtLog)

$btnStart.Add_Click({
  $btnStart.Enabled = $false
  $btnStop.Enabled = $false
  try {
    Invoke-ScriptOutside $startAll 'Start-all'
    Write-UiLog 'Doi server khoi dong...'
    Start-Sleep -Seconds 4
    Refresh-Status
    Write-UiLog 'Xong. Neu con STOPPED, bam Lam moi sau vai giay.'
  } finally {
    $btnStart.Enabled = $true
    $btnStop.Enabled = $true
  }
})

$btnStop.Add_Click({
  $confirm = [System.Windows.Forms.MessageBox]::Show(
    'Dung toan bo DataServer / JoinServer / ConnectServer / GameServer / AntiHack + watchdog?',
    'Xac nhan',
    'YesNo',
    'Warning'
  )
  if ($confirm -ne 'Yes') { return }
  $btnStart.Enabled = $false
  $btnStop.Enabled = $false
  try {
    Invoke-ScriptOutside $stopAll 'Stop-all'
    Start-Sleep -Seconds 2
    Refresh-Status
    Write-UiLog 'Da gui lenh dung.'
  } finally {
    $btnStart.Enabled = $true
    $btnStop.Enabled = $true
  }
})

$btnRefresh.Add_Click({ Refresh-Status; Write-UiLog 'Da lam moi trang thai.' })
$btnOpenFolder.Add_Click({ Start-Process explorer.exe $base })

$timer = New-Object System.Windows.Forms.Timer
$timer.Interval = 5000
$timer.Add_Tick({ Refresh-Status })
$form.Add_Shown({
  Write-UiLog ("Thu muc: {0}" -f $base)
  Refresh-Status
  $timer.Start()
})
$form.Add_FormClosed({ $timer.Stop(); $timer.Dispose() })

[void]$form.ShowDialog()
