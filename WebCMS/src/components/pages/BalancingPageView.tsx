"use client";

import { useEffect, useState } from "react";
import type { Dictionary } from "@/lib/i18n/get-dictionary";
import {
  BALANCE_CLASS_CODES,
  BALANCE_CLASS_META,
  type BalanceClassCode,
} from "@/lib/gs/balancing";
import type { PublicBalancing } from "@/lib/gs/public-balancing";

type Props = {
  t: Dictionary;
};

function cellTone(value: number | null): string {
  if (value == null) return "bg-white/5 text-mu-muted";
  if (value < 50) return "bg-sky-500/15 text-sky-200";
  if (value < 100) return "bg-white/5 text-gray-200";
  if (value === 100) return "bg-mu-gold/15 text-mu-gold font-semibold";
  return "bg-rose-500/20 text-rose-200 font-semibold";
}

function RateCell({ value }: { value: number | null }) {
  return (
    <td
      className={`px-2 py-2 text-center font-mono text-sm tabular-nums ${cellTone(value)}`}
    >
      {value == null ? "—" : `${value}%`}
    </td>
  );
}

function ClassRateStrip({
  title,
  rates,
}: {
  title: string;
  rates: Record<BalanceClassCode, number | null>;
}) {
  return (
    <section className="card p-5">
      <h2 className="panel-title">{title}</h2>
      <div className="mt-4 grid gap-2 sm:grid-cols-2 lg:grid-cols-4 xl:grid-cols-7">
        {BALANCE_CLASS_CODES.map((code) => (
          <div
            key={code}
            className="rounded-lg border border-white/10 bg-black/20 px-3 py-3 text-center"
          >
            <p className="text-[11px] font-semibold uppercase tracking-[0.18em] text-mu-lime">
              {code}
            </p>
            <p className="mt-1 text-xs text-mu-muted">
              {BALANCE_CLASS_META[code].full}
            </p>
            <p
              className={`mt-2 font-mono text-lg tabular-nums ${cellTone(rates[code])}`}
            >
              {rates[code] == null ? "—" : `${rates[code]}%`}
            </p>
          </div>
        ))}
      </div>
    </section>
  );
}

export default function BalancingPageView({ t }: Props) {
  const [data, setData] = useState<PublicBalancing | null>(null);
  const [loading, setLoading] = useState(true);
  const [failed, setFailed] = useState(false);

  useEffect(() => {
    let cancelled = false;
    (async () => {
      try {
        const res = await fetch("/api/balancing", { cache: "no-store" });
        const json = await res.json().catch(() => ({}));
        if (!res.ok || !json?.ok || !json.data) {
          if (!cancelled) setFailed(true);
          return;
        }
        if (!cancelled) {
          setData(json.data as PublicBalancing);
          setFailed(false);
        }
      } catch {
        if (!cancelled) setFailed(true);
      } finally {
        if (!cancelled) setLoading(false);
      }
    })();
    return () => {
      cancelled = true;
    };
  }, []);

  return (
    <div className="relative mx-auto max-w-6xl px-4 py-12 md:py-16">
      <h1 className="section-title">{t.balancing.title}</h1>
      <p className="muted mt-2 mb-8 max-w-3xl">{t.balancing.subtitle}</p>

      {loading ? (
        <div className="card p-6 text-mu-muted">{t.common.loading}</div>
      ) : failed || !data ? (
        <div className="card border-rose-500/30 p-6 text-rose-200">
          {t.balancing.unavailable}
        </div>
      ) : (
        <div className="space-y-8">
          <section className="card overflow-hidden p-0">
            <div className="border-b border-white/10 px-5 py-4">
              <h2 className="panel-title">{t.balancing.matrixTitle}</h2>
              <p className="muted mt-1 text-sm">{t.balancing.matrixHint}</p>
            </div>
            <div className="overflow-x-auto">
              <table className="w-full min-w-[720px] border-collapse text-sm">
                <thead>
                  <tr className="bg-mu-panel/80 text-xs uppercase tracking-wider text-mu-muted">
                    <th className="sticky left-0 z-10 bg-mu-panel px-3 py-3 text-left">
                      {t.balancing.attacker}
                    </th>
                    {BALANCE_CLASS_CODES.map((code) => (
                      <th key={code} className="px-2 py-3 text-center">
                        <span className="block text-mu-gold">{code}</span>
                        <span className="mt-0.5 block text-[10px] font-normal normal-case tracking-normal text-mu-muted">
                          {BALANCE_CLASS_META[code].full}
                        </span>
                      </th>
                    ))}
                  </tr>
                </thead>
                <tbody>
                  {BALANCE_CLASS_CODES.map((atk) => (
                    <tr key={atk} className="border-t border-white/5">
                      <th className="sticky left-0 z-10 bg-mu-card px-3 py-2 text-left">
                        <span className="block font-semibold text-mu-lime">
                          {atk}
                        </span>
                        <span className="block text-[11px] font-normal text-mu-muted">
                          {BALANCE_CLASS_META[atk].full}
                        </span>
                      </th>
                      {BALANCE_CLASS_CODES.map((def) => (
                        <RateCell key={def} value={data.matrix[atk][def]} />
                      ))}
                    </tr>
                  ))}
                </tbody>
              </table>
            </div>
            <div className="flex flex-wrap gap-3 border-t border-white/10 px-5 py-3 text-xs text-mu-muted">
              <span className={`rounded px-2 py-1 ${cellTone(30)}`}>
                {t.balancing.legendLow}
              </span>
              <span className={`rounded px-2 py-1 ${cellTone(75)}`}>
                {t.balancing.legendMid}
              </span>
              <span className={`rounded px-2 py-1 ${cellTone(100)}`}>
                {t.balancing.legendBase}
              </span>
              <span className={`rounded px-2 py-1 ${cellTone(130)}`}>
                {t.balancing.legendHigh}
              </span>
            </div>
          </section>

          <ClassRateStrip title={t.balancing.pvpTitle} rates={data.pvp} />
          <ClassRateStrip title={t.balancing.pvmTitle} rates={data.pvm} />
        </div>
      )}
    </div>
  );
}
