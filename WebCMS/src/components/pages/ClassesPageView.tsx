"use client";

import Image from "next/image";
import Link from "next/link";
import { useEffect, useMemo, useState } from "react";
import type { Dictionary } from "@/lib/i18n/get-dictionary";
import {
  CLASS_CODES,
  type ClassCode,
  type PublicClassBase,
  type PublicSkillsGuide,
  type PublicSkillTip,
} from "@/lib/gs/public-skills";

type Props = {
  t: Dictionary;
};

const CLASS_ASSETS = [
  "/assets/classes/dark-wizard.png",
  "/assets/classes/dark-knight.png",
  "/assets/classes/fairy-elf.png",
  "/assets/classes/magic-gladiator.png",
  "/assets/classes/dark-lord.png",
  "/assets/classes/summoner.png",
  "/assets/classes/rage-fighter.png",
] as const;

const CLASS_CODE_BY_INDEX: ClassCode[] = [
  "DW",
  "DK",
  "FE",
  "MG",
  "DL",
  "SU",
  "RF",
];

const classAccents = [
  "from-violet-500/25 via-fuchsia-400/10 to-transparent",
  "from-red-500/25 via-amber-400/10 to-transparent",
  "from-emerald-400/25 via-lime-300/10 to-transparent",
  "from-sky-400/25 via-cyan-300/10 to-transparent",
  "from-amber-400/25 via-yellow-300/10 to-transparent",
  "from-purple-500/25 via-indigo-400/10 to-transparent",
  "from-rose-500/25 via-orange-400/10 to-transparent",
];

function tipSummary(tip: PublicSkillTip): string {
  const parts: string[] = [];
  const v = tip.values;
  if (v.constA != null || v.constB != null || v.constC != null) {
    parts.push(
      `A/B/C ${[v.constA, v.constB, v.constC].map((x) => x ?? "—").join("/")}`
    );
  }
  if (v.maxRate != null) parts.push(`max ${v.maxRate}%`);
  if (v.timeA != null) {
    parts.push(
      v.timeB != null ? `time ${v.timeA}/${v.timeB}` : `time ${v.timeA}`
    );
  }
  const selfRate = tip.classCodes
    .map((c) => v[`rate${c}`])
    .find((x) => x != null);
  if (selfRate != null) parts.push(`rate ${selfRate}%`);
  return parts.join(", ") || "—";
}

function BaseStats({
  base,
  t,
}: {
  base: PublicClassBase | undefined;
  t: Dictionary["classes"];
}) {
  if (!base) return null;
  return (
    <div className="mt-3 rounded-lg border border-white/10 bg-black/30 p-3">
      <p className="text-[10px] font-semibold uppercase tracking-[0.2em] text-mu-muted">
        {t.baseStatsLabel}
      </p>
      <div className="mt-2 grid grid-cols-4 gap-1.5 text-center text-[11px]">
        {(
          [
            [t.statStr, base.strength],
            [t.statAgi, base.dexterity],
            [t.statVit, base.vitality],
            [t.statEne, base.energy],
            [t.statCmd, base.leadership],
            [t.statLife, base.maxLife],
            [t.statMana, base.maxMana],
          ] as const
        ).map(([label, val]) => (
          <div
            key={label}
            className="rounded border border-white/5 bg-black/20 px-1 py-1.5"
          >
            <p className="text-mu-muted">{label}</p>
            <p className="font-mono text-mu-gold">{val ?? "—"}</p>
          </div>
        ))}
      </div>
    </div>
  );
}

export default function ClassesPageView({ t }: Props) {
  const [skills, setSkills] = useState<PublicSkillsGuide | null>(null);
  const [skillsFailed, setSkillsFailed] = useState(false);

  useEffect(() => {
    let cancelled = false;
    (async () => {
      try {
        const res = await fetch("/api/guide/skills", { cache: "no-store" });
        const json = await res.json().catch(() => ({}));
        if (!res.ok || !json?.ok || !json.data) {
          if (!cancelled) setSkillsFailed(true);
          return;
        }
        if (!cancelled) setSkills(json.data as PublicSkillsGuide);
      } catch {
        if (!cancelled) setSkillsFailed(true);
      }
    })();
    return () => {
      cancelled = true;
    };
  }, []);

  const baseByCode = useMemo(() => {
    const map = new Map<ClassCode, PublicClassBase>();
    for (const b of skills?.bases ?? []) map.set(b.code, b);
    return map;
  }, [skills]);

  const tipsByCode = useMemo(() => {
    const map = new Map<ClassCode, PublicSkillTip[]>();
    for (const code of CLASS_CODES) map.set(code, []);
    for (const tip of skills?.tips ?? []) {
      for (const code of tip.classCodes) {
        map.get(code)?.push(tip);
      }
    }
    return map;
  }, [skills]);

  return (
    <div className="relative mx-auto max-w-6xl px-4 py-12 md:py-16">
      <h1 className="section-title">{t.classes.title}</h1>
      <p className="muted mt-2 max-w-3xl">{t.classes.subtitle}</p>
      <div className="mb-10 mt-4 flex flex-wrap gap-2">
        <Link href="/balancing" className="btn-ghost">
          {t.classes.balancingCta}
        </Link>
        <Link href="/guide/mix" className="btn-ghost">
          {t.classes.mixCta}
        </Link>
        <Link href="/guide/farm" className="btn-ghost">
          {t.classes.farmCta}
        </Link>
      </div>

      {skillsFailed ? (
        <p className="mb-6 text-sm text-rose-300">
          {t.classes.skillUnavailable}
        </p>
      ) : null}

      <div className="grid gap-4 sm:grid-cols-2 lg:grid-cols-3 xl:grid-cols-4">
        {t.classes.items.map((cls, idx) => {
          const code = CLASS_CODE_BY_INDEX[idx] ?? "DW";
          const tips = tipsByCode.get(code) ?? [];
          return (
            <article
              key={cls.name}
              className="mu-card-glow card group relative overflow-hidden"
            >
              <div className="relative h-44 overflow-hidden border-b border-white/10">
                <Image
                  src={CLASS_ASSETS[idx % CLASS_ASSETS.length]}
                  alt={cls.name}
                  fill
                  className="object-cover object-top transition duration-500 group-hover:scale-105"
                  sizes="(max-width: 640px) 100vw, (max-width: 1024px) 50vw, 25vw"
                />
                <div
                  className={`pointer-events-none absolute inset-0 bg-gradient-to-t ${classAccents[idx % classAccents.length]}`}
                />
              </div>
              <div className="relative p-5">
                <p className="text-[11px] font-semibold uppercase tracking-[0.22em] text-mu-lime">
                  {cls.role}, {code}
                </p>
                <h2 className="mt-2 font-display text-xl text-mu-gold">
                  {cls.name}
                </h2>
                <p className="mt-3 text-sm leading-relaxed text-gray-300">
                  {cls.body}
                </p>
                <div className="mt-4 rounded-lg border border-white/10 bg-black/25 p-3">
                  <p className="text-[10px] font-semibold uppercase tracking-[0.2em] text-mu-muted">
                    {t.classes.playstyleLabel}
                  </p>
                  <p className="mt-1.5 text-xs leading-relaxed text-gray-300">
                    {cls.playstyle}
                  </p>
                </div>
                <BaseStats base={baseByCode.get(code)} t={t.classes} />
                {tips.length ? (
                  <div className="mt-3 rounded-lg border border-mu-gold/20 bg-mu-gold/5 p-3">
                    <p className="text-[10px] font-semibold uppercase tracking-[0.2em] text-mu-gold">
                      {t.classes.skillTipsLabel}
                    </p>
                    <ul className="mt-2 space-y-1.5">
                      {tips.map((tip) => (
                        <li key={tip.id} className="text-xs text-gray-300">
                          <span className="font-semibold text-mu-lime">
                            {(
                              t.classes.tipLabels as Record<string, string>
                            )[tip.id] ?? tip.id}
                          </span>
                          <span className="text-mu-muted">
                            {" "}
                            — {tipSummary(tip)}
                          </span>
                        </li>
                      ))}
                    </ul>
                  </div>
                ) : null}
              </div>
            </article>
          );
        })}
      </div>
    </div>
  );
}
