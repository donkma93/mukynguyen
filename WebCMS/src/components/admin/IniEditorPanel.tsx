"use client";

import Link from "next/link";
import { useCallback, useEffect, useMemo, useState } from "react";
import FlashMessage from "@/components/FlashMessage";
import { getIniAccountLabel, getIniEntryLabel } from "@/lib/gs/ini-labels";

type IniListItem = { name: string; slug: string; size: number; mtime: string };
type IniEntry = { key: string; value: string; section: string; description?: string };

type Props = {
  initialSlug?: string;
};

const fileLabels: Record<string, string> = {
  common: "Thiết lập chung, ngọc và Luck",
  chaosmix: "Tỉ lệ ép đồ & Chaos Mix",
  character: "Nhân vật, level và reset",
  command: "Lệnh trong game",
  custom: "Tính năng tùy chỉnh",
  event: "Sự kiện",
  skill: "Kỹ năng",
};

function readableFileLabel(targetSlug: string, fallback = "") {
  return fileLabels[targetSlug] || fallback || "Cấu hình GameServer";
}

function readableEntry(entry: IniEntry) {
  const label = getIniEntryLabel(entry.key, entry.description || "");
  const account = getIniAccountLabel(entry.key);
  return { label, account };
}

export default function IniEditorPanel({ initialSlug }: Props) {
  const [items, setItems] = useState<IniListItem[]>([]);
  const [slug, setSlug] = useState(initialSlug || "chaosmix");
  const [fileName, setFileName] = useState("");
  const [entries, setEntries] = useState<IniEntry[]>([]);
  const [draft, setDraft] = useState<Record<string, string>>({});
  const [raw, setRaw] = useState("");
  const [mode, setMode] = useState<"form" | "raw">("form");
  const [filter, setFilter] = useState("");
  const [startupKeys, setStartupKeys] = useState<string[]>([]);
  const [error, setError] = useState<string | null>(null);
  const [message, setMessage] = useState<string | null>(null);
  const [loading, setLoading] = useState(false);
  const [saving, setSaving] = useState(false);

  const loadList = useCallback(async () => {
    const res = await fetch("/api/admin/gs/ini", { cache: "no-store" });
    const data = await res.json().catch(() => ({}));
    if (res.ok && Array.isArray(data.items)) {
      setItems(data.items);
      if (!initialSlug && data.items[0]?.slug) {
        setSlug((cur) => cur || data.items[0].slug);
      }
    }
  }, [initialSlug]);

  const loadFile = useCallback(async (targetSlug: string) => {
    if (!targetSlug) return;
    setLoading(true);
    setError(null);
    setMessage(null);
    try {
      const res = await fetch(`/api/admin/gs/ini/${encodeURIComponent(targetSlug)}`, {
        cache: "no-store",
      });
      const data = await res.json().catch(() => ({}));
      if (!res.ok) {
        setError(data.error || "Không đọc được INI");
        return;
      }
      setFileName(String(data.fileName || ""));
      setRaw(String(data.raw || ""));
      const list = (data.entries || []) as IniEntry[];
      setEntries(list);
      const nextDraft: Record<string, string> = {};
      for (const e of list) nextDraft[e.key] = e.value;
      setDraft(nextDraft);
      setStartupKeys((data.startupKeys || []) as string[]);
    } catch {
      setError("Không kết nối được API INI");
    } finally {
      setLoading(false);
    }
  }, []);

  useEffect(() => {
    void loadList();
  }, [loadList]);

  useEffect(() => {
    void loadFile(slug);
  }, [slug, loadFile]);

  const filtered = useMemo(() => {
    const q = filter.trim().toLowerCase();
    if (!q) return entries;
    return entries.filter(
      (entry) => {
        const { label, account } = readableEntry(entry);
        return (
          entry.key.toLowerCase().includes(q) ||
          label.toLowerCase().includes(q) ||
          (account || "").toLowerCase().includes(q) ||
          (entry.description || "").toLowerCase().includes(q) ||
          entry.value.toLowerCase().includes(q)
        );
      }
    );
  }, [entries, filter]);

  const dirtyKeys = useMemo(() => {
    const dirty: string[] = [];
    for (const e of entries) {
      if ((draft[e.key] ?? "") !== e.value) dirty.push(e.key);
    }
    return dirty;
  }, [entries, draft]);

  async function onSave() {
    setSaving(true);
    setError(null);
    setMessage(null);
    try {
      let body: Record<string, unknown>;
      if (mode === "raw") {
        body = { mode: "raw", raw };
      } else {
        const updates: Record<string, string> = {};
        for (const k of dirtyKeys) updates[k] = draft[k] ?? "";
        if (!Object.keys(updates).length) {
          setMessage("Không có thay đổi.");
          setSaving(false);
          return;
        }
        body = { mode: "keys", updates };
      }

      const res = await fetch(`/api/admin/gs/ini/${encodeURIComponent(slug)}`, {
        method: "PUT",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(body),
      });
      const data = await res.json().catch(() => ({}));
      if (!res.ok) {
        setError(data.error || "Lưu thất bại");
        return;
      }
      const changed = Array.isArray(data.changed) ? data.changed.length : 0;
      setMessage(
        data.message ||
          `Đã lưu ${changed} key. Restart GameServer để áp dụng.`
      );
      await loadFile(slug);
      await loadList();
    } catch {
      setError("Không kết nối được API lưu INI");
    } finally {
      setSaving(false);
    }
  }

  return (
    <div className="space-y-6">
      <FlashMessage type="error" message={error} />
      <FlashMessage type="success" message={message} />

      <div className="card p-5 space-y-4">
        <div className="flex flex-wrap items-end gap-3">
          <div className="min-w-[220px] flex-1">
            <label className="label">Nhóm cấu hình</label>
            <select
              className="input"
              value={slug}
              onChange={(e) => setSlug(e.target.value)}
            >
              {items.map((it) => (
                <option key={it.slug} value={it.slug}>
                  {readableFileLabel(it.slug, it.name)}
                </option>
              ))}
            </select>
          </div>
          <div className="flex gap-2">
            <button
              type="button"
              className={mode === "form" ? "btn-gold" : "btn-ghost"}
              onClick={() => setMode("form")}
            >
              Danh sách
            </button>
            <button
              type="button"
              className={mode === "raw" ? "btn-gold" : "btn-ghost"}
              onClick={() => setMode("raw")}
            >
              Nội dung kỹ thuật
            </button>
          </div>
          <Link href="/admin/enhancement-rates" className="btn-gold">
            Chỉnh tỉ lệ đập đồ
          </Link>
          <Link href="/admin/ops" className="btn-lime">
            Tới Ops / Restart
          </Link>
        </div>

        <p className="text-sm text-mu-muted">
          Đang mở: <span className="text-mu-gold">{readableFileLabel(slug, fileName)}</span>
          {loading ? " — đang tải…" : ""}
          {dirtyKeys.length > 0 && mode === "form"
            ? ` — ${dirtyKeys.length} thay đổi chưa lưu`
            : ""}
        </p>

        {startupKeys.length > 0 ? (
          <p className="rounded-lg border border-amber-500/40 bg-amber-500/10 px-3 py-2 text-xs text-amber-200">
            File có key startup-only (port / DS / JS…): {startupKeys.slice(0, 8).join(", ")}
            {startupKeys.length > 8 ? "…" : ""}. Đổi các key này nên Restart full
            stack.
          </p>
        ) : null}

        {mode === "form" ? (
          <>
            <div>
              <label className="label">Tìm mục cấu hình</label>
              <input
                className="input"
                placeholder="ví dụ: nâng cấp đồ +15 hoặc cánh cấp 2"
                value={filter}
                onChange={(e) => setFilter(e.target.value)}
              />
            </div>
            <div className="max-h-[560px] overflow-auto rounded-lg border border-white/10">
              <table className="w-full min-w-[640px] text-left text-sm">
                <thead className="sticky top-0 bg-mu-panel text-xs uppercase tracking-wider text-mu-muted">
                  <tr>
                    <th className="px-3 py-2">Nội dung</th>
                    <th className="px-3 py-2 w-[180px]">Giá trị</th>
                  </tr>
                </thead>
                <tbody>
                  {filtered.map((entry) => {
                    const dirty = (draft[entry.key] ?? "") !== entry.value;
                    const { label, account } = readableEntry(entry);
                    return (
                      <tr
                        key={entry.key}
                        className={`border-t border-white/5 ${
                          dirty ? "bg-mu-gold/5" : ""
                        }`}
                      >
                        <td className="px-3 py-2 align-middle">
                          <div className="font-medium text-[13px] text-white">{label}</div>
                          {account ? (
                            <div className="text-[11px] text-mu-muted">
                              Áp dụng cho: {account}
                            </div>
                          ) : entry.description ? (
                            <div className="text-[11px] text-mu-muted">{entry.description}</div>
                          ) : null}
                        </td>
                        <td className="px-3 py-2">
                          <input
                            className="input font-mono"
                            value={draft[entry.key] ?? ""}
                            onChange={(ev) =>
                              setDraft((d) => ({
                                ...d,
                                [entry.key]: ev.target.value,
                              }))
                            }
                          />
                        </td>
                      </tr>
                    );
                  })}
                  {!filtered.length && (
                    <tr>
                      <td colSpan={2} className="px-3 py-6 text-mu-muted">
                        Không có key khớp bộ lọc.
                      </td>
                    </tr>
                  )}
                </tbody>
              </table>
            </div>
          </>
        ) : (
          <textarea
            className="input min-h-[480px] font-mono text-xs leading-relaxed"
            value={raw}
            onChange={(e) => setRaw(e.target.value)}
            spellCheck={false}
          />
        )}

        <div className="flex flex-wrap gap-3">
          <button
            type="button"
            className="btn-gold"
            disabled={saving || loading}
            onClick={() => void onSave()}
          >
            {saving ? "Đang lưu…" : "Lưu + backup"}
          </button>
          <button
            type="button"
            className="btn-ghost"
            disabled={loading || saving}
            onClick={() => void loadFile(slug)}
          >
            Tải lại
          </button>
        </div>
      </div>
    </div>
  );
}
