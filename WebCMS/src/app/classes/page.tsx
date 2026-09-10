import ClassesPageView from "@/components/pages/ClassesPageView";
import { getDictionary } from "@/lib/i18n/get-dictionary";
import { getLocale } from "@/lib/i18n/locale";

export async function generateMetadata() {
  const locale = await getLocale();
  const t = getDictionary(locale);
  return { title: t.classes.title };
}

export default async function ClassesPage() {
  const locale = await getLocale();
  const t = getDictionary(locale);
  return <ClassesPageView t={t} />;
}
