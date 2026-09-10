import CommandsPageView from "@/components/pages/CommandsPageView";
import { getDictionary } from "@/lib/i18n/get-dictionary";
import { getLocale } from "@/lib/i18n/locale";

export async function generateMetadata() {
  const locale = await getLocale();
  const t = getDictionary(locale);
  return { title: t.commands.title };
}

export default async function CommandsPage() {
  const locale = await getLocale();
  const t = getDictionary(locale);
  return <CommandsPageView t={t} />;
}
