"use client";

import { useEffect, useState } from "react";
import type { Dictionary } from "@/lib/i18n/get-dictionary";
import type { PublicFarmGuide } from "@/lib/gs/public-farm";

type Props = {
  t: Dictionary;
};

function rateCell(v: number | null): string {
  return v == null ? "—" : `${v}%`;
}

export default function GuideFarmPageView({ t }: Props) {
  const f = t.guide.farm;
  const [data, setData] = useState<PublicFarmGuide | null>(null);
  const [loading, setLoading] = useState(true);
  const [failed, setFailed] = useState(false);

  useEffect(() => {
    let cancelled = false;
    (async () => {
      try {
        const res = await fetch("/api/guide/farm", { cache: "no-store" });
        const json = await res.json().catch(() => ({}));
        if (!res.ok || !json?.ok || !json.data) {
          if (!cancelled) setFailed(true);
          return;
        }
        if (!cancelled) {
          setData(json.data as PublicFarmGuide);
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

  const groupCols = [
    { key: "normal" as const, label: f.groupNormal },
    { key: "vip" as const, label: f.groupVip },
  ];

  return (
    <div className="relative mx-auto max-w-6xl px-4 py-12 md:py-16">
      <h1 className="section-title">{f.title}</h1>
      <p className="muted mt-2 max-w-3xl">{f.subtitle}</p>
      <p className="mt-3 text-xs text-mu-lime">{f.sourceNote}</p>

      {loading ? (
        <p className="mt-10 text-sm text-mu-muted">{t.common.loading}</p>
      ) : null}
      {failed ? (
        <p className="mt-10 text-sm text-rose-300">{f.unavailable}</p>
      ) : null}

      {data ? (
        <div className="mt-10 space-y-6">
          <section className="card p-5">
            <h2 className="panel-title">{f.vipTitle}</h2>
            <p className="muted mt-1 text-xs">{f.groupHint}</p>
            <div className="mt-4 overflow-x-auto">
              <table className="w-full min-w-[360px] border-collapse text-sm">
                <thead>
                  <tr className="border-b border-white/10 text-left text-[11px] uppercase tracking-[0.16em] text-mu-muted">
                    <th className="px-3 py-2">{t.guide.mix.colLevel}</th>
                    {groupCols.map((col) => (
                      <th key={col.key} className="px-3 py-2 text-center">
                        {col.label}
                      </th>
                    ))}
                  </tr>
                </thead>
                <tbody>
                  {(
                    [
                      ["item", t.guide.mix.itemDrop, "itemDropRate"],
                      ["jewel", f.jewelDrop, "jewelDropRate"],
                      ["zen", t.guide.mix.moneyDrop, "moneyDropRate"],
                    ] as const
                  ).map(([key, label, field]) => (
                    <tr key={key} className="border-b border-white/5">
                      <td className="px-3 py-2.5 text-gray-200">{label}</td>
                      {groupCols.map((col) => (
                        <td
                          key={col.key}
                          className="px-3 py-2.5 text-center font-mono tabular-nums text-mu-gold"
                        >
                          {rateCell(data[col.key][field])}
                        </td>
                      ))}
                    </tr>
                  ))}
                </tbody>
              </table>
            </div>
            {data.availableSubs.length || data.missingSubs.length ? (
              <p className="mt-3 text-[11px] text-mu-muted">
                {f.subsNote
                  .replace(
                    "{available}",
                    data.availableSubs.map((s) => s.label).join(", ") || "—"
                  )
                  .replace(
                    "{missing}",
                    data.missingSubs.map((s) => s.label).join(", ") || "—"
                  )}
              </p>
            ) : null}
          </section>

          <section className="card p-5">
            <h2 className="panel-title">{f.mapTitle}</h2>
            <p className="muted mt-1 text-xs">{f.mapHint}</p>
            <div className="mt-4 overflow-x-auto">
              <table className="w-full min-w-[480px] border-collapse text-sm">
                <thead>
                  <tr className="border-b border-white/10 text-left text-[11px] uppercase tracking-[0.16em] text-mu-muted">
                    <th className="px-3 py-2">#</th>
                    <th className="px-3 py-2">{f.colMap}</th>
                    <th className="px-3 py-2 text-center">{f.colExp}</th>
                    <th className="px-3 py-2 text-center">{f.colDrop}</th>
                    <th className="px-3 py-2 text-center">{f.colHelper}</th>
                  </tr>
                </thead>
                <tbody>
                  {data.maps.map((map) => (
                    <tr key={map.index} className="border-b border-white/5">
                      <td className="px-3 py-2 font-mono text-mu-muted">
                        {map.index}
                      </td>
                      <td className="px-3 py-2 text-gray-200">{map.name}</td>
                      <td className="px-3 py-2 text-center font-mono tabular-nums text-mu-lime">
                        {map.experienceRate ?? "—"}
                      </td>
                      <td className="px-3 py-2 text-center font-mono tabular-nums text-mu-gold">
                        {map.itemDropRate ?? "—"}
                      </td>
                      <td className="px-3 py-2 text-center text-xs text-mu-muted">
                        {map.helperEnable == null
                          ? "—"
                          : map.helperEnable
                            ? f.helperOn
                            : f.helperOff}
                      </td>
                    </tr>
                  ))}
                </tbody>
              </table>
            </div>
          </section>
        </div>
      ) : null}

      <section className="mt-10">
        <h2 className="section-title text-2xl">{f.routesTitle}</h2>
        <div className="mt-6 grid gap-4 md:grid-cols-3">
          {f.routes.map((route, idx) => (
            <article key={route.title} className="mu-card-glow card p-5">
              <span className="text-[11px] font-semibold uppercase tracking-[0.2em] text-mu-lime">
                {String(idx + 1).padStart(2, "0")}
              </span>
              <h3 className="mt-2 panel-title">{route.title}</h3>
              <p className="mt-3 text-sm leading-relaxed text-gray-300">
                {route.body}
              </p>
            </article>
          ))}
        </div>
      </section>
    </div>
  );
}
