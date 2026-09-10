Add-Type -AssemblyName System.Data
$conn = New-Object System.Data.SqlClient.SqlConnection("Server=(localdb)\MSSQLLocalDB;Integrated Security=true;Database=MuThangCuoi;Pooling=false")
$conn.Open()
$cmd = $conn.CreateCommand()
$cmd.CommandText = "SELECT 1"
while ($true) {
  try {
    if ($conn.State -ne 'Open') { $conn.Open() }
    [void]$cmd.ExecuteScalar()
  } catch {
    try { $conn.Close() } catch {}
    try { $conn.Open() } catch {}
  }
  Start-Sleep -Seconds 10
}
