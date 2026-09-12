$source = Join-Path $PSScriptRoot 'Main.exe'
$target = 'E:\SRC ThangCuoi\Client\Main.exe'
while (Get-Process -Name Main -ErrorAction SilentlyContinue) {
    Start-Sleep -Seconds 2
}

for ($attempt = 0; $attempt -lt 10; $attempt++) {
    try {
        Copy-Item -LiteralPath $source -Destination $target -Force -ErrorAction Stop
        exit 0
    }
    catch {
        Start-Sleep -Seconds 2
    }
}

exit 1
