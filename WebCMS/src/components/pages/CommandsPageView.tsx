"use client";

import { useEffect, useState } from "react";
import type { Dictionary } from "@/lib/i18n/get-dictionary";
import { ACCOUNT_LEVELS } from "@/lib/gs/public-guide-parse";
import type { PublicCommandsGuide } from "@/lib/gs/public-commands";
import { formatAlRates } from "@/lib/gs/public-chaos-mix";

type Props = {
  t: Dictionary;
};

function enabledLabel(
  enabled: Record<0 | 1 | 2 | 3, boolean>,
  on: string,
  off: string,
  vipShort: Record<string, string>,
  join: string
): string {
  const anyOn = ACCOUNT_LEVELS.some((al) => enabled[al]);
  if (!anyOn) return off;
  const allOn = ACCOUNT_LEVELS.every((al) => enabled[al]);
  if (allOn) return on;
  return ACCOUNT_LEVELS.map((al) => {
    const label =
      vipShort[String(al) as keyof typeof vipShort] ?? `VIP ${al}`;
    return `${label} ${enabled[al] ? on : off}`;
  }).join(join);
}

function costLabel(
  row: PublicCommandsGuide["commands"][number],
  t: Dictionary["commands"],
  vipShort: Record<string, string>,
  join: string
): string {
  const parts: string[] = [];
  const moneyVals = ACCOUNT_LEVELS.map((al) => row.money[al]).filter(
    (v): v is number => v != null && v > 0
  );
  if (moneyVals.length) {
    parts.push(
      `${formatAlRates(row.money, { vipShort, join, suffix: "" })} Zen`
    );
  }
  if (row.wcoinC && row.wcoinC > 0) parts.push(`${row.wcoinC} ${t.wcoinC}`);
  if (row.wcoinP && row.wcoinP > 0) parts.push(`${row.wcoinP} WCoinP`);
  if (row.goblinPoint && row.goblinPoint > 0)
    parts.push(`${row.goblinPoint} GP`);
  return parts.length ? parts.join(", ") : "—";
}

export default function CommandsPageView({ t }: Props) {
  const c = t.commands;
  const vipShort = t.guide.mix.vipShort;
  const vipJoin = t.guide.mix.vipJoin;
  const [data, setData] = useState<PublicCommandsGuide | null>(null);
  const [loading, setLoading] = useState(true);
  const [failed, setFailed] = useState(false);

  useEffect(() => {
    let cancelled = false;
    (async () => {
      try {
        const res = await fetch("/api/guide/commands", { cache: "no-store" });
        const json = await res.json().catch(() => ({}));
        if (!res.ok || !json?.ok || !json.data) {
          if (!cancelled) setFailed(true);
          return;
        }
        if (!cancelled) {
          setData(json.data as PublicCommandsGuide);
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
      <div className="mb-8 flex flex-wrap items-end justify-between gap-4">
        <div className="max-w-2xl">
          <h1 className="section-title">{c.title}</h1>
          <p className="muted mt-2">{c.subtitle}</p>
          <p className="mt-3 text-xs text-mu-lime">{c.sourceNote}</p>
        </div>
        <p className="max-w-sm text-xs text-mu-muted">{c.note}</p>
      </div>

      {loading ? (
        <p className="text-sm text-mu-muted">{t.common.loading}</p>
      ) : null}
      {failed ? (
        <p className="text-sm text-rose-300">{c.unavailable}</p>
      ) : null}

      {/* Fallback static list if API fails */}
      {failed || (!loading && !data) ? (
        <div className="mt-6 grid gap-3 sm:grid-cols-2 lg:grid-cols-3">
          {c.items.map((item) => (
            <div
              key={item.cmd}
              className="mu-card-glow card flex items-start gap-3 p-4"
            >
              <code className="mu-cmd rounded-md border border-mu-lime/30 bg-mu-lime/10 px-2.5 py-1 text-sm font-semibold text-mu-lime">
                {item.cmd}
              </code>
              <p className="pt-0.5 text-sm text-gray-300">{item.desc}</p>
            </div>
          ))}
        </div>
      ) : null}

      {data ? (
        <div className="space-y-6">
          <section className="card p-5">
            <h2 className="panel-title">{c.listTitle}</h2>
            <div className="mt-4 overflow-x-auto">
              <table className="w-full min-w-[640px] border-collapse text-sm">
                <thead>
                  <tr className="border-b border-white/10 text-left text-[11px] uppercase tracking-[0.16em] text-mu-muted">
                    <th className="px-3 py-2">{c.colCmd}</th>
                    <th className="px-3 py-2">{c.colStatus}</th>
                    <th className="px-3 py-2">{c.colCost}</th>
                    <th className="px-3 py-2">{c.colComment}</th>
                  </tr>
                </thead>
                <tbody>
                  {data.commands.map((row) => (
                    <tr key={row.cmd} className="border-b border-white/5">
                      <td className="px-3 py-2.5">
                        <code className="rounded border border-mu-lime/30 bg-mu-lime/10 px-2 py-0.5 text-sm font-semibold text-mu-lime">
                          {row.cmd.replace(/^\//, "")}
                        </code>
                      </td>
                      <td className="px-3 py-2.5 text-xs text-gray-300">
                        {enabledLabel(row.enabled, c.on, c.off, vipShort, vipJoin)}
                      </td>
                      <td className="px-3 py-2.5 font-mono text-xs text-mu-gold">
                        {costLabel(row, c, vipShort, vipJoin)}
                      </td>
                      <td className="px-3 py-2.5 text-xs text-gray-400">
                        {row.comment ?? "—"}
                      </td>
                    </tr>
                  ))}
                </tbody>
              </table>
            </div>
          </section>

          <div className="grid gap-6 lg:grid-cols-2">
            <section className="card p-5">
              <h2 className="panel-title">{c.resetTitle}</h2>
              <dl className="mt-4 space-y-2 text-sm">
                <div className="flex justify-between gap-3 border-b border-white/5 py-2">
                  <dt className="text-mu-muted">{c.resetLevel}</dt>
                  <dd className="font-mono text-mu-gold">
                    {formatAlRates(data.reset.level, { suffix: "", vipShort, join: vipJoin })}
                  </dd>
                </div>
                <div className="flex justify-between gap-3 border-b border-white/5 py-2">
                  <dt className="text-mu-muted">{c.resetMoney}</dt>
                  <dd className="font-mono text-mu-gold">
                    {formatAlRates(data.reset.money, { suffix: "", vipShort, join: vipJoin })} Zen
                  </dd>
                </div>
                <div className="flex justify-between gap-3 border-b border-white/5 py-2">
                  <dt className="text-mu-muted">{c.resetStart}</dt>
                  <dd className="font-mono text-mu-gold">
                    {formatAlRates(data.reset.startLevel, { suffix: "", vipShort, join: vipJoin })}
                  </dd>
                </div>
              </dl>
            </section>

            <section className="card p-5">
              <h2 className="panel-title">{c.masterTitle}</h2>
              <dl className="mt-4 space-y-2 text-sm">
                <div className="flex justify-between gap-3 border-b border-white/5 py-2">
                  <dt className="text-mu-muted">{c.masterLevel}</dt>
                  <dd className="font-mono text-mu-gold">
                    {formatAlRates(data.masterReset.level, { suffix: "", vipShort, join: vipJoin })}
                  </dd>
                </div>
                <div className="flex justify-between gap-3 border-b border-white/5 py-2">
                  <dt className="text-mu-muted">{c.masterMoney}</dt>
                  <dd className="font-mono text-mu-gold">
                    {formatAlRates(data.masterReset.money, { suffix: "", vipShort, join: vipJoin })} Zen
                  </dd>
                </div>
                <div className="flex justify-between gap-3 border-b border-white/5 py-2">
                  <dt className="text-mu-muted">{c.masterPoint}</dt>
                  <dd className="font-mono text-mu-gold">
                    {formatAlRates(data.masterReset.point, { suffix: "", vipShort, join: vipJoin })}
                  </dd>
                </div>
              </dl>
            </section>
          </div>

          <section className="card p-5">
            <h2 className="panel-title">{c.otherTitle}</h2>
            <dl className="mt-4 grid gap-2 sm:grid-cols-2 text-sm">
              <div className="flex justify-between gap-3 border-b border-white/5 py-2">
                <dt className="text-mu-muted">{c.wareNumber}</dt>
                <dd className="font-mono text-mu-gold">
                  {formatAlRates(data.wareNumber, { suffix: "", vipShort, join: vipJoin })}
                </dd>
              </div>
              <div className="flex justify-between gap-3 border-b border-white/5 py-2">
                <dt className="text-mu-muted">{c.marryLevel}</dt>
                <dd className="font-mono text-mu-gold">
                  {data.marryLevel ?? "—"}
                </dd>
              </div>
              <div className="flex justify-between gap-3 border-b border-white/5 py-2">
                <dt className="text-mu-muted">{c.marryCost}</dt>
                <dd className="font-mono text-mu-gold">
                  {data.marryCost ?? "—"}
                </dd>
              </div>
              <div className="flex justify-between gap-3 border-b border-white/5 py-2">
                <dt className="text-mu-muted">{c.giftLimit}</dt>
                <dd className="font-mono text-mu-gold">
                  {data.giftLimit ?? "—"}
                </dd>
              </div>
            </dl>
          </section>

          <section className="card p-5">
            <h2 className="panel-title">{c.moveTitle}</h2>
            {data.moveNote === "command-disabled-use-move-txt" ? (
              <p className="mt-2 text-sm text-amber-200/90">
                {c.moveDisabledNote}
              </p>
            ) : null}
            <div className="mt-4 overflow-x-auto">
              <table className="w-full min-w-[360px] border-collapse text-sm">
                <thead>
                  <tr className="border-b border-white/10 text-left text-[11px] uppercase tracking-[0.16em] text-mu-muted">
                    <th className="px-3 py-2">{c.colMap}</th>
                    <th className="px-3 py-2 text-center">{c.colMinLevel}</th>
                    <th className="px-3 py-2 text-center">{c.colMoney}</th>
                  </tr>
                </thead>
                <tbody>
                  {data.moves.map((m) => (
                    <tr key={m.map} className="border-b border-white/5">
                      <td className="px-3 py-2 text-gray-200">{m.map}</td>
                      <td className="px-3 py-2 text-center font-mono text-mu-lime">
                        {m.minLevel ?? "—"}
                      </td>
                      <td className="px-3 py-2 text-center font-mono text-mu-gold">
                        {m.money ?? "—"}
                      </td>
                    </tr>
                  ))}
                </tbody>
              </table>
            </div>
          </section>
        </div>
      ) : null}
    </div>
  );
}
