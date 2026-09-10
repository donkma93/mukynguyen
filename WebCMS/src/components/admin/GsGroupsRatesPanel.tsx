"use client";

import Link from "next/link";
import { useCallback, useEffect, useState } from "react";
import FlashMessage from "@/components/FlashMessage";

type GroupRates = {
  itemDropRate: number | null;
  moneyDropRate: number | null;
  jewelDropRate: number | null;
};

type SubAvail = {
  id: string;
  label: string;
  group: "normal" | "vip";
  folder?: string;
};

type Props = {
  labels: {
    title: string;
    subtitle: string;
    normal: string;
    vip: string;
    itemDrop: string;
    jewelDrop: string;
    moneyDrop: string;
    jewelHint: string;
    save: string;
    saving: string;
    available: string;
    missing: string;
    toOps: string;
  };
};

export default function GsGroupsRatesPanel({ labels }: Props) {
  const [rates, setRates] = useState<{
    normal: GroupRates;
    vip: GroupRates;
  } | null>(null);
  const [draft, setDraft] = useState<{
    normal: { itemDropRate: string; moneyDropRate: string };
    vip: { itemDropRate: string; moneyDropRate: string };
  }>({
    normal: { itemDropRate: "", moneyDropRate: "" },
    vip: { itemDropRate: "", moneyDropRate: "" },
  });
  const [available, setAvailable] = useState<SubAvail[]>([]);
  const [missing, setMissing] = useState<SubAvail[]>([]);
  const [error, setError] = useState<string | null>(null);
  const [message, setMessage] = useState<string | null>(null);
  const [loading, setLoading] = useState(true);
  const [saving, setSaving] = useState<"normal" | "vip" | null>(null);

  const applySnapshot = useCallback(
    (data: {
      rates: { normal: GroupRates; vip: GroupRates };
      available?: SubAvail[];
      missing?: SubAvail[];
    }) => {
      setRates(data.rates);
      setDraft({
        normal: {
          itemDropRate: String(data.rates.normal.itemDropRate ?? ""),
          moneyDropRate: String(data.rates.normal.moneyDropRate ?? ""),
        },
        vip: {
          itemDropRate: String(data.rates.vip.itemDropRate ?? ""),
          moneyDropRate: String(data.rates.vip.moneyDropRate ?? ""),
        },
      });
      setAvailable(data.available || []);
      setMissing(data.missing || []);
    },
    []
  );

  const load = useCallback(async () => {
    setLoading(true);
    setError(null);
    try {
      const res = await fetch("/api/admin/gs/groups", { cache: "no-store" });
      const data = await res.json().catch(() => ({}));
      if (!res.ok || !data?.ok) {
        setError(data.error || "Không đọc được nhóm GS");
        return;
      }
      applySnapshot(data);
    } catch {
      setError("Không kết nối được API nhóm GS");
    } finally {
      setLoading(false);
    }
  }, [applySnapshot]);

  useEffect(() => {
    void load();
  }, [load]);

  async function onSave(group: "normal" | "vip") {
    setSaving(group);
    setError(null);
    setMessage(null);
    try {
      const body = {
        group,
        itemDropRate: draft[group].itemDropRate,
        moneyDropRate: draft[group].moneyDropRate,
      };
      const res = await fetch("/api/admin/gs/groups", {
        method: "PUT",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(body),
      });
      const data = await res.json().catch(() => ({}));
      if (!res.ok || !data?.ok) {
        setError(data.error || "Lưu thất bại");
        return;
      }
      setMessage(data.message || "Đã lưu.");
      if (data.rates) applySnapshot(data);
      else await load();
    } catch {
      setError("Không kết nối được API lưu nhóm GS");
    } finally {
      setSaving(null);
    }
  }

  function renderGroupCard(group: "normal" | "vip", title: string) {
    const members = [...available, ...missing].filter((s) => s.group === group);
    const present = available.filter((s) => s.group === group).map((s) => s.label);
    const absent = missing.filter((s) => s.group === group).map((s) => s.label);

    return (
      <section className="card p-5 space-y-4">
        <div>
          <h2 className="panel-title">{title}</h2>
          <p className="muted mt-1 text-xs">
            {present.length
              ? `${labels.available}: ${present.join(", ")}`
              : `${labels.available}: —`}
            {absent.length ? ` · ${labels.missing}: ${absent.join(", ")}` : ""}
          </p>
        </div>

        <div className="grid gap-3 sm:grid-cols-2">
          <div>
            <label className="label">{labels.itemDrop}</label>
            <input
              className="input"
              type="number"
              min={0}
              step={1}
              value={draft[group].itemDropRate}
              onChange={(e) =>
                setDraft((d) => ({
                  ...d,
                  [group]: { ...d[group], itemDropRate: e.target.value },
                }))
              }
            />
          </div>
          <div>
            <label className="label">{labels.moneyDrop}</label>
            <input
              className="input"
              type="number"
              min={0}
              step={1}
              value={draft[group].moneyDropRate}
              onChange={(e) =>
                setDraft((d) => ({
                  ...d,
                  [group]: { ...d[group], moneyDropRate: e.target.value },
                }))
              }
            />
          </div>
        </div>

        <p className="text-xs text-mu-muted">
          {labels.jewelDrop}:{" "}
          <span className="font-mono text-mu-gold">
            {draft[group].itemDropRate || "—"}%
          </span>{" "}
          — {labels.jewelHint}
        </p>

        {members.length === 0 ? (
          <p className="text-xs text-amber-200">Chưa có Sub nào trong nhóm này.</p>
        ) : null}

        <button
          type="button"
          className="btn-gold"
          disabled={saving != null || loading}
          onClick={() => void onSave(group)}
        >
          {saving === group ? labels.saving : labels.save}
        </button>
      </section>
    );
  }

  return (
    <div className="space-y-6">
      <div>
        <h1 className="section-title">{labels.title}</h1>
        <p className="muted mt-1">{labels.subtitle}</p>
      </div>

      <FlashMessage type="error" message={error} />
      <FlashMessage type="success" message={message} />

      {loading && !rates ? (
        <p className="text-sm text-mu-muted">Đang tải…</p>
      ) : (
        <div className="grid gap-4 lg:grid-cols-2">
          {renderGroupCard("normal", labels.normal)}
          {renderGroupCard("vip", labels.vip)}
        </div>
      )}

      <div className="flex flex-wrap gap-2">
        <Link href="/admin/gs/ini" className="btn-ghost">
          GS INI
        </Link>
        <Link href="/admin/ops" className="btn-lime">
          {labels.toOps}
        </Link>
      </div>
    </div>
  );
}
