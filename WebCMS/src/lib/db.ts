import sql from "mssql/msnodesqlv8";

declare global {
  // eslint-disable-next-line no-var
  var __muSqlPool: sql.ConnectionPool | undefined;
}

function buildConfig(): sql.config {
  // ODBC DSN "MuThangCuoi" (created by Mu Server/start-all.ps1). Change DSN/DB
  // on another machine — do not hardcode a host filesystem path.
  const connectionString =
    process.env.MSSQL_CONNECTION_STRING ||
    process.env.DATABASE_URL ||
    "DSN=MuThangCuoi;";

  return {
    server: "localhost",
    connectionString,
    connectionTimeout: 15000,
    requestTimeout: 30000,
    pool: { max: 10, min: 0, idleTimeoutMillis: 30000 },
    options: {
      trustedConnection: true,
      trustServerCertificate: true,
      enableArithAbort: true,
      encrypt: false,
      useUTC: false,
    },
  } as unknown as sql.config;
}

export async function getPool(): Promise<sql.ConnectionPool> {
  if (global.__muSqlPool?.connected) return global.__muSqlPool;
  const pool = new sql.ConnectionPool(buildConfig());
  await pool.connect();
  global.__muSqlPool = pool;
  return pool;
}

export { sql };
