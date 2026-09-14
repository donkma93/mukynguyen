export type PartnerTier = "new" | "stable" | "top";
export type PartnerPackageId = "NEWBIE" | "LIVE_DROP" | "HIGHLIGHT";

export type PartnerItemReward = {
  itemIndex: number;
  quantity: number;
  /** Timed buff length in seconds; 0 = permanent. */
  durationSeconds: number;
  label: string;
};

export type PartnerPackage = {
  id: PartnerPackageId;
  titleVi: string;
  wc: number;
  wp: number;
  wg: number;
  items: readonly PartnerItemReward[];
  /** Max giftcode uses when creating a session code. */
  maxUses: number;
  /** Code TTL in ms; null = no auto-expiry from catalog. */
  ttlMs: number | null;
};

export type PartnerTierBudget = {
  tier: PartnerTier;
  labelVi: string;
  wc: number;
  wp: number;
  wg: number;
  liveSessions: number;
  highlight: number;
};

/** Item indexes from Mu Server Item.txt (section*512 + type). */
export const PARTNER_ITEMS = {
  bless: { itemIndex: 7181, label: "Ngọc Ước Nguyện" },
  soul: { itemIndex: 7182, label: "Ngọc Tâm Linh" },
  chaos: { itemIndex: 6159, label: "Ngọc Hỗn Nguyên" },
  life: { itemIndex: 7184, label: "Ngọc Sinh Mệnh" },
  expBuff: { itemIndex: 6699, label: "Bùa Tăng EXP" },
} as const;

export const PARTNER_PACKAGES: Record<PartnerPackageId, PartnerPackage> = {
  NEWBIE: {
    id: "NEWBIE",
    titleVi: "Chào mừng người xem mới",
    wc: 0,
    wp: 100,
    wg: 0,
    maxUses: 200,
    ttlMs: 7 * 24 * 60 * 60 * 1000,
    items: [
      { itemIndex: PARTNER_ITEMS.bless.itemIndex, quantity: 2, durationSeconds: 0, label: PARTNER_ITEMS.bless.label },
      { itemIndex: PARTNER_ITEMS.soul.itemIndex, quantity: 2, durationSeconds: 0, label: PARTNER_ITEMS.soul.label },
    ],
  },
  LIVE_DROP: {
    id: "LIVE_DROP",
    titleVi: "Quà phiên live",
    wc: 0,
    wp: 80,
    wg: 30,
    maxUses: 30,
    ttlMs: 2 * 60 * 60 * 1000,
    items: [
      { itemIndex: PARTNER_ITEMS.chaos.itemIndex, quantity: 1, durationSeconds: 0, label: PARTNER_ITEMS.chaos.label },
      {
        itemIndex: PARTNER_ITEMS.expBuff.itemIndex,
        quantity: 1,
        durationSeconds: 1800,
        label: PARTNER_ITEMS.expBuff.label,
      },
    ],
  },
  HIGHLIGHT: {
    id: "HIGHLIGHT",
    titleVi: "Quà Highlight",
    wc: 100,
    wp: 200,
    wg: 0,
    maxUses: 1,
    ttlMs: null,
    items: [
      { itemIndex: PARTNER_ITEMS.bless.itemIndex, quantity: 3, durationSeconds: 0, label: PARTNER_ITEMS.bless.label },
      { itemIndex: PARTNER_ITEMS.soul.itemIndex, quantity: 3, durationSeconds: 0, label: PARTNER_ITEMS.soul.label },
      { itemIndex: PARTNER_ITEMS.life.itemIndex, quantity: 1, durationSeconds: 0, label: PARTNER_ITEMS.life.label },
    ],
  },
};

export const PARTNER_TIER_BUDGETS: Record<PartnerTier, PartnerTierBudget> = {
  new: {
    tier: "new",
    labelVi: "Partner mới",
    wc: 1000,
    wp: 8000,
    wg: 2000,
    liveSessions: 12,
    highlight: 20,
  },
  stable: {
    tier: "stable",
    labelVi: "Partner ổn định",
    wc: 2000,
    wp: 15000,
    wg: 4000,
    liveSessions: 20,
    highlight: 40,
  },
  top: {
    tier: "top",
    labelVi: "Partner top",
    wc: 3000,
    wp: 25000,
    wg: 6000,
    liveSessions: 30,
    highlight: 60,
  },
};

export const HIGHLIGHT_PER_LIVE_SESSION = 5;
export const NEWBIE_COOLDOWN_MS = 7 * 24 * 60 * 60 * 1000;
export const PARTNER_TZ = "Asia/Ho_Chi_Minh";

export function isPartnerTier(value: string): value is PartnerTier {
  return value === "new" || value === "stable" || value === "top";
}

export function isPartnerPackageId(value: string): value is PartnerPackageId {
  return value === "NEWBIE" || value === "LIVE_DROP" || value === "HIGHLIGHT";
}

export function packageCoinCost(packageId: PartnerPackageId): {
  wc: number;
  wp: number;
  wg: number;
} {
  const pack = PARTNER_PACKAGES[packageId];
  return { wc: pack.wc, wp: pack.wp, wg: pack.wg };
}

export function getTierBudget(tier: PartnerTier): PartnerTierBudget {
  return PARTNER_TIER_BUDGETS[tier];
}

/** Calendar day key in Vietnam time: YYYY-MM-DD */
export function vietnamDayKey(date: Date = new Date()): string {
  return new Intl.DateTimeFormat("en-CA", {
    timeZone: PARTNER_TZ,
    year: "numeric",
    month: "2-digit",
    day: "2-digit",
  }).format(date);
}

/** Budget month key in Vietnam time: YYYY-MM */
export function vietnamYearMonth(date: Date = new Date()): string {
  return vietnamDayKey(date).slice(0, 7);
}

/** Start of Vietnam calendar day as UTC Date (for SQL comparisons). */
export function vietnamDayStartUtc(date: Date = new Date()): Date {
  const day = vietnamDayKey(date);
  // Asia/Ho_Chi_Minh is UTC+7 year-round
  return new Date(`${day}T00:00:00+07:00`);
}

export function remainingBudget(
  tier: PartnerTier,
  used: {
    wcUsed: number;
    wpUsed: number;
    wgUsed: number;
    liveSessionsUsed: number;
    highlightUsed: number;
  }
) {
  const cap = getTierBudget(tier);
  return {
    wc: Math.max(0, cap.wc - used.wcUsed),
    wp: Math.max(0, cap.wp - used.wpUsed),
    wg: Math.max(0, cap.wg - used.wgUsed),
    liveSessions: Math.max(0, cap.liveSessions - used.liveSessionsUsed),
    highlight: Math.max(0, cap.highlight - used.highlightUsed),
    caps: cap,
  };
}
