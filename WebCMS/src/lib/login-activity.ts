import { getPool, sql } from "@/lib/db";

export type LoginActivity = {
  id: number;
  ipAddress: string;
  device: string;
  loggedInAt: Date;
};

export type GameLoginActivity = {
  ipAddress: string | null;
  serverName: string | null;
  connectedAt: Date | null;
};

export type AdminLoginActivity = LoginActivity & {
  account: string;
  gameIpAddress: string | null;
  gameServerName: string | null;
  gameConnectedAt: Date | null;
};

let schemaEnsured = false;

function normalizeAccount(account: string) {
  return account.trim().toLowerCase().slice(0, 10);
}

/** Produces a compact, privacy-conscious device label instead of retaining the full user agent. */
export function describeDevice(userAgent: string): string {
  const ua = userAgent.toLowerCase();
  const platform = ua.includes("iphone")
    ? "iPhone"
    : ua.includes("ipad")
      ? "iPad"
      : ua.includes("android")
        ? "Android"
        : ua.includes("windows")
          ? "Windows"
          : ua.includes("mac os") || ua.includes("macintosh")
            ? "macOS"
            : ua.includes("linux")
              ? "Linux"
              : "Thiết bị không xác định";
  const browser = ua.includes("edg/")
    ? "Edge"
    : ua.includes("opr/") || ua.includes("opera")
      ? "Opera"
      : ua.includes("firefox/")
        ? "Firefox"
        : ua.includes("chrome/") || ua.includes("crios/")
          ? "Chrome"
          : ua.includes("safari/")
            ? "Safari"
            : "Trình duyệt không xác định";
  return `${platform} • ${browser}`.slice(0, 160);
}

export async function ensureLoginActivitySchema(): Promise<void> {
  if (schemaEnsured) return;
  const pool = await getPool();
  await pool.request().query(`
    IF NOT EXISTS (SELECT 1 FROM sys.schemas WHERE name = N'cms')
      EXEC(N'CREATE SCHEMA cms');

    IF OBJECT_ID(N'cms.login_activity', N'U') IS NULL
    BEGIN
      CREATE TABLE cms.login_activity (
        id BIGINT IDENTITY(1,1) NOT NULL PRIMARY KEY,
        account VARCHAR(10) NOT NULL,
        ip_address VARCHAR(64) NOT NULL,
        device NVARCHAR(160) NOT NULL,
        logged_in_at DATETIME NOT NULL CONSTRAINT DF_cms_login_activity_logged_in_at DEFAULT GETDATE()
      );
      CREATE INDEX IX_cms_login_activity_account_time
        ON cms.login_activity (account, logged_in_at DESC);
    END
  `);
  schemaEnsured = true;
}

/** Stores a successful website login and removes entries older than the 30-day retention period. */
export async function recordLoginActivity(input: {
  account: string;
  ipAddress: string;
  device: string;
}): Promise<void> {
  await ensureLoginActivitySchema();
  const pool = await getPool();
  await pool
    .request()
    .input("account", sql.VarChar(10), normalizeAccount(input.account))
    .input("ip", sql.VarChar(64), input.ipAddress.trim().slice(0, 64) || "unknown")
    .input("device", sql.NVarChar(160), input.device.trim().slice(0, 160) || "Thiết bị không xác định")
    .query(`
      DELETE FROM cms.login_activity WHERE logged_in_at < DATEADD(day, -30, GETDATE());
      INSERT INTO cms.login_activity (account, ip_address, device, logged_in_at)
      VALUES (@account, @ip, @device, GETDATE());
    `);
}

export async function listLoginActivity(account: string): Promise<LoginActivity[]> {
  await ensureLoginActivitySchema();
  const pool = await getPool();
  const result = await pool
    .request()
    .input("account", sql.VarChar(10), normalizeAccount(account))
    .query<{
      id: number;
      ip_address: string;
      device: string;
      logged_in_at: Date;
    }>(`
      SELECT id, ip_address, device, logged_in_at
      FROM cms.login_activity
      WHERE account = @account AND logged_in_at >= DATEADD(day, -30, GETDATE())
      ORDER BY logged_in_at DESC, id DESC
    `);
  return result.recordset.map((row) => ({
    id: Number(row.id),
    ipAddress: row.ip_address,
    device: row.device,
    loggedInAt: row.logged_in_at,
  }));
}

/** Latest authentication written by the Mu GameServer for this account. */
export async function getGameLoginActivity(account: string): Promise<GameLoginActivity | null> {
  const pool = await getPool();
  const result = await pool
    .request()
    .input("account", sql.VarChar(10), normalizeAccount(account))
    .query<{
      IP: string | null;
      ServerName: string | null;
      ConnectTM: Date | null;
    }>(`
      SELECT TOP 1 IP, ServerName, ConnectTM
      FROM dbo.MEMB_STAT
      WHERE memb___id = @account
    `);
  const row = result.recordset[0];
  if (!row) return null;
  return {
    ipAddress: row.IP?.trim() || null,
    serverName: row.ServerName?.trim() || null,
    connectedAt: row.ConnectTM ?? null,
  };
}

/** Recent website authentication events for the admin security view. */
export async function listAdminLoginActivity(accountQuery = "", limit = 200): Promise<AdminLoginActivity[]> {
  await ensureLoginActivitySchema();
  const pool = await getPool();
  const query = accountQuery.trim().toLowerCase().slice(0, 10);
  const result = await pool
    .request()
    .input("query", sql.VarChar(10), query || null)
    .input("limit", sql.Int, Math.max(1, Math.min(500, Math.trunc(limit))))
    .query<{
      id: number;
      account: string;
      ip_address: string;
      device: string;
      logged_in_at: Date;
      game_ip_address: string | null;
      game_server_name: string | null;
      game_connected_at: Date | null;
    }>(`
      WITH latest_ip_per_account AS (
        SELECT
          id, account, ip_address, device, logged_in_at,
          ROW_NUMBER() OVER (
            PARTITION BY account, ip_address
            ORDER BY logged_in_at DESC, id DESC
          ) AS ip_rank
        FROM cms.login_activity
        WHERE logged_in_at >= DATEADD(day, -30, GETDATE())
          AND (@query IS NULL OR account LIKE '%' + @query + '%')
      )
      SELECT TOP (@limit)
        a.id, a.account, a.ip_address, a.device, a.logged_in_at,
        s.IP AS game_ip_address, s.ServerName AS game_server_name, s.ConnectTM AS game_connected_at
      FROM latest_ip_per_account a
      LEFT JOIN dbo.MEMB_STAT s ON s.memb___id = a.account
      WHERE a.ip_rank = 1
      ORDER BY a.logged_in_at DESC, a.id DESC
    `);
  return result.recordset.map((row) => ({
    id: Number(row.id),
    account: row.account,
    ipAddress: row.ip_address,
    device: row.device,
    loggedInAt: row.logged_in_at,
    gameIpAddress: row.game_ip_address?.trim() || null,
    gameServerName: row.game_server_name?.trim() || null,
    gameConnectedAt: row.game_connected_at ?? null,
  }));
}
