import GsGroupsRatesPanel from "@/components/admin/GsGroupsRatesPanel";
import { getDictionary } from "@/lib/i18n/get-dictionary";
import { getLocale } from "@/lib/i18n/locale";

export const dynamic = "force-dynamic";

export async function generateMetadata() {
  const locale = await getLocale();
  const t = getDictionary(locale);
  return { title: t.admin.gsGroups };
}

export default async function AdminGsGroupsPage() {
  const locale = await getLocale();
  const t = getDictionary(locale);
  const a = t.admin;

  return (
    <GsGroupsRatesPanel
      labels={{
        title: a.gsGroupsTitle,
        subtitle: a.gsGroupsSubtitle,
        normal: a.gsGroupNormal,
        vip: a.gsGroupVip,
        itemDrop: a.gsGroupItemDrop,
        jewelDrop: a.gsGroupJewelDrop,
        moneyDrop: a.gsGroupMoneyDrop,
        jewelHint: a.gsGroupJewelHint,
        save: a.gsGroupSave,
        saving: a.gsGroupSaving,
        available: a.gsGroupAvailable,
        missing: a.gsGroupMissing,
        toOps: a.gsGroupToOps,
      }}
    />
  );
}
