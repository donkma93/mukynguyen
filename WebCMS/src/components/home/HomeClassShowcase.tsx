import Image from "next/image";
import Link from "next/link";
import type { Dictionary } from "@/lib/i18n/get-dictionary";

type Props = {
  t: Dictionary;
};

const CLASS_ASSETS = [
  {
    slug: "dark-wizard",
    image: "/assets/classes/dark-wizard.png",
    accent: "from-violet-500/35 via-fuchsia-400/10 to-transparent",
    ring: "shadow-[0_0_28px_rgba(168,96,255,0.35)]",
  },
  {
    slug: "dark-knight",
    image: "/assets/classes/dark-knight.png",
    accent: "from-red-500/35 via-amber-400/10 to-transparent",
    ring: "shadow-[0_0_28px_rgba(220,72,72,0.35)]",
  },
  {
    slug: "fairy-elf",
    image: "/assets/classes/fairy-elf.png",
    accent: "from-emerald-400/35 via-lime-300/10 to-transparent",
    ring: "shadow-[0_0_28px_rgba(80,220,140,0.35)]",
  },
  {
    slug: "magic-gladiator",
    image: "/assets/classes/magic-gladiator.png",
    accent: "from-sky-400/35 via-cyan-300/10 to-transparent",
    ring: "shadow-[0_0_28px_rgba(70,190,255,0.35)]",
  },
  {
    slug: "dark-lord",
    image: "/assets/classes/dark-lord.png",
    accent: "from-amber-400/35 via-yellow-300/10 to-transparent",
    ring: "shadow-[0_0_28px_rgba(255,190,70,0.35)]",
  },
  {
    slug: "summoner",
    image: "/assets/classes/summoner.png",
    accent: "from-purple-500/35 via-indigo-400/10 to-transparent",
    ring: "shadow-[0_0_28px_rgba(190,120,255,0.35)]",
  },
  {
    slug: "rage-fighter",
    image: "/assets/classes/rage-fighter.png",
    accent: "from-rose-500/35 via-orange-400/10 to-transparent",
    ring: "shadow-[0_0_28px_rgba(255,120,70,0.35)]",
  },
] as const;

export default function HomeClassShowcase({ t }: Props) {
  return (
    <section className="relative overflow-hidden border-y border-white/5 bg-black/30 py-16 md:py-22">
      <div className="pointer-events-none absolute inset-0">
        <Image
          src="/assets/classes/hero-classes.png"
          alt=""
          fill
          className="object-cover object-center opacity-[0.18] scale-110 blur-[1px]"
          sizes="100vw"
        />
        <div className="absolute inset-0 bg-gradient-to-b from-black/70 via-black/55 to-black/80" />
        <div className="absolute inset-0 mu-aurora opacity-40" />
      </div>

      <div className="relative mx-auto max-w-6xl px-4">
        <div className="mb-8 flex flex-wrap items-end justify-between gap-4">
          <div className="max-w-2xl">
            <p className="mb-2 text-xs font-semibold uppercase tracking-[0.3em] text-mu-lime">
              Season 5.2
            </p>
            <h2 className="section-title">{t.classes.title}</h2>
            <p className="muted mt-2">{t.classes.subtitle}</p>
          </div>
          <div className="flex flex-wrap gap-2">
            <Link href="/classes" className="btn-ghost">
              {t.home.exploreCta} →
            </Link>
            <Link href="/balancing" className="btn-ghost">
              {t.classes.balancingCta}
            </Link>
          </div>
        </div>

        <div className="grid gap-4 sm:grid-cols-2 lg:grid-cols-3 xl:grid-cols-4">
          {t.classes.items.map((cls, idx) => {
            const asset = CLASS_ASSETS[idx % CLASS_ASSETS.length];
            return (
              <article
                key={cls.name}
                className={`mu-card-glow card group relative overflow-hidden ${asset.ring}`}
                style={{ animationDelay: `${idx * 70}ms` }}
              >
                <div className="relative h-44 overflow-hidden border-b border-white/10">
                  <Image
                    src={asset.image}
                    alt={cls.name}
                    fill
                    className="object-cover object-top transition duration-500 group-hover:scale-105"
                    sizes="(max-width: 640px) 100vw, (max-width: 1024px) 50vw, 25vw"
                  />
                  <div
                    className={`pointer-events-none absolute inset-0 bg-gradient-to-t ${asset.accent}`}
                  />
                  <div className="pointer-events-none absolute inset-x-0 bottom-0 h-16 bg-gradient-to-t from-black/80 to-transparent" />
                </div>
                <div className="relative p-4">
                  <p className="text-[11px] font-semibold uppercase tracking-[0.22em] text-mu-lime">
                    {cls.role}
                  </p>
                  <h3 className="mt-1 font-display text-lg text-mu-gold">
                    {cls.name}
                  </h3>
                  <p className="mt-2 text-sm leading-relaxed text-gray-300">
                    {cls.body}
                  </p>
                </div>
              </article>
            );
          })}
        </div>
      </div>
    </section>
  );
}
