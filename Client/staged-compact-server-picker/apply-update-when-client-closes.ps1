$source = 'E:\SRC ThangCuoi\Client\staged-compact-server-picker\Main.exe'
$target = 'E:\SRC ThangCuoi\Client\Main.exe'

for ($attempt = 0; $attempt -lt 1800; $attempt++) {
    if (-not (Get-Process -Name Main -ErrorAction SilentlyContinue)) {
        try {
            Copy-Item -LiteralPath $source -Destination $target -Force -ErrorAction Stop
            exit 0
        }
        catch {
            Start-Sleep -Seconds 1
        }
    }
    else {
        Start-Sleep -Seconds 1
    }
}

exit 1
