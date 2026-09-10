/** Public community links — override via NEXT_PUBLIC_* in .env.local */
export const siteLinks = {
  facebookGroup:
    process.env.NEXT_PUBLIC_FB_GROUP_URL?.trim() || "#community-group",
  fanpage: process.env.NEXT_PUBLIC_FB_FANPAGE_URL?.trim() || "#fanpage",
  zalo: process.env.NEXT_PUBLIC_ZALO_URL?.trim() || "#zalo",
} as const;
