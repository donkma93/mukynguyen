import HomePageView from "@/components/home/HomePageView";
import { getServerAuthSession } from "@/lib/auth";
import { getAccountCount, getOnlineCount } from "@/lib/game";
import { getDictionary } from "@/lib/i18n/get-dictionary";
import { getLocale } from "@/lib/i18n/locale";

export const dynamic = "force-dynamic";

export default async function HomePage() {
  const locale = await getLocale();
  const t = getDictionary(locale);
  const session = await getServerAuthSession();
  let online = 0;
  let accounts = 0;

  try {
    [online, accounts] = await Promise.all([
      getOnlineCount(),
      getAccountCount(),
    ]);
  } catch {
    // ignore offline DB during UI boot
  }

  const numberLocale =
    locale === "vi" ? "vi-VN" : locale === "pt" ? "pt-BR" : locale === "es" ? "es-ES" : "en-US";

  return (
    <HomePageView
      t={t}
      online={online}
      accounts={accounts}
      loggedIn={Boolean(session?.user?.id) && session?.user?.role === "user"}
      numberLocale={numberLocale}
    />
  );
}
