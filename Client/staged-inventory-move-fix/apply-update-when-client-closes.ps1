$ErrorActionPreference = 'Stop'

$inventoryFixRoot = Split-Path -Parent $PSScriptRoot
$inventoryFixSource = Join-Path $PSScriptRoot 'Main.exe'
$inventoryFixTarget = Join-Path $inventoryFixRoot 'Main.exe'
$inventoryFixSourcePdb = Join-Path $PSScriptRoot 'Main.pdb'
$inventoryFixTargetPdb = Join-Path $inventoryFixRoot 'Main.pdb'

while (Get-Process -Name Main -ErrorAction SilentlyContinue) {
  Start-Sleep -Seconds 2
}

$inventoryFixBackup = Join-Path $inventoryFixRoot ('backup-before-inventory-move-fix-' + (Get-Date -Format 'yyyyMMdd-HHmmss'))
New-Item -ItemType Directory -Path $inventoryFixBackup -ErrorAction Stop | Out-Null
Copy-Item -LiteralPath $inventoryFixTarget -Destination (Join-Path $inventoryFixBackup 'Main.exe') -ErrorAction Stop
if (Test-Path -LiteralPath $inventoryFixTargetPdb) {
  Copy-Item -LiteralPath $inventoryFixTargetPdb -Destination (Join-Path $inventoryFixBackup 'Main.pdb') -ErrorAction Stop
}

Copy-Item -LiteralPath $inventoryFixSource -Destination $inventoryFixTarget -Force -ErrorAction Stop
Copy-Item -LiteralPath $inventoryFixSourcePdb -Destination $inventoryFixTargetPdb -Force -ErrorAction Stop

if ((Get-FileHash -LiteralPath $inventoryFixSource -Algorithm SHA256).Hash -ne
    (Get-FileHash -LiteralPath $inventoryFixTarget -Algorithm SHA256).Hash) {
  throw 'Main.exe hash mismatch after applying the update.'
}
