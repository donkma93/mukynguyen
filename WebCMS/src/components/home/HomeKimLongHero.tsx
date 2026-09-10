import Image from "next/image";
import Link from "next/link";
import HomeFx from "@/components/home/HomeFx";
import StatBadge from "@/components/StatBadge";
import type { Dictionary } from "@/lib/i18n/get-dictionary";
import { siteLinks } from "@/lib/site-links";

type Props = {
  t: Dictionary;
  online: number;
  accounts: number;
};

const GEMS = [
  {
    src: "/assets/landing/gems/bless.png",
    className: "left-[6%] top-[26%] hidden w-16 opacity-95 md:block lg:w-[4.5rem]",
    delay: "0ms",
  },
  {
    src: "/assets/landing/gems/cre.png",
    className: "left-[40%] top-[6%] w-14 opacity-90 sm:block lg:w-16",
    delay: "400ms",
  },
  {
    src: "/assets/landing/gems/chaos.png",
    className: "right-[6%] top-[8%] hidden w-[5.5rem] opacity-95 md:block lg:w-28",
    delay: "800ms",
  },
  {
    src: "/assets/landing/gems/soul.png",
    className: "left-[4%] bottom-[18%] hidden w-24 opacity-95 lg:block",
    delay: "1200ms",
  },
  {
    src: "/assets/landing/gems/life.png",
    className: "right-[10%] bottom-[20%] hidden w-20 opacity-95 md:block lg:w-24",
    delay: "600ms",
  },
] as const;

export default function HomeKimLongHero({ t, online, accounts }: Props) {
  const L = t.home.landing;
  const socialButtons = [
    {
      href: "/download",
      src: "/assets/landing/cta/download.png",
      tip: L.tipDownload,
      external: false,
      wide: true,
    },
    {
      href: "/register",
      src: "/assets/landing/cta/register.png",
      tip: L.tipRegister,
      external: false,
      wide: false,
    },
    {
      href: "/login",
      src: "/assets/landing/cta/login.png",
      tip: L.tipLogin,
      external: false,
      wide: false,
    },
    {
      href: siteLinks.facebookGroup,
      src: "/assets/landing/cta/groups.png",
      tip: L.tipGroups,
      external: true,
      wide: false,
    },
    {
      href: siteLinks.fanpage,
      src: "/assets/landing/cta/fanpage.png",
      tip: L.tipFanpage,
      external: true,
      wide: false,
    },
  ] as const;

  return (
    <section className="relative min-h-[92vh] overflow-hidden border-b border-mu-gold/20">
      <div className="absolute inset-0">
        <Image
          src="/assets/hero.png"
          alt={t.home.heroAlt}
          fill
          priority
          className="object-cover object-center scale-105"
          sizes="100vw"
        />
        <Image
          src="/assets/classes/hero-classes.png"
          alt=""
          fill
          priority
          className="object-cover object-right opacity-50 mix-blend-screen scale-110"
          sizes="100vw"
        />
        <div className="absolute inset-0 bg-hero-veil" />
        <div className="absolute inset-0 bg-gradient-to-b from-black/75 via-black/45 to-black/90" />
        <div className="absolute inset-0 bg-gradient-to-r from-black/70 via-transparent to-black/55" />
      </div>

      <div className="pointer-events-none absolute inset-0 z-[6] overflow-hidden" aria-hidden>
        {GEMS.map((gem) => (
          <Image
            key={gem.src}
            src={gem.src}
            alt=""
            width={120}
            height={120}
            className={`mu-gem ${gem.className}`}
            style={{ animationDelay: gem.delay }}
            unoptimized
          />
        ))}
      </div>

      <HomeFx count={28} />

      <div className="relative z-10 mx-auto flex max-w-6xl flex-col items-center px-4 pb-28 pt-16 text-center md:pb-32 md:pt-20">
        <p className="mu-rise mb-5 flex items-center justify-center gap-3 text-[11px] font-semibold uppercase tracking-[0.35em] text-[#f3d9a3] md:text-xs">
          <span className="hidden h-px w-12 bg-gradient-to-r from-transparent to-[#ffba00] sm:block" />
          {L.eyebrow}
          <span className="hidden h-px w-12 bg-gradient-to-l from-transparent to-[#ffba00] sm:block" />
        </p>

        <h1
          className="mu-rise mu-glow-title text-5xl leading-none md:text-7xl lg:text-8xl"
          data-text={L.glowTitle}
          style={{ animationDelay: "80ms" }}
        >
          {L.glowTitle}
        </h1>

        <div
          className="mu-rise mt-6 space-y-2"
          style={{ animationDelay: "140ms" }}
        >
          {L.slogans.map((line) => (
            <p
              key={line}
              className="text-sm font-semibold tracking-[0.18em] text-gray-100 md:text-base"
            >
              {line}
            </p>
          ))}
        </div>

        <div
          className="mu-rise mt-8 flex flex-col items-center gap-3 sm:flex-row sm:flex-wrap sm:justify-center"
          style={{ animationDelay: "200ms" }}
        >
          <div className="mu-date-badge bg-gradient-to-r from-mu-purple/80 to-black/80">
            <span className="bg-mu-purple/90 px-3 py-2 text-[11px] font-bold uppercase tracking-wider text-purple-100">
              {L.alphaLabel}
            </span>
            <span className="px-4 py-2 text-sm font-semibold text-white">
              {L.alphaValue}
            </span>
          </div>
          <div className="mu-date-badge bg-gradient-to-r from-mu-gold/90 to-[#6b5210]">
            <span className="bg-black/35 px-3 py-2 text-[11px] font-bold uppercase tracking-wider text-[#fff6d4]">
              {L.openLabel}
            </span>
            <span className="px-4 py-2 text-sm font-bold text-black">
              {L.openValue}
            </span>
          </div>
        </div>

        <div
          className="mu-rise mt-8 flex flex-wrap items-center justify-center gap-3"
          style={{ animationDelay: "260ms" }}
        >
          <Link href="/register" className="mu-cta-bar">
            {L.registerBar}
          </Link>
          <Link href="/download" className="mu-cta-bar border-mu-lime/40 text-mu-lime hover:text-white">
            {L.downloadBar}
          </Link>
        </div>

        <div
          className="mu-rise mt-8 flex flex-wrap items-end justify-center gap-3 md:gap-4"
          style={{ animationDelay: "320ms" }}
        >
          {socialButtons.map((btn) => {
            const sizeClass = btn.wide
              ? "h-14 w-[13.5rem] md:h-16 md:w-60"
              : "h-14 w-[7.25rem] md:h-16 md:w-32";
            const className = `group relative flex items-center justify-center transition hover:-translate-y-1 hover:drop-shadow-[0_0_16px_rgba(212,175,55,0.45)] ${sizeClass}`;
            const img = (
              // eslint-disable-next-line @next/next/no-img-element
              <img
                src={btn.src}
                alt={btn.tip}
                className="max-h-full max-w-full object-contain"
              />
            );
            const tip = (
              <span className="pointer-events-none absolute -top-8 left-1/2 z-20 -translate-x-1/2 whitespace-nowrap rounded bg-black/85 px-2 py-1 text-[10px] font-semibold uppercase tracking-wider text-mu-gold opacity-0 transition group-hover:opacity-100">
                {btn.tip}
              </span>
            );
            if (btn.external) {
              return (
                <a
                  key={btn.src}
                  href={btn.href}
                  target={btn.href.startsWith("http") ? "_blank" : undefined}
                  rel={btn.href.startsWith("http") ? "noreferrer" : undefined}
                  className={className}
                  title={btn.tip}
                >
                  {tip}
                  {img}
                </a>
              );
            }
            return (
              <Link key={btn.src} href={btn.href} className={className} title={btn.tip}>
                {tip}
                {img}
              </Link>
            );
          })}
        </div>

        <div
          className="mu-rise mt-10 grid w-full max-w-4xl gap-3 sm:grid-cols-2 lg:grid-cols-4"
          style={{ animationDelay: "380ms" }}
        >
          <StatBadge label={t.home.online} value={online} tone="lime" />
          <StatBadge label={t.home.accounts} value={accounts} tone="gold" />
          <StatBadge
            label={t.home.seasonLabel}
            value={t.home.seasonValue}
            tone="purple"
          />
          <StatBadge
            label={t.home.fpsLabel}
            value={t.home.fpsValue}
            tone="danger"
          />
        </div>

        <div className="mu-float mt-10 flex items-center gap-3 text-[10px] uppercase tracking-[0.28em] text-mu-muted">
          <Image
            src="/assets/landing/ui/mouse.png"
            alt=""
            width={18}
            height={28}
            className="opacity-80"
            unoptimized
          />
          {t.home.scrollHint}
          <Image
            src="/assets/landing/ui/scroll-hint.png"
            alt=""
            width={14}
            height={18}
            className="opacity-70"
            unoptimized
          />
        </div>
      </div>
    </section>
  );
}
