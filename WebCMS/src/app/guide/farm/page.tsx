import GuideFarmPageView from "@/components/pages/GuideFarmPageView";
import { getDictionary } from "@/lib/i18n/get-dictionary";
import { getLocale } from "@/lib/i18n/locale";

export async function generateMetadata() {
  const locale = await getLocale();
  const t = getDictionary(locale);
  return { title: t.guide.farm.title };
}

export default async function GuideFarmPage() {
  const locale = await getLocale();
  const t = getDictionary(locale);
  return <GuideFarmPageView t={t} />;
}
