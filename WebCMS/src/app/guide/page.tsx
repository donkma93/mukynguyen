import GuideHubPageView from "@/components/pages/GuideHubPageView";
import { getDictionary } from "@/lib/i18n/get-dictionary";
import { getLocale } from "@/lib/i18n/locale";

export async function generateMetadata() {
  const locale = await getLocale();
  const t = getDictionary(locale);
  return { title: t.guide.hub.title };
}

export default async function GuideHubPage() {
  const locale = await getLocale();
  const t = getDictionary(locale);
  return <GuideHubPageView t={t} />;
}
