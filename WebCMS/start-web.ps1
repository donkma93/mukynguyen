$ErrorActionPreference = "Stop"
Set-Location $PSScriptRoot

Write-Host "==> MU Ky Nguyen WebCMS" -ForegroundColor Yellow

try {
  $sqlService = Get-Service -Name 'MSSQL$SQLEXPRESS' -ErrorAction SilentlyContinue
  if ($sqlService -and $sqlService.Status -ne 'Running') {
    Write-Host "Starting SQL Server Express..." -ForegroundColor Cyan
    Start-Service -Name 'MSSQL$SQLEXPRESS' -ErrorAction SilentlyContinue
  } elseif ($sqlService) {
    Write-Host "SQL Server Express dang chay." -ForegroundColor Green
  } else {
    Write-Host "Khong thay MSSQL`$SQLEXPRESS - dung DSN MuThangCuoi trong .env.local." -ForegroundColor DarkYellow
  }
} catch {
  Write-Host "Khong start duoc SQL Express. Tiep tuc Next.js..." -ForegroundColor DarkYellow
}

if (-not (Test-Path ".\node_modules")) {
  Write-Host "Chua co node_modules, dang npm install..." -ForegroundColor Cyan
  npm install
}

Write-Host "Chay Next.js: http://localhost:5000" -ForegroundColor Green
npm run dev
