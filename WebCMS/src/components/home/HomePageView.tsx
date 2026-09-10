import Image from "next/image";
import Link from "next/link";
import HomeClassShowcase from "@/components/home/HomeClassShowcase";
import HomeKimLongHero from "@/components/home/HomeKimLongHero";
import HomeServerInfoTabs from "@/components/home/HomeServerInfoTabs";
import HomeVipShopTeaser from "@/components/home/HomeVipShopTeaser";
import type { Dictionary } from "@/lib/i18n/get-dictionary";

type Props = {
  t: Dictionary;
  online: number;
  accounts: number;
  loggedIn?: boolean;
  numberLocale?: string;
};

const exploreCards = [
  {
    href: "/gameplay",
    titleKey: "gameplay" as const,
    bodyKey: "exploreGameplay" as const,
    accent: "from-amber-400/20 via-mu-gold/10 to-transparent",
  },
  {
    href: "/classes",
    titleKey: "classes" as const,
    bodyKey: "exploreClasses" as const,
    accent: "from-violet-500/20 via-fuchsia-400/10 to-transparent",
  },
  {
    href: "/events",
    titleKey: "events" as const,
    bodyKey: "exploreEvents" as const,
    accent: "from-mu-purple/25 via-purple-400/10 to-transparent",
  },
  {
    href: "/commands",
    titleKey: "commands" as const,
    bodyKey: "exploreCommands" as const,
    accent: "from-mu-lime/20 via-emerald-400/10 to-transparent",
  },
];

export default function HomePageView({
  t,
  online,
  accounts,
  loggedIn = false,
  numberLocale = "vi-VN",
}: Props) {
  return (
    <div className="relative overflow-hidden">
      <HomeKimLongHero t={t} online={online} accounts={accounts} />

      <HomeServerInfoTabs t={t} />

      <HomeVipShopTeaser loggedIn={loggedIn} numberLocale={numberLocale} />

      <section className="relative mx-auto max-w-6xl px-4 py-16 md:py-20">
        <div className="pointer-events-none absolute inset-x-0 top-0 h-px mu-section-line" />
        <div className="max-w-3xl">
          <p className="mb-2 text-xs font-semibold uppercase tracking-[0.3em] text-mu-lime">
            {t.home.aboutEyebrow}
          </p>
          <h2 className="section-title">{t.home.aboutTitle}</h2>
          <p className="mt-4 text-base leading-relaxed text-gray-300">
            {t.home.aboutBody}
          </p>
          <div className="mt-6 flex flex-wrap gap-2">
            <span className="badge-gold">{t.home.seasonValue}</span>
            <span className="badge-lime">{t.home.fpsValue} FPS</span>
            <span className="badge-purple">{t.home.antihackValue}</span>
          </div>
        </div>
      </section>

      <HomeClassShowcase t={t} />

      <section className="relative border-y border-white/5 bg-black/20 py-16 md:py-20">
        <div className="pointer-events-none absolute inset-0 mu-aurora opacity-40" />
        <div className="relative mx-auto max-w-6xl px-4">
          <div className="mb-8 max-w-2xl">
            <h2 className="section-title">{t.home.exploreTitle}</h2>
            <p className="muted mt-2">{t.home.exploreSubtitle}</p>
          </div>
          <div className="grid gap-4 md:grid-cols-2">
            {exploreCards.map((card, idx) => (
              <Link
                key={card.href}
                href={card.href}
                className="mu-card-glow card group relative overflow-hidden p-5 md:p-6 transition hover:border-mu-gold/40"
                style={{ animationDelay: `${idx * 60}ms` }}
              >
                <div
                  className={`pointer-events-none absolute inset-0 bg-gradient-to-br ${card.accent} opacity-70 transition duration-500 group-hover:opacity-100`}
                />
                <div className="relative">
                  <div className="mb-3 flex items-center justify-between gap-3">
                    <h3 className="panel-title">{t.nav[card.titleKey]}</h3>
                    <span className="text-xs font-semibold uppercase tracking-wider text-mu-gold">
                      {t.home.exploreCta} →
                    </span>
                  </div>
                  <p className="text-sm leading-relaxed text-gray-300">
                    {t.home[card.bodyKey]}
                  </p>
                </div>
              </Link>
            ))}
          </div>
        </div>
      </section>

      <section className="relative overflow-hidden py-16 md:py-24">
        <div className="pointer-events-none absolute inset-0">
          <Image
            src="/assets/classes/hero-classes.png"
            alt=""
            fill
            className="object-cover object-center opacity-20"
            sizes="100vw"
          />
          <div className="mu-aurora absolute inset-0 opacity-70" />
          <div className="absolute left-1/2 top-1/2 h-72 w-[36rem] -translate-x-1/2 -translate-y-1/2 rounded-full bg-mu-gold/10 blur-3xl" />
          <div className="absolute inset-0 bg-gradient-to-b from-black/70 via-black/50 to-black/80" />
        </div>
        <div className="relative mx-auto max-w-4xl px-4 text-center">
          <h2 className="font-display text-3xl font-bold text-white md:text-5xl">
            <span className="mu-shine-text">{t.home.ctaTitle}</span>
          </h2>
          <p className="mx-auto mt-4 max-w-2xl text-base text-gray-300">
            {t.home.ctaBody}
          </p>
          <div className="mt-8 flex flex-wrap items-center justify-center gap-3">
            <Link href="/register" className="btn-gold shadow-gold">
              {t.home.ctaRegister}
            </Link>
            <Link href="/download" className="btn-lime">
              {t.home.ctaDownload}
            </Link>
          </div>
        </div>
      </section>
    </div>
  );
}
