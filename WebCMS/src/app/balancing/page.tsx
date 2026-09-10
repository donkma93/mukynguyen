import BalancingPageView from "@/components/pages/BalancingPageView";
import { getPublicDictionary } from "@/lib/i18n/get-dictionary";
import { getLocale } from "@/lib/i18n/locale";

export const dynamic = "force-dynamic";

export async function generateMetadata() {
  const locale = await getLocale();
  const t = getPublicDictionary(locale);
  return { title: t.balancing.title };
}

export default async function BalancingPage() {
  const locale = await getLocale();
  const t = getPublicDictionary(locale);
  return <BalancingPageView t={t} />;
}
