/* WebCMS schema for the MuThangCuoi game database. Safe to run more than once. */
IF NOT EXISTS (SELECT 1 FROM sys.schemas WHERE name = N'cms')
  EXEC(N'CREATE SCHEMA cms');
GO

IF OBJECT_ID(N'cms.admins', N'U') IS NULL
CREATE TABLE cms.admins (
  id INT IDENTITY(1,1) NOT NULL PRIMARY KEY,
  username NVARCHAR(50) NOT NULL UNIQUE,
  password_hash NVARCHAR(200) NOT NULL,
  display_name NVARCHAR(100) NULL,
  created_at DATETIME NOT NULL CONSTRAINT DF_cms_admins_created_at DEFAULT GETDATE()
);
GO

IF OBJECT_ID(N'cms.posts', N'U') IS NULL
CREATE TABLE cms.posts (
  id INT IDENTITY(1,1) NOT NULL PRIMARY KEY,
  title NVARCHAR(200) NOT NULL,
  slug NVARCHAR(150) NOT NULL UNIQUE,
  excerpt NVARCHAR(500) NULL,
  content NVARCHAR(MAX) NOT NULL,
  post_type NVARCHAR(50) NOT NULL,
  cover_url NVARCHAR(500) NULL,
  is_published BIT NOT NULL CONSTRAINT DF_cms_posts_is_published DEFAULT 0,
  author NVARCHAR(100) NULL,
  created_at DATETIME NOT NULL CONSTRAINT DF_cms_posts_created_at DEFAULT GETDATE(),
  updated_at DATETIME NOT NULL CONSTRAINT DF_cms_posts_updated_at DEFAULT GETDATE()
);
GO

IF OBJECT_ID(N'cms.topup_requests', N'U') IS NULL
CREATE TABLE cms.topup_requests (
  id INT IDENTITY(1,1) NOT NULL PRIMARY KEY,
  account VARCHAR(10) NOT NULL,
  amount_vnd INT NOT NULL,
  note NVARCHAR(500) NULL,
  status NVARCHAR(20) NOT NULL,
  admin_note NVARCHAR(500) NULL,
  wcoinc_granted INT NULL,
  reviewed_by NVARCHAR(100) NULL,
  created_at DATETIME NOT NULL CONSTRAINT DF_cms_topup_requests_created_at DEFAULT GETDATE(),
  reviewed_at DATETIME NULL
);
GO

IF OBJECT_ID(N'cms.giftcodes', N'U') IS NULL
CREATE TABLE cms.giftcodes (
  id INT IDENTITY(1,1) NOT NULL PRIMARY KEY,
  code NVARCHAR(32) NOT NULL UNIQUE,
  description NVARCHAR(500) NULL,
  reward_wc INT NOT NULL CONSTRAINT DF_cms_giftcodes_reward_wc DEFAULT 0,
  reward_wp INT NOT NULL CONSTRAINT DF_cms_giftcodes_reward_wp DEFAULT 0,
  reward_wg INT NOT NULL CONSTRAINT DF_cms_giftcodes_reward_wg DEFAULT 0,
  reward_zen BIGINT NOT NULL CONSTRAINT DF_cms_giftcodes_reward_zen DEFAULT 0,
  reward_point INT NOT NULL CONSTRAINT DF_cms_giftcodes_reward_point DEFAULT 0,
  has_set BIT NOT NULL CONSTRAINT DF_cms_giftcodes_has_set DEFAULT 0,
  set_level INT NOT NULL CONSTRAINT DF_cms_giftcodes_set_level DEFAULT 9,
  set_skill BIT NOT NULL CONSTRAINT DF_cms_giftcodes_set_skill DEFAULT 1,
  set_luck BIT NOT NULL CONSTRAINT DF_cms_giftcodes_set_luck DEFAULT 1,
  set_option INT NOT NULL CONSTRAINT DF_cms_giftcodes_set_option DEFAULT 1,
  set_exc INT NOT NULL CONSTRAINT DF_cms_giftcodes_set_exc DEFAULT 32,
  set_duration INT NOT NULL CONSTRAINT DF_cms_giftcodes_set_duration DEFAULT 0,
  max_uses INT NOT NULL CONSTRAINT DF_cms_giftcodes_max_uses DEFAULT 1,
  used_count INT NOT NULL CONSTRAINT DF_cms_giftcodes_used_count DEFAULT 0,
  expires_at DATETIME NULL,
  is_active BIT NOT NULL CONSTRAINT DF_cms_giftcodes_is_active DEFAULT 1,
  created_at DATETIME NOT NULL CONSTRAINT DF_cms_giftcodes_created_at DEFAULT GETDATE()
);
GO

IF OBJECT_ID(N'cms.giftcode_claims', N'U') IS NULL
CREATE TABLE cms.giftcode_claims (
  id INT IDENTITY(1,1) NOT NULL PRIMARY KEY,
  giftcode_id INT NOT NULL,
  account VARCHAR(10) NOT NULL,
  claimed_at DATETIME NOT NULL CONSTRAINT DF_cms_giftcode_claims_claimed_at DEFAULT GETDATE(),
  CONSTRAINT UQ_cms_giftcode_claims_giftcode_account UNIQUE (giftcode_id, account),
  CONSTRAINT FK_cms_giftcode_claims_giftcodes FOREIGN KEY (giftcode_id)
    REFERENCES cms.giftcodes(id)
);
GO

IF OBJECT_ID(N'cms.admin_item_deliveries', N'U') IS NULL
CREATE TABLE cms.admin_item_deliveries (
  id INT IDENTITY(1,1) NOT NULL PRIMARY KEY,
  character_name VARCHAR(10) NOT NULL,
  account_id VARCHAR(10) NOT NULL,
  item_index INT NOT NULL,
  item_level TINYINT NOT NULL,
  durability TINYINT NOT NULL,
  skill BIT NOT NULL CONSTRAINT DF_cms_admin_item_deliveries_skill DEFAULT 0,
  luck BIT NOT NULL CONSTRAINT DF_cms_admin_item_deliveries_luck DEFAULT 0,
  item_option TINYINT NOT NULL CONSTRAINT DF_cms_admin_item_deliveries_option DEFAULT 0,
  excellent TINYINT NOT NULL CONSTRAINT DF_cms_admin_item_deliveries_excellent DEFAULT 0,
  ancient TINYINT NOT NULL CONSTRAINT DF_cms_admin_item_deliveries_ancient DEFAULT 0,
  socket_count TINYINT NOT NULL CONSTRAINT DF_cms_admin_item_deliveries_socket_count DEFAULT 0,
  duration_seconds INT NOT NULL CONSTRAINT DF_cms_admin_item_deliveries_duration_seconds DEFAULT 0,
  quantity TINYINT NOT NULL CONSTRAINT DF_cms_admin_item_deliveries_quantity DEFAULT 1,
  status VARCHAR(16) NOT NULL CONSTRAINT DF_cms_admin_item_deliveries_status DEFAULT 'pending',
  note NVARCHAR(300) NULL,
  created_by NVARCHAR(100) NULL,
  created_at DATETIME NOT NULL CONSTRAINT DF_cms_admin_item_deliveries_created_at DEFAULT GETDATE(),
  dispatched_at DATETIME NULL
);
GO

IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_cms_posts_published_created')
  CREATE INDEX IX_cms_posts_published_created ON cms.posts (is_published, created_at DESC);
IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_cms_topup_requests_account')
  CREATE INDEX IX_cms_topup_requests_account ON cms.topup_requests (account, created_at DESC);
IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_cms_topup_requests_status')
  CREATE INDEX IX_cms_topup_requests_status ON cms.topup_requests (status, created_at ASC);
IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_cms_admin_item_deliveries_pending')
  CREATE INDEX IX_cms_admin_item_deliveries_pending ON cms.admin_item_deliveries (character_name, account_id, status, created_at);
