import Link from "next/link";
import type { Dictionary } from "@/lib/i18n/get-dictionary";

type Props = {
  t: Dictionary;
};

export default function GameplayPageView({ t }: Props) {
  return (
    <div className="relative overflow-hidden">
      <section className="relative mx-auto max-w-6xl px-4 py-12 md:py-16">
        <p className="mb-2 text-xs font-semibold uppercase tracking-[0.3em] text-mu-lime">
          {t.gameplay.aboutEyebrow}
        </p>
        <h1 className="section-title">{t.gameplay.title}</h1>
        <p className="muted mt-2 max-w-3xl">{t.gameplay.subtitle}</p>

        <div className="mt-10 grid items-center gap-8 lg:grid-cols-[1.1fr_0.9fr]">
          <div>
            <h2 className="panel-title text-mu-gold">{t.gameplay.aboutTitle}</h2>
            <p className="mt-4 text-base leading-relaxed text-gray-300">
              {t.gameplay.aboutBody}
            </p>
            <div className="mt-6 flex flex-wrap gap-2">
              <span className="badge-gold">{t.home.seasonValue}</span>
              <span className="badge-lime">{t.home.fpsValue} FPS</span>
              <span className="badge-purple">{t.home.antihackValue}</span>
            </div>
          </div>

          <div className="card-glow relative overflow-hidden p-6">
            <div className="absolute -right-10 -top-10 h-40 w-40 rounded-full bg-mu-gold/15 blur-2xl" />
            <div className="absolute -bottom-12 -left-8 h-36 w-36 rounded-full bg-mu-purple/25 blur-2xl" />
            <div className="relative grid grid-cols-2 gap-3">
              <div className="rounded-xl border border-mu-gold/25 bg-black/30 p-4">
                <p className="text-[11px] uppercase tracking-wider text-mu-muted">
                  {t.home.seasonLabel}
                </p>
                <p className="mt-1 font-display text-3xl text-mu-gold">
                  {t.home.seasonValue}
                </p>
              </div>
              <div className="rounded-xl border border-mu-lime/25 bg-black/30 p-4">
                <p className="text-[11px] uppercase tracking-wider text-mu-muted">
                  {t.home.fpsLabel}
                </p>
                <p className="mt-1 font-display text-3xl text-mu-lime">
                  {t.home.fpsValue}
                </p>
              </div>
              <div className="col-span-2 rounded-xl border border-purple-400/25 bg-black/30 p-4">
                <p className="text-[11px] uppercase tracking-wider text-mu-muted">
                  {t.home.antihackLabel}
                </p>
                <p className="mt-1 font-display text-2xl text-purple-200">
                  {t.home.antihackValue}
                </p>
              </div>
            </div>
          </div>
        </div>
      </section>

      <section className="relative border-y border-white/5 bg-black/20 py-16 md:py-20">
        <div className="pointer-events-none absolute inset-0 mu-aurora opacity-40" />
        <div className="relative mx-auto max-w-6xl px-4">
          <div className="mb-8 max-w-2xl">
            <h2 className="section-title">{t.gameplay.pillarsTitle}</h2>
            <p className="muted mt-2">{t.gameplay.pillarsSubtitle}</p>
          </div>
          <div className="grid gap-4 md:grid-cols-2">
            {t.gameplay.pillars.map((item, idx) => (
              <article
                key={item.title}
                className="mu-card-glow card p-5 md:p-6"
                style={{ animationDelay: `${idx * 60}ms` }}
              >
                <div className="mb-3 flex items-center gap-3">
                  <span className="flex h-10 w-10 items-center justify-center rounded-full border border-mu-gold/40 bg-mu-gold/10 font-display text-mu-gold">
                    {String(idx + 1).padStart(2, "0")}
                  </span>
                  <h3 className="panel-title">{item.title}</h3>
                </div>
                <p className="text-sm leading-relaxed text-gray-300">
                  {item.body}
                </p>
              </article>
            ))}
          </div>

          <div className="mt-10 card p-6 md:flex md:items-center md:justify-between md:gap-6">
            <div>
              <h3 className="panel-title text-mu-gold">
                {t.gameplay.guideCtaTitle}
              </h3>
              <p className="mt-2 text-sm text-gray-300">
                {t.gameplay.guideCtaBody}
              </p>
            </div>
            <Link href="/guide" className="btn-primary mt-4 inline-flex md:mt-0">
              {t.gameplay.guideCta}
            </Link>
          </div>
        </div>
      </section>
    </div>
  );
}
