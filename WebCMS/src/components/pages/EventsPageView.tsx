"use client";

import { useEffect, useMemo, useState } from "react";
import type { Dictionary } from "@/lib/i18n/get-dictionary";
import {
  formatHm,
  type PublicEventsGuide,
} from "@/lib/gs/public-events";
import {
  humanizeDayKey,
  humanizeSchedulePattern,
  humanizeScheduleTime,
  scheduleCopyFromDict,
  type ScheduleCopy,
} from "@/lib/guide/plain-copy";

type Props = {
  t: Dictionary;
};

const eventTone = [
  "badge-danger",
  "badge-lime",
  "badge-gold",
  "badge-purple",
  "badge-gold",
  "badge-lime",
];

function StatusBadge({
  enabled,
  t,
}: {
  enabled: boolean | null;
  t: Dictionary["events"];
}) {
  if (enabled == null) {
    return <span className="badge-purple">{t.unknown}</span>;
  }
  return enabled ? (
    <span className="badge-lime">{t.on}</span>
  ) : (
    <span className="badge-danger">{t.off}</span>
  );
}

function TimeChips({
  times,
  copy,
  moreLabel,
}: {
  times: string[];
  copy: ScheduleCopy;
  moreLabel: string;
}) {
  if (!times.length) return null;
  const shown = times.length > 14 ? times.slice(0, 12) : times;
  const rest = times.length - shown.length;
  return (
    <div className="mt-2 flex flex-wrap gap-1.5">
      {shown.map((tm) => (
        <span
          key={tm}
          className="rounded-md border border-mu-gold/30 bg-black/40 px-2 py-0.5 text-xs text-mu-gold"
        >
          {humanizeScheduleTime(tm, copy)}
        </span>
      ))}
      {rest > 0 ? (
        <span className="rounded-md border border-white/10 bg-black/30 px-2 py-0.5 text-xs text-mu-muted">
          {moreLabel.replace("{count}", String(rest))}
        </span>
      ) : null}
    </div>
  );
}

export default function EventsPageView({ t }: Props) {
  const e = t.events;
  const scheduleCopy = useMemo(() => scheduleCopyFromDict(e), [e]);
  const [data, setData] = useState<PublicEventsGuide | null>(null);
  const [loading, setLoading] = useState(true);
  const [failed, setFailed] = useState(false);

  useEffect(() => {
    let cancelled = false;
    (async () => {
      try {
        const res = await fetch("/api/guide/events", { cache: "no-store" });
        const json = await res.json().catch(() => ({}));
        if (!res.ok || !json?.ok || !json.data) {
          if (!cancelled) setFailed(true);
          return;
        }
        if (!cancelled) {
          setData(json.data as PublicEventsGuide);
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

  const flagLabel = (id: string) =>
    (e.flagLabels as Record<string, string>)[id] ?? id;

  const scheduleLabel = (id: string) =>
    (e.scheduleLabels as Record<string, string>)[id] ??
    (e.flagLabels as Record<string, string>)[id] ??
    id;

  const flagById = new Map(data?.flags.map((f) => [f.id, f.enabled]) ?? []);

  return (
    <div className="relative overflow-hidden">
      <section className="relative border-b border-white/5 bg-gradient-to-b from-mu-purple/10 via-transparent to-black/30 py-12 md:py-16">
        <div className="pointer-events-none absolute inset-x-0 top-0 h-px mu-section-line" />
        <div className="mx-auto max-w-6xl px-4">
          <h1 className="section-title">{e.title}</h1>
          <p className="muted mt-2 max-w-3xl">{e.subtitle}</p>
          <p className="mt-3 text-xs text-mu-lime">{e.sourceNote}</p>

          {loading ? (
            <p className="mt-8 text-sm text-mu-muted">{t.common.loading}</p>
          ) : null}
          {failed ? (
            <p className="mt-8 text-sm text-rose-300">{e.unavailable}</p>
          ) : null}

          {data ? (
            <div className="mt-8 space-y-6">
              <section className="card p-5">
                <h2 className="panel-title">{e.liveTitle}</h2>
                <div className="mt-4 grid gap-2 sm:grid-cols-2 lg:grid-cols-5">
                  {data.flags.map((flag) => (
                    <div
                      key={flag.id}
                      className="flex items-center justify-between gap-2 rounded-lg border border-white/10 bg-black/25 px-3 py-2.5"
                    >
                      <span className="text-sm text-gray-200">
                        {flagLabel(flag.id)}
                      </span>
                      <StatusBadge enabled={flag.enabled} t={e} />
                    </div>
                  ))}
                </div>
                <p className="mt-4 text-xs leading-relaxed text-mu-muted">
                  {e.classicNote}
                </p>
              </section>

              {data.schedules?.classic?.length ? (
                <section className="card p-5">
                  <h2 className="panel-title">{e.scheduleTitle}</h2>
                  <p className="mt-1 text-xs text-mu-muted">{e.scheduleHint}</p>
                  <div className="mt-4 grid gap-4 md:grid-cols-2">
                    {data.schedules.classic.map((ev) => (
                      <article
                        key={ev.id}
                        className="rounded-xl border border-white/10 bg-black/25 p-4"
                      >
                        <div className="flex flex-wrap items-center justify-between gap-2">
                          <h3 className="font-display text-lg text-mu-gold">
                            {scheduleLabel(ev.id)}
                          </h3>
                          {flagById.has(ev.id) ? (
                            <StatusBadge
                              enabled={flagById.get(ev.id) ?? null}
                              t={e}
                            />
                          ) : null}
                        </div>
                        {ev.empty ? (
                          <p className="mt-3 text-sm text-rose-300/90">
                            {e.scheduleEmpty}
                          </p>
                        ) : (
                          <>
                            {humanizeSchedulePattern(ev.pattern, scheduleCopy) ? (
                              <p className="mt-2 text-xs text-mu-muted">
                                {e.schedulePattern}:{" "}
                                <span className="text-mu-lime">
                                  {humanizeSchedulePattern(ev.pattern, scheduleCopy)}
                                </span>
                              </p>
                            ) : null}
                            <p className="mt-2 text-[11px] uppercase tracking-wider text-mu-muted">
                              {e.scheduleTimes}
                            </p>
                            <TimeChips
                              times={ev.times}
                              copy={scheduleCopy}
                              moreLabel={e.scheduleMore}
                            />
                            <div className="mt-3 flex flex-wrap gap-3 text-xs text-mu-muted">
                              {ev.durationMinutes != null ? (
                                <span>
                                  {e.scheduleDuration}:{" "}
                                  <span className="text-gray-200">
                                    {ev.durationMinutes} {e.ctcMinutes}
                                  </span>
                                </span>
                              ) : null}
                              {ev.timing?.warningMinutes != null ? (
                                <span>
                                  {e.scheduleWarning}:{" "}
                                  <span className="text-gray-200">
                                    {ev.timing.warningMinutes} {e.ctcMinutes}
                                  </span>
                                </span>
                              ) : null}
                              {ev.timing?.notifyMinutes != null ? (
                                <span>
                                  {e.scheduleNotify}:{" "}
                                  <span className="text-gray-200">
                                    {ev.timing.notifyMinutes} {e.ctcMinutes}
                                  </span>
                                </span>
                              ) : null}
                              {ev.timing?.closeMinutes != null ? (
                                <span>
                                  {e.scheduleClose}:{" "}
                                  <span className="text-gray-200">
                                    {ev.timing.closeMinutes} {e.ctcMinutes}
                                  </span>
                                </span>
                              ) : null}
                            </div>
                          </>
                        )}
                      </article>
                    ))}
                  </div>

                  {data.schedules.crywolf &&
                  !data.schedules.crywolf.empty ? (
                    <div className="mt-4 rounded-xl border border-white/10 bg-black/25 p-4">
                      <div className="flex flex-wrap items-center justify-between gap-2">
                        <h3 className="font-display text-lg text-mu-gold">
                          {e.crywolfTitle}
                        </h3>
                        <StatusBadge
                          enabled={flagById.get("crywolf") ?? null}
                          t={e}
                        />
                      </div>
                      <ul className="mt-3 space-y-2">
                        {data.schedules.crywolf.entries.map((entry, i) => (
                          <li
                            key={`${entry.timeLabel}-${i}`}
                            className="flex flex-wrap items-center gap-3 text-sm text-gray-300"
                          >
                            {humanizeDayKey(entry.dayLabel, scheduleCopy) ? (
                              <span className="text-mu-muted">
                                {humanizeDayKey(entry.dayLabel, scheduleCopy)}
                              </span>
                            ) : null}
                            <span className="text-mu-gold">{entry.timeLabel}</span>
                            {entry.continuanceMinutes != null ? (
                              <span className="text-xs text-mu-muted">
                                {e.crywolfDuration}:{" "}
                                <span className="text-gray-200">
                                  {entry.continuanceMinutes} {e.ctcMinutes}
                                </span>
                              </span>
                            ) : null}
                          </li>
                        ))}
                      </ul>
                    </div>
                  ) : null}
                </section>
              ) : null}

              {data.schedules?.invasions?.length ? (
                <section className="card p-5">
                  <h2 className="panel-title">{e.invasionTitle}</h2>
                  <p className="mt-1 text-xs text-mu-muted">{e.invasionHint}</p>
                  <div className="mt-4 grid gap-3 sm:grid-cols-2 lg:grid-cols-3">
                    {data.schedules.invasions.map((boss) => (
                      <article
                        key={boss.index}
                        className="rounded-xl border border-white/10 bg-black/25 p-3.5"
                      >
                        <h3 className="text-sm font-medium text-gray-100">
                          {boss.name}
                        </h3>
                        {humanizeSchedulePattern(boss.pattern, scheduleCopy) ? (
                          <p className="mt-1 text-[11px] text-mu-lime">
                            {humanizeSchedulePattern(boss.pattern, scheduleCopy)}
                          </p>
                        ) : null}
                        <TimeChips
                          times={boss.times}
                          copy={scheduleCopy}
                          moreLabel={e.scheduleMore}
                        />
                      </article>
                    ))}
                  </div>
                </section>
              ) : null}

              {data.ctcMini ? (
                <section className="card p-5">
                  <div className="mb-3 flex flex-wrap items-center justify-between gap-2">
                    <h2 className="panel-title">{e.ctcTitle}</h2>
                    <StatusBadge enabled={data.ctcMini.enabled} t={e} />
                  </div>
                  <div className="grid gap-3 sm:grid-cols-2 lg:grid-cols-3">
                    <div className="rounded-lg border border-white/10 bg-black/20 px-3 py-3">
                      <p className="text-[11px] uppercase tracking-wider text-mu-muted">
                        {e.ctcTime}
                      </p>
                      <p className="mt-1 font-mono text-lg text-mu-gold">
                        {formatHm(data.ctcMini.hour, data.ctcMini.minute) ??
                          "—"}
                      </p>
                    </div>
                    <div className="rounded-lg border border-white/10 bg-black/20 px-3 py-3">
                      <p className="text-[11px] uppercase tracking-wider text-mu-muted">
                        {e.ctcPrepare}
                      </p>
                      <p className="mt-1 font-mono text-lg text-gray-200">
                        {data.ctcMini.prepareMinutes ?? "—"} {e.ctcMinutes}
                      </p>
                    </div>
                    <div className="rounded-lg border border-white/10 bg-black/20 px-3 py-3">
                      <p className="text-[11px] uppercase tracking-wider text-mu-muted">
                        {e.ctcDuration}
                      </p>
                      <p className="mt-1 font-mono text-lg text-gray-200">
                        {data.ctcMini.eventMinutes ?? "—"} {e.ctcMinutes}
                      </p>
                    </div>
                    <div className="rounded-lg border border-white/10 bg-black/20 px-3 py-3">
                      <p className="text-[11px] uppercase tracking-wider text-mu-muted">
                        {e.ctcGate}
                      </p>
                      <p className="mt-1 font-mono text-lg text-mu-lime">
                        {data.ctcMini.gateRewardWCoin ?? "—"} {e.ctcWcoin}
                      </p>
                    </div>
                    <div className="rounded-lg border border-white/10 bg-black/20 px-3 py-3">
                      <p className="text-[11px] uppercase tracking-wider text-mu-muted">
                        {e.ctcTower}
                      </p>
                      <p className="mt-1 font-mono text-lg text-mu-lime">
                        {data.ctcMini.towerRewardWCoin ?? "—"} {e.ctcWcoin}
                      </p>
                    </div>
                    <div className="rounded-lg border border-white/10 bg-black/20 px-3 py-3">
                      <p className="text-[11px] uppercase tracking-wider text-mu-muted">
                        {e.ctcWin}
                      </p>
                      <p className="mt-1 font-mono text-lg text-mu-lime">
                        {data.ctcMini.guildWinWCoin ?? "—"} {e.ctcWcoin}
                      </p>
                    </div>
                  </div>
                </section>
              ) : null}

              {data.customEventDrop ? (
                <section className="card p-5">
                  <div className="mb-3 flex flex-wrap items-center justify-between gap-2">
                    <h2 className="panel-title">{e.customDropTitle}</h2>
                    <StatusBadge
                      enabled={data.customEventDrop.switchOn}
                      t={e}
                    />
                  </div>
                  <p className="text-sm text-gray-300">
                    {data.customEventDrop.switchOn
                      ? e.customDropOn
                      : e.customDropOff}
                  </p>
                  {data.customEventDrop.times.length ? (
                    <p className="mt-3 text-sm text-gray-300">
                      <span className="text-mu-muted">
                        {e.customDropTimes}:{" "}
                      </span>
                      <span className="text-mu-gold">
                        {data.customEventDrop.times
                          .map(
                            (tm) =>
                              formatHm(tm.hour, tm.minute) ??
                              `${tm.hour}:${tm.minute}`
                          )
                          .join(", ")}
                      </span>
                    </p>
                  ) : null}
                  {data.customEventDrop.map != null ? (
                    <p className="mt-2 text-sm text-gray-300">
                      <span className="text-mu-muted">
                        {e.customDropMap}:{" "}
                      </span>
                      Map {data.customEventDrop.map}
                      {data.customEventDrop.x != null &&
                      data.customEventDrop.y != null
                        ? ` (${data.customEventDrop.x}, ${data.customEventDrop.y})`
                        : ""}
                      {data.customEventDrop.name
                        ? ` — ${data.customEventDrop.name}`
                        : ""}
                    </p>
                  ) : null}
                  {data.customEventDrop.drops.length ? (
                    <div className="mt-4">
                      <p className="mb-2 text-[11px] uppercase tracking-wider text-mu-muted">
                        {e.customDropLoot}
                      </p>
                      <ul className="grid gap-1.5 sm:grid-cols-2">
                        {data.customEventDrop.drops.map((d, i) => (
                          <li
                            key={`${d.itemIndex}-${d.itemLevel}-${i}`}
                            className="rounded-md border border-white/10 bg-black/20 px-3 py-2 text-sm text-gray-300"
                          >
                            {d.comment ?? `Item #${d.itemIndex}`}
                            {d.itemLevel > 0 ? ` +${d.itemLevel}` : ""}
                            <span className="text-mu-muted">
                              {" "}
                              ×{d.dropCount}
                            </span>
                          </li>
                        ))}
                      </ul>
                    </div>
                  ) : null}
                </section>
              ) : null}

              {data.bags?.length ? (
                <section className="card p-5">
                  <h2 className="panel-title">{e.bagsTitle}</h2>
                  <p className="mt-1 text-xs text-mu-muted">{e.bagsHint}</p>
                  <div className="mt-4 grid gap-4 md:grid-cols-2">
                    {data.bags.map((bag) => (
                      <article
                        key={bag.id}
                        className="rounded-xl border border-white/10 bg-black/25 p-4"
                      >
                        <h3 className="font-display text-lg text-mu-gold">
                          {bag.eventName ?? bag.label}
                        </h3>
                        <div className="mt-2 flex flex-wrap gap-3 text-xs text-mu-muted">
                          {bag.dropZen != null ? (
                            <span>
                              {e.bagsZen}:{" "}
                              <span className="font-mono text-mu-lime">
                                {bag.dropZen}
                              </span>
                            </span>
                          ) : null}
                          {bag.itemDropRate != null ? (
                            <span>
                              {e.bagsRate}:{" "}
                              <span className="font-mono text-mu-gold">
                                {bag.itemDropRate}
                              </span>
                            </span>
                          ) : null}
                        </div>
                        {bag.rewards.length ? (
                          <div className="mt-3">
                            <p className="mb-1.5 text-[10px] uppercase tracking-wider text-mu-muted">
                              {e.bagsRewards}
                            </p>
                            <ul className="space-y-1">
                              {bag.rewards.slice(0, 8).map((r, i) => (
                                <li
                                  key={`${bag.id}-${r.name}-${i}`}
                                  className="flex items-center justify-between gap-2 text-sm text-gray-300"
                                >
                                  <span>{r.name}</span>
                                  <span className="text-xs text-mu-gold">
                                    {r.rate != null ? `${r.rate}` : "—"}
                                    {r.minLevel != null || r.maxLevel != null
                                      ? `, ${
                                          r.maxLevel != null &&
                                          r.maxLevel !== r.minLevel
                                            ? e.bagsPlusRange
                                                .replace(
                                                  "{min}",
                                                  String(r.minLevel ?? 0)
                                                )
                                                .replace(
                                                  "{max}",
                                                  String(r.maxLevel)
                                                )
                                            : e.bagsPlusLevel.replace(
                                                "{min}",
                                                String(r.minLevel ?? 0)
                                              )
                                        }`
                                      : ""}
                                  </span>
                                </li>
                              ))}
                            </ul>
                          </div>
                        ) : null}
                      </article>
                    ))}
                  </div>
                </section>
              ) : null}
            </div>
          ) : null}

          <div className="mt-10 grid gap-4 md:grid-cols-2 lg:grid-cols-3">
            {e.items.map((event, idx) => (
              <article key={event.name} className="mu-card-glow card p-5">
                <div className="mb-3 flex items-center justify-between gap-3">
                  <h2 className="panel-title">{event.name}</h2>
                  <span className={eventTone[idx % eventTone.length]}>
                    {event.tag}
                  </span>
                </div>
                <p className="text-sm leading-relaxed text-gray-300">
                  {event.body}
                </p>
              </article>
            ))}
          </div>
        </div>
      </section>
    </div>
  );
}
