import GameplayPageView from "@/components/pages/GameplayPageView";
import { getDictionary } from "@/lib/i18n/get-dictionary";
import { getLocale } from "@/lib/i18n/locale";

export async function generateMetadata() {
  const locale = await getLocale();
  const t = getDictionary(locale);
  return { title: t.gameplay.title };
}

export default async function GameplayPage() {
  const locale = await getLocale();
  const t = getDictionary(locale);
  return <GameplayPageView t={t} />;
}
