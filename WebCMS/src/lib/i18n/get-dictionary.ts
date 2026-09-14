import type { Locale } from "@/lib/i18n/config";
import type { Dictionary } from "@/lib/i18n/dictionaries/vi";
import vi from "@/lib/i18n/dictionaries/vi";
import en from "@/lib/i18n/dictionaries/en";
import pt from "@/lib/i18n/dictionaries/pt";
import es from "@/lib/i18n/dictionaries/es";

const dictionaries: Record<Locale, Dictionary> = {
  vi,
  en,
  pt,
  es,
};

export function getDictionary(locale: Locale): Dictionary {
  return dictionaries[locale] ?? dictionaries.vi;
}

/**
 * Dictionary safe to embed in public HTML via I18nProvider.
 * Omits admin-only copy so GS/ops internals are not leaked to visitors.
 */
export function getPublicDictionary(locale: Locale): Dictionary {
  const full = getDictionary(locale);
  return {
    ...full,
    admin: {
      sidebarTitle: "",
      dashboard: "",
      accounts: "",
      characters: "",
      hack: "",
      ops: "",
      gsIni: "",
      gsGroups: "",
      balancing: "",
      giftcodes: "",
      partners: "",
      dashboardTitle: "",
      dashboardSubtitle: "",
      online: "",
      accountsStat: "",
      gsIniCard: "",
      gsIniCardHint: "",
      gsGroupsCard: "",
      gsGroupsCardHint: "",
      gsGroupsTitle: "",
      gsGroupsSubtitle: "",
      gsGroupNormal: "",
      gsGroupVip: "",
      gsGroupItemDrop: "",
      gsGroupJewelDrop: "",
      gsGroupMoneyDrop: "",
      gsGroupJewelHint: "",
      gsGroupSave: "",
      gsGroupSaving: "",
      gsGroupAvailable: "",
      gsGroupMissing: "",
      gsGroupToOps: "",
      balancingCard: "",
      balancingCardHint: "",
      opsCard: "",
      opsCardHint: "",
      hackCard: "",
      hackCardHint: "",
    },
  };
}

export type { Dictionary };
