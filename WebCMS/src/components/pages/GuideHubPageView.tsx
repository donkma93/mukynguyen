import Link from "next/link";
import type { Dictionary } from "@/lib/i18n/get-dictionary";

type Props = {
  t: Dictionary;
};

export default function GuideHubPageView({ t }: Props) {
  const g = t.guide.hub;
  return (
    <div className="relative overflow-hidden">
      <section className="relative border-b border-white/5 bg-gradient-to-b from-mu-purple/10 via-transparent to-black/30 py-12 md:py-16">
        <div className="pointer-events-none absolute inset-x-0 top-0 h-px mu-section-line" />
        <div className="mx-auto max-w-6xl px-4">
          <div className="mb-3 flex flex-wrap items-center gap-2">
            <span className="badge-lime">{g.liveBadge}</span>
          </div>
          <h1 className="section-title">{g.title}</h1>
          <p className="muted mt-2 mb-10 max-w-3xl">{g.subtitle}</p>

          <div className="grid gap-4 md:grid-cols-2">
            {g.topics.map((topic) => (
              <Link
                key={topic.href}
                href={topic.href}
                className="mu-card-glow card group block p-5 transition hover:border-mu-gold/40"
              >
                <h2 className="panel-title text-mu-gold group-hover:text-mu-lime">
                  {topic.title}
                </h2>
                <p className="mt-3 text-sm leading-relaxed text-gray-300">
                  {topic.body}
                </p>
                <span className="mt-4 inline-block text-xs font-semibold uppercase tracking-[0.18em] text-mu-muted group-hover:text-mu-gold">
                  {t.home.exploreCta} →
                </span>
              </Link>
            ))}
          </div>

          <div className="mt-10 card p-5">
            <h2 className="panel-title">{g.moreTitle}</h2>
            <div className="mt-4 flex flex-wrap gap-2">
              <Link href="/balancing" className="btn-ghost">
                {g.moreBalancing}
              </Link>
              <Link href="/commands" className="btn-ghost">
                {g.moreCommands}
              </Link>
              <Link href="/gameplay" className="btn-ghost">
                {g.moreGameplay}
              </Link>
            </div>
          </div>
        </div>
      </section>
    </div>
  );
}
