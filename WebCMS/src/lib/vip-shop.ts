export type VipPackageId = "vip7" | "vip15" | "vip30";

export type VipPackage = {
  id: VipPackageId;
  days: number;
  priceWc: number;
  titleVi: string;
  titleEn: string;
};

/** Single VIP type (AccountLevel=1). Three duration packs priced in WCoin (WC). */
export const VIP_PACKAGES: readonly VipPackage[] = [
  { id: "vip7", days: 7, priceWc: 5000, titleVi: "VIP 7 ngày", titleEn: "VIP 7 Days" },
  { id: "vip15", days: 15, priceWc: 9000, titleVi: "VIP 15 ngày", titleEn: "VIP 15 Days" },
  { id: "vip30", days: 30, priceWc: 15000, titleVi: "VIP 30 ngày", titleEn: "VIP 30 Days" },
] as const;

export function getVipPackage(id: string): VipPackage | undefined {
  return VIP_PACKAGES.find((p) => p.id === id);
}

export function clampVipLevel(level: number): 0 | 1 {
  if (!Number.isFinite(level) || level <= 0) return 0;
  return 1;
}
