import Image from "next/image";
import Link from "next/link";
import { getServerAuthSession } from "@/lib/auth";
import LanguageSwitcher from "@/components/LanguageSwitcher";
import LogoutButton from "@/components/LogoutButton";
import SiteNav from "@/components/SiteNav";
import { getPublicDictionary } from "@/lib/i18n/get-dictionary";
import { getLocale } from "@/lib/i18n/locale";

export default async function SiteHeader() {
  const session = await getServerAuthSession();
  const role = session?.user?.role;
  const name = session?.user?.name;
  const locale = await getLocale();
  const t = getPublicDictionary(locale);

  return (
    <header className="sticky top-0 z-50 border-b border-mu-gold/20 bg-mu-bg/90 backdrop-blur-md">
      <div className="mx-auto grid h-[65px] max-w-6xl grid-cols-[auto_1fr_auto] items-center gap-3 px-4">
        <Link href="/" className="flex shrink-0 items-center gap-3">
          <Image
            src="/assets/logo.png"
            alt={t.brand.name}
            width={40}
            height={40}
            className="rounded-full"
          />
          <div className="leading-tight">
            <div className="font-display text-base font-bold text-mu-gold sm:text-lg">
              {t.brand.name}
            </div>
            <div className="hidden text-[10px] uppercase tracking-[0.2em] text-mu-muted sm:block">
              {t.brand.webcms}
            </div>
          </div>
        </Link>

        <div className="flex min-w-0 justify-center">
          <SiteNav
            t={t}
            showPanel={Boolean(session)}
            showAdmin={role === "admin"}
          />
        </div>

        <div className="flex shrink-0 items-center justify-end gap-2">
          <LanguageSwitcher />
          {session ? (
            <>
              <span className="hidden text-sm text-mu-muted xl:inline">
                {t.nav.hello} <span className="text-mu-gold">{name}</span>
              </span>
              <LogoutButton />
            </>
          ) : (
            <>
              <Link
                href="/login"
                className="btn-ghost hidden px-3 py-2 text-xs sm:inline-flex sm:text-sm"
              >
                {t.nav.login}
              </Link>
              <Link
                href="/register"
                className="btn-gold px-3 py-2 text-xs sm:text-sm"
              >
                {t.nav.register}
              </Link>
            </>
          )}
        </div>
      </div>
    </header>
  );
}
