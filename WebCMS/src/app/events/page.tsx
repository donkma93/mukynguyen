import EventsPageView from "@/components/pages/EventsPageView";
import { getDictionary } from "@/lib/i18n/get-dictionary";
import { getLocale } from "@/lib/i18n/locale";

export async function generateMetadata() {
  const locale = await getLocale();
  const t = getDictionary(locale);
  return { title: t.events.title };
}

export default async function EventsPage() {
  const locale = await getLocale();
  const t = getDictionary(locale);
  return <EventsPageView t={t} />;
}
