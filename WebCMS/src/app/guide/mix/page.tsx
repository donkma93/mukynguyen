import GuideMixPageView from "@/components/pages/GuideMixPageView";
import { getDictionary } from "@/lib/i18n/get-dictionary";
import { getLocale } from "@/lib/i18n/locale";

export async function generateMetadata() {
  const locale = await getLocale();
  const t = getDictionary(locale);
  return { title: t.guide.mix.title };
}

export default async function GuideMixPage() {
  const locale = await getLocale();
  const t = getDictionary(locale);
  return <GuideMixPageView t={t} />;
}
