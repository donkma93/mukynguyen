"use client";

import Link from "next/link";
import { useEffect, useMemo, useState } from "react";
import type { Dictionary } from "@/lib/i18n/get-dictionary";
import type { PublicEventsGuide } from "@/lib/gs/public-events";
import {
  humanizeDayKey,
  humanizeSchedulePattern,
  humanizeScheduleTime,
  scheduleCopyFromDict,
} from "@/lib/guide/plain-copy";

type Props = {
  t: Dictionary;
};

type TabId = "info" | "events" | "top";

export default function HomeServerInfoTabs({ t }: Props) {
  const L = t.home.landing;
  const e = t.events;
  const scheduleCopy = useMemo(() => scheduleCopyFromDict(e), [e]);
  const [tab, setTab] = useState<TabId>("info");
  const [liveLines, setLiveLines] = useState<string[] | null>(null);

  const tabs = useMemo(
    () =>
      [
        { id: "info" as const, label: L.tabInfo },
        { id: "events" as const, label: L.tabEvents },
        { id: "top" as const, label: L.tabTop },
      ] as const,
    [L.tabEvents, L.tabInfo, L.tabTop],
  );

  useEffect(() => {
    let cancelled = false;
    (async () => {
      try {
        const res = await fetch("/api/guide/events", { cache: "no-store" });
        const json = await res.json().catch(() => ({}));
        if (!res.ok || !json?.ok || !json.data) return;
        const data = json.data as PublicEventsGuide;
        const labels = e.scheduleLabels as Record<string, string>;
        const lines: string[] = [];
        for (const ev of data.schedules?.classic ?? []) {
          if (ev.empty || !ev.times.length) continue;
          const name = labels[ev.id] ?? ev.id;
          const cadence = humanizeSchedulePattern(ev.pattern, scheduleCopy);
          if (cadence) {
            lines.push(`${name}: ${cadence}`);
            continue;
          }
          const sampleTimes = (
            ev.times.length <= 4 ? ev.times : ev.times.slice(0, 3)
          ).map((tm) => humanizeScheduleTime(tm, scheduleCopy));
          const more =
            ev.times.length > 4
              ? `, ${e.scheduleMore.replace("{count}", String(ev.times.length - 3))}`
              : "";
          lines.push(`${name}: ${sampleTimes.join(", ")}${more}`);
        }
        const cry = data.schedules?.crywolf;
        if (cry && !cry.empty) {
          const cryBits = cry.entries.map((x) => {
            const day = humanizeDayKey(x.dayLabel, scheduleCopy);
            return day ? `${day} ${x.timeLabel}` : x.timeLabel;
          });
          lines.push(`${e.crywolfTitle}: ${[...new Set(cryBits)].join(", ")}`);
        }
        if (data.ctcMini?.hour != null && data.ctcMini.minute != null) {
          const hm = `${String(data.ctcMini.hour).padStart(2, "0")}:${String(
            data.ctcMini.minute
          ).padStart(2, "0")}`;
          lines.push(`${e.ctcTitle}: ${hm}`);
        }
        if (data.schedules?.invasions?.length) {
          lines.push(
            `${e.invasionTitle}: ${data.schedules.invasions.length} boss`
          );
        }
        if (!cancelled && lines.length) setLiveLines(lines);
      } catch {
        // keep static landing items
      }
    })();
    return () => {
      cancelled = true;
    };
  }, [
    e.crywolfTitle,
    e.ctcTitle,
    e.invasionTitle,
    e.scheduleLabels,
    e.scheduleMore,
    scheduleCopy,
  ]);

  const eventItems = liveLines?.length ? liveLines : L.eventItems;

  return (
    <section className="relative overflow-hidden border-y border-mu-gold/15 py-16 md:py-20">
      <div
        className="pointer-events-none absolute inset-0 bg-cover bg-center opacity-35"
        style={{ backgroundImage: "url(/assets/landing/ui/server-tab-bg.jpg)" }}
      />
      <div className="pointer-events-none absolute inset-0 bg-gradient-to-b from-black/85 via-black/70 to-black/90" />

      <div className="relative mx-auto max-w-6xl px-4">
        <div className="mb-8 text-center">
          <span className="badge-gold mb-3 inline-flex items-center gap-2">
            <span className="h-1.5 w-1.5 rounded-full bg-mu-lime shadow-[0_0_8px_rgba(120,255,20,0.8)]" />
            {L.serverBadge}
            <span className="h-1.5 w-1.5 rounded-full bg-mu-lime shadow-[0_0_8px_rgba(120,255,20,0.8)]" />
          </span>
          <h2 className="section-title mt-3">{L.serverTitle}</h2>
          <div className="mx-auto mt-4 flex max-w-xs items-center gap-3">
            <span className="h-px flex-1 bg-gradient-to-r from-transparent to-mu-gold/70" />
            <span className="text-mu-gold">◆</span>
            <span className="h-px flex-1 bg-gradient-to-l from-transparent to-mu-gold/70" />
          </div>
        </div>

        <div className="mb-6 flex flex-wrap justify-center gap-2">
          {tabs.map((item) => (
            <button
              key={item.id}
              type="button"
              onClick={() => setTab(item.id)}
              className={tab === item.id ? "mu-sv-tab mu-sv-tab-active" : "mu-sv-tab"}
            >
              {item.label}
            </button>
          ))}
        </div>

        {tab === "info" && (
          <div className="grid gap-4 lg:grid-cols-2">
            <Panel title={L.infoTitle} items={L.infoItems} />
            <Panel title={L.rewardTitle} items={L.rewardItems} />
            <div className="flex flex-wrap gap-2 lg:col-span-2">
              <Link href="/commands" className="btn-ghost">
                {L.viewCommands}
              </Link>
              <Link href="/gameplay" className="btn-ghost">
                {t.nav.gameplay}
              </Link>
            </div>
          </div>
        )}

        {tab === "events" && (
          <div className="grid gap-4">
            <Panel title={L.eventTitle} items={eventItems} wide />
            <div>
              <Link href="/events" className="btn-gold">
                {L.viewEvents}
              </Link>
            </div>
          </div>
        )}

        {tab === "top" && (
          <div className="grid gap-4">
            <Panel title={L.topTitle} items={L.topItems} wide />
            <div>
              <Link href="/ranking" className="btn-lime">
                {L.viewRanking}
              </Link>
            </div>
          </div>
        )}
      </div>
    </section>
  );
}

function Panel({
  title,
  items,
}: {
  title: string;
  items: readonly string[];
  wide?: boolean;
}) {
  return (
    <div className="mu-sv-panel rounded-2xl p-5 md:p-6">
      <div className="mb-4 flex items-center gap-3 border-b border-mu-gold/20 pb-3">
        <span className="flex h-8 w-8 items-center justify-center rounded-full border border-mu-gold/40 bg-mu-gold/10 text-sm text-mu-gold">
          ★
        </span>
        <h3 className="panel-title">{title}</h3>
      </div>
      <ul>
        {items.map((text, idx) => (
          <li key={`${idx}-${text.slice(0, 24)}`} className="mu-sv-item">
            <span className="num">★</span>
            <span className="txt">{text}</span>
          </li>
        ))}
      </ul>
    </div>
  );
}
