# Resolve the current LocalDB pipe: its name changes whenever LocalDB restarts.
# The legacy SQL Server ODBC driver used by the 32-bit servers needs this pipe.
$ErrorActionPreference = 'Stop'
$localDbTool = (Get-Command SqlLocalDB.exe -ErrorAction Stop).Source
& $localDbTool start MSSQLLocalDB | Out-Null
if ($LASTEXITCODE -ne 0) { throw 'Cannot start MSSQLLocalDB.' }
$localDbInfo = (& $localDbTool info MSSQLLocalDB) -join "`n"
$pipeMatch = [regex]::Match($localDbInfo, 'np:[^\r\n]+')
if (-not $pipeMatch.Success) { throw 'MSSQLLocalDB did not return an instance pipe.' }
$muSqlPipe = $pipeMatch.Value.Trim()

# Fail before launching servers if the existing game database is unavailable.
Add-Type -AssemblyName System.Data
$muDbConnection = New-Object System.Data.SqlClient.SqlConnection(
  'Server=(localdb)\MSSQLLocalDB;Integrated Security=true;Database=MuThangCuoi;Connect Timeout=5')
try {
  $muDbConnection.Open()
  $muDbQuery = $muDbConnection.CreateCommand()
  $muDbQuery.CommandText = 'SELECT 1'
  [void]$muDbQuery.ExecuteScalar()
} finally {
  $muDbConnection.Dispose()
}

foreach ($muDsnName in @('MuThangCuoi', 'MuOnline')) {
  # Use the ODBC provider as well as the registry views. Packaged launchers can
  # have a virtualized HKCU view; the provider updates the DSN seen by servers.
  $muOdbcDsn = Get-OdbcDsn -Name $muDsnName -DsnType User -ErrorAction SilentlyContinue
  $muOdbcValues = @("Server=$muSqlPipe", 'Database=MuThangCuoi', 'Trusted_Connection=Yes')
  if (-not $muOdbcDsn) {
    Add-OdbcDsn -Name $muDsnName -DriverName 'SQL Server' -DsnType User -SetPropertyValue $muOdbcValues
  } elseif ($muOdbcDsn.Attribute['Server'] -ne $muSqlPipe -or
            $muOdbcDsn.Attribute['Database'] -ne 'MuThangCuoi' -or
            $muOdbcDsn.Attribute['Trusted_Connection'] -ne 'Yes') {
    Set-OdbcDsn -Name $muDsnName -DsnType User -SetPropertyValue $muOdbcValues
  }
  foreach ($muRegistryRoot in @('HKCU:\Software\ODBC\ODBC.INI', 'HKCU:\Software\Wow6432Node\ODBC\ODBC.INI')) {
    $muDsnPath = Join-Path $muRegistryRoot $muDsnName
    $muSourcesPath = Join-Path $muRegistryRoot 'ODBC Data Sources'
    $muDriverDirectory = if ($muRegistryRoot -like '*Wow6432Node*') { 'SysWOW64' } else { 'System32' }
    $muDsnProperties = @{
      Driver = Join-Path $env:SystemRoot "$muDriverDirectory\SQLSRV32.dll"
      Server = $muSqlPipe
      Database = 'MuThangCuoi'
      Trusted_Connection = 'Yes'
      Description = "$muDsnName LocalDB"
    }
    if (-not (Test-Path -LiteralPath $muDsnPath)) { New-Item -Path $muDsnPath -Force | Out-Null }
    if (-not (Test-Path -LiteralPath $muSourcesPath)) { New-Item -Path $muSourcesPath -Force | Out-Null }
    $muExisting = Get-ItemProperty -LiteralPath $muDsnPath
    foreach ($muProperty in $muDsnProperties.GetEnumerator()) {
      if ($muExisting.($muProperty.Key) -ne $muProperty.Value) {
        New-ItemProperty -LiteralPath $muDsnPath -Name $muProperty.Key -Value $muProperty.Value -PropertyType String -Force | Out-Null
      }
    }
    $muSource = Get-ItemProperty -LiteralPath $muSourcesPath
    if ($muSource.$muDsnName -ne 'SQL Server') {
      New-ItemProperty -LiteralPath $muSourcesPath -Name $muDsnName -Value 'SQL Server' -PropertyType String -Force | Out-Null
    }
  }
}
