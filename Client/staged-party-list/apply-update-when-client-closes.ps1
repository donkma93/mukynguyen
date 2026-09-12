param(
  [string]$Source = 'E:\SRC ThangCuoi\Client\staged-party-list\Main.exe',
  [string]$Target = 'E:\SRC ThangCuoi\Client\Main.exe'
)
while (Get-Process -Name Main -ErrorAction SilentlyContinue) { Start-Sleep -Seconds 2 }
Copy-Item -LiteralPath $Source -Destination $Target -Force
