import { getPool, sql } from "@/lib/db";

let ensured = false;

export async function ensureAdminAuditTable(): Promise<void> {
  if (ensured) return;
  const pool = await getPool();
  await pool.request().query(`
    IF NOT EXISTS (SELECT 1 FROM sys.schemas WHERE name = 'cms')
      EXEC('CREATE SCHEMA cms');

    IF NOT EXISTS (
      SELECT 1 FROM sys.tables t
      INNER JOIN sys.schemas s ON t.schema_id = s.schema_id
      WHERE s.name = 'cms' AND t.name = 'admin_audit'
    )
    BEGIN
      CREATE TABLE cms.admin_audit (
        id INT IDENTITY(1,1) NOT NULL PRIMARY KEY,
        admin_user NVARCHAR(100) NOT NULL,
        action NVARCHAR(80) NOT NULL,
        target NVARCHAR(400) NULL,
        detail NVARCHAR(MAX) NULL,
        created_at DATETIME NOT NULL CONSTRAINT DF_cms_admin_audit_created DEFAULT (GETDATE())
      );
      CREATE INDEX IX_cms_admin_audit_created ON cms.admin_audit (created_at DESC);
    END
  `);
  ensured = true;
}

export async function writeAudit(input: {
  adminUser: string;
  action: string;
  target?: string;
  detail?: unknown;
}): Promise<void> {
  await ensureAdminAuditTable();
  const pool = await getPool();
  await pool
    .request()
    .input("admin", sql.NVarChar(100), input.adminUser)
    .input("action", sql.NVarChar(80), input.action)
    .input("target", sql.NVarChar(400), input.target ?? null)
    .input(
      "detail",
      sql.NVarChar(sql.MAX),
      input.detail === undefined || input.detail === null
        ? null
        : typeof input.detail === "string"
          ? input.detail
          : JSON.stringify(input.detail)
    )
    .query(`
      INSERT INTO cms.admin_audit (admin_user, action, target, detail, created_at)
      VALUES (@admin, @action, @target, @detail, GETDATE())
    `);
}

export async function listRecentAudit(limit = 50) {
  await ensureAdminAuditTable();
  const pool = await getPool();
  const r = await pool
    .request()
    .input("limit", sql.Int, limit)
    .query(`
      SELECT TOP (@limit) id, admin_user, action, target, detail, created_at
      FROM cms.admin_audit
      ORDER BY created_at DESC, id DESC
    `);
  return r.recordset.map((row) => ({
    id: Number(row.id),
    adminUser: String(row.admin_user),
    action: String(row.action),
    target: (row.target as string | null) ?? null,
    detail: (row.detail as string | null) ?? null,
    createdAt: row.created_at as Date,
  }));
}
