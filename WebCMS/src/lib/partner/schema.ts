import { getPool } from "@/lib/db";

/** Creates partner tables on existing game DBs without a manual migration. */
export async function ensurePartnerSchema(): Promise<void> {
  const pool = await getPool();
  await pool.request().query(`
    IF OBJECT_ID(N'cms.partners', N'U') IS NULL
    BEGIN
      CREATE TABLE cms.partners (
        account VARCHAR(10) NOT NULL PRIMARY KEY,
        tier VARCHAR(16) NOT NULL CONSTRAINT DF_cms_partners_tier DEFAULT 'new',
        is_active BIT NOT NULL CONSTRAINT DF_cms_partners_is_active DEFAULT 1,
        created_by NVARCHAR(100) NULL,
        created_at DATETIME NOT NULL CONSTRAINT DF_cms_partners_created_at DEFAULT GETDATE(),
        updated_at DATETIME NOT NULL CONSTRAINT DF_cms_partners_updated_at DEFAULT GETDATE()
      );
    END

    IF OBJECT_ID(N'cms.partner_budget_months', N'U') IS NULL
    BEGIN
      CREATE TABLE cms.partner_budget_months (
        partner_account VARCHAR(10) NOT NULL,
        year_month CHAR(7) NOT NULL,
        wc_used INT NOT NULL CONSTRAINT DF_cms_partner_budget_wc DEFAULT 0,
        wp_used INT NOT NULL CONSTRAINT DF_cms_partner_budget_wp DEFAULT 0,
        wg_used INT NOT NULL CONSTRAINT DF_cms_partner_budget_wg DEFAULT 0,
        live_sessions_used INT NOT NULL CONSTRAINT DF_cms_partner_budget_live DEFAULT 0,
        highlight_used INT NOT NULL CONSTRAINT DF_cms_partner_budget_hl DEFAULT 0,
        CONSTRAINT PK_cms_partner_budget_months PRIMARY KEY (partner_account, year_month)
      );
    END

    IF OBJECT_ID(N'cms.partner_sessions', N'U') IS NULL
    BEGIN
      CREATE TABLE cms.partner_sessions (
        id INT IDENTITY(1,1) NOT NULL PRIMARY KEY,
        partner_account VARCHAR(10) NOT NULL,
        package_id VARCHAR(16) NOT NULL,
        giftcode_id INT NULL,
        code NVARCHAR(32) NOT NULL,
        max_uses INT NOT NULL CONSTRAINT DF_cms_partner_sessions_max_uses DEFAULT 1,
        highlight_count INT NOT NULL CONSTRAINT DF_cms_partner_sessions_hl DEFAULT 0,
        expires_at DATETIME NULL,
        is_active BIT NOT NULL CONSTRAINT DF_cms_partner_sessions_active DEFAULT 1,
        created_at DATETIME NOT NULL CONSTRAINT DF_cms_partner_sessions_created DEFAULT GETDATE()
      );
      CREATE INDEX IX_cms_partner_sessions_partner
        ON cms.partner_sessions (partner_account, created_at DESC);
      CREATE UNIQUE INDEX UQ_cms_partner_sessions_code
        ON cms.partner_sessions (code);
    END

    IF OBJECT_ID(N'cms.partner_claims', N'U') IS NULL
    BEGIN
      CREATE TABLE cms.partner_claims (
        id INT IDENTITY(1,1) NOT NULL PRIMARY KEY,
        partner_account VARCHAR(10) NOT NULL,
        package_id VARCHAR(16) NOT NULL,
        session_id INT NULL,
        claimer_account VARCHAR(10) NOT NULL,
        claimer_character VARCHAR(10) NOT NULL,
        wc INT NOT NULL CONSTRAINT DF_cms_partner_claims_wc DEFAULT 0,
        wp INT NOT NULL CONSTRAINT DF_cms_partner_claims_wp DEFAULT 0,
        wg INT NOT NULL CONSTRAINT DF_cms_partner_claims_wg DEFAULT 0,
        note NVARCHAR(300) NULL,
        claimed_at DATETIME NOT NULL CONSTRAINT DF_cms_partner_claims_at DEFAULT GETDATE()
      );
      CREATE INDEX IX_cms_partner_claims_partner
        ON cms.partner_claims (partner_account, claimed_at DESC);
      CREATE INDEX IX_cms_partner_claims_claimer_pkg
        ON cms.partner_claims (claimer_account, package_id, claimed_at DESC);
    END
  `);
}
