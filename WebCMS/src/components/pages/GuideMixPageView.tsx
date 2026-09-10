"use client";

import { useEffect, useState } from "react";
import type { Dictionary } from "@/lib/i18n/get-dictionary";
import {
  ACCOUNT_LEVELS,
  type AccountLevel,
} from "@/lib/gs/public-guide-parse";
import {
  formatAlRates,
  type AlRates,
  type PublicChaosMix,
} from "@/lib/gs/public-chaos-mix";

type Props = {
  t: Dictionary;
};

function AlTable({
  rows,
  labelCol,
  rateLabel,
  vipShort,
  vipJoin,
}: {
  rows: { key: string; label: string; rates: AlRates }[];
  labelCol: string;
  rateLabel: string;
  vipShort: Record<string, string>;
  vipJoin: string;
}) {
  const showSplit = rows.some((r) => {
    const vals = ACCOUNT_LEVELS.map((al) => r.rates[al]).filter(
      (v): v is number => v != null
    );
    return vals.length > 1 && new Set(vals).size > 1;
  });

  return (
    <div className="overflow-x-auto">
      <table className="w-full min-w-[320px] border-collapse text-sm">
        <thead>
          <tr className="border-b border-white/10 text-left text-[11px] uppercase tracking-[0.16em] text-mu-muted">
            <th className="px-3 py-2">{labelCol}</th>
            {showSplit ? (
              ACCOUNT_LEVELS.map((al) => (
                <th key={al} className="px-3 py-2 text-center">
                  {vipShort[String(al) as keyof typeof vipShort] ?? `VIP ${al}`}
                </th>
              ))
            ) : (
              <th className="px-3 py-2 text-center">{rateLabel}</th>
            )}
          </tr>
        </thead>
        <tbody>
          {rows.map((row) => (
            <tr key={row.key} className="border-b border-white/5">
              <td className="px-3 py-2.5 font-medium text-gray-200">
                {row.label}
              </td>
              {showSplit ? (
                ACCOUNT_LEVELS.map((al: AccountLevel) => (
                  <td
                    key={al}
                    className="px-3 py-2.5 text-center tabular-nums text-mu-gold"
                  >
                    {row.rates[al] == null ? "—" : `${row.rates[al]}%`}
                  </td>
                ))
              ) : (
                <td className="px-3 py-2.5 text-center tabular-nums text-mu-gold">
                  {formatAlRates(row.rates, { vipShort, join: vipJoin })}
                </td>
              )}
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
}

export default function GuideMixPageView({ t }: Props) {
  const m = t.guide.mix;
  const [data, setData] = useState<PublicChaosMix | null>(null);
  const [loading, setLoading] = useState(true);
  const [failed, setFailed] = useState(false);

  useEffect(() => {
    let cancelled = false;
    (async () => {
      try {
        const res = await fetch("/api/guide/mix", { cache: "no-store" });
        const json = await res.json().catch(() => ({}));
        if (!res.ok || !json?.ok || !json.data) {
          if (!cancelled) setFailed(true);
          return;
        }
        if (!cancelled) {
          setData(json.data as PublicChaosMix);
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
      <h1 className="section-title">{m.title}</h1>
      <p className="muted mt-2 max-w-3xl">{m.subtitle}</p>
      <p className="mt-3 text-xs text-mu-lime">{m.sourceNote}</p>
      <p className="mt-1 text-xs text-mu-muted">{m.alHint}</p>

      {loading ? (
        <p className="mt-10 text-sm text-mu-muted">{t.common.loading}</p>
      ) : null}
      {failed ? (
        <p className="mt-10 text-sm text-rose-300">{m.unavailable}</p>
      ) : null}

      {data ? (
        <div className="mt-10 space-y-6">
          <section className="card p-5">
            <h2 className="panel-title">{m.plusTitle}</h2>
            <p className="muted mt-1 text-xs">{m.plusHint}</p>
            <div className="mt-4">
              <AlTable
                labelCol={m.colLevel}
                rateLabel={m.colRate}
                vipShort={m.vipShort}
                vipJoin={m.vipJoin}
                rows={data.plusItem.map((r) => ({
                  key: String(r.level),
                  label: r.label,
                  rates: r.rates,
                }))}
              />
            </div>
          </section>

          <section className="card p-5">
            <h2 className="panel-title">{m.jewelTitle}</h2>
            <div className="mt-4">
              <AlTable
                labelCol={m.colLevel}
                rateLabel={m.colRate}
                vipShort={m.vipShort}
                vipJoin={m.vipJoin}
                rows={[
                  { key: "soul", label: m.soul, rates: data.jewels.soul },
                  { key: "life", label: m.life, rates: data.jewels.life },
                  {
                    key: "harmony",
                    label: m.harmony,
                    rates: data.jewels.harmony,
                  },
                  { key: "luck1", label: m.luck1, rates: data.jewels.luck1 },
                  { key: "luck2", label: m.luck2, rates: data.jewels.luck2 },
                ]}
              />
            </div>
          </section>

          <section className="card p-5">
            <h2 className="panel-title">{m.wingTitle}</h2>
            <div className="mt-4">
              <AlTable
                labelCol={m.colLevel}
                rateLabel={m.colRate}
                vipShort={m.vipShort}
                vipJoin={m.vipJoin}
                rows={data.wings.map((w) => ({
                  key: w.key,
                  label: w.label,
                  rates: w.rates,
                }))}
              />
            </div>
          </section>

          <div className="grid gap-6 lg:grid-cols-2">
            <section className="card p-5">
              <h2 className="panel-title">{m.bcTitle}</h2>
              <div className="mt-4">
                <AlTable
                  labelCol={m.colTier}
                  rateLabel={m.colRate}
                  vipShort={m.vipShort}
                  vipJoin={m.vipJoin}
                  rows={data.bloodCastleMix.map((r) => ({
                    key: `bc-${r.tier}`,
                    label: String(r.tier),
                    rates: r.rates,
                  }))}
                />
              </div>
            </section>
            <section className="card p-5">
              <h2 className="panel-title">{m.dsTitle}</h2>
              <div className="mt-4">
                <AlTable
                  labelCol={m.colTier}
                  rateLabel={m.colRate}
                  vipShort={m.vipShort}
                  vipJoin={m.vipJoin}
                  rows={data.devilSquareMix.map((r) => ({
                    key: `ds-${r.tier}`,
                    label: String(r.tier),
                    rates: r.rates,
                  }))}
                />
              </div>
            </section>
          </div>

          <section className="card p-5">
            <h2 className="panel-title">{m.dropTitle}</h2>
            <div className="mt-4">
              <AlTable
                labelCol={m.colLevel}
                rateLabel={m.colRate}
                vipShort={m.vipShort}
                vipJoin={m.vipJoin}
                rows={[
                  {
                    key: "itemDrop",
                    label: m.itemDrop,
                    rates: data.itemDropRate,
                  },
                  {
                    key: "moneyDrop",
                    label: m.moneyDrop,
                    rates: data.moneyDropRate,
                  },
                ]}
              />
            </div>
          </section>
        </div>
      ) : null}
    </div>
  );
}
