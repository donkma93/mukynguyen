"use client";

import { FormEvent, useEffect, useState } from "react";
import FlashMessage from "@/components/FlashMessage";

type Source = "chaos" | "common";

type RateEntry = {
  source: Source;
  key: string;
  label: string;
  rates: [number, number, number, number];
};

type Section = {
  id: Source;
  label: string;
  description: string;
  entries: RateEntry[];
};

const accountLabels = ["Thường", "VIP 1", "VIP 2", "VIP 3"];

export default function EnhancementRatesPanel() {
  const [sections, setSections] = useState<Section[]>([]);
  const [loading, setLoading] = useState(true);
  const [saving, setSaving] = useState(false);
  const [message, setMessage] = useState<string | null>(null);
  const [error, setError] = useState<string | null>(null);

  async function load() {
    setLoading(true);
    setError(null);
    try {
      const response = await fetch("/api/admin/enhancement-rates", { cache: "no-store" });
      const data = await response.json().catch(() => ({}));
      if (!response.ok) {
        setError(data.error || "Không tải được tỉ lệ đập đồ");
        return;
      }
      setSections(Array.isArray(data.sections) ? data.sections : []);
    } catch {
      setError("Không thể kết nối máy chủ.");
    } finally {
      setLoading(false);
    }
  }

  useEffect(() => {
    void load();
  }, []);

  function updateRate(source: Source, key: string, level: number, value: string) {
    setSections((current) =>
      current.map((section) =>
        section.id !== source
          ? section
          : {
              ...section,
              entries: section.entries.map((entry) => {
                if (entry.key !== key) return entry;
                const rates = [...entry.rates] as [number, number, number, number];
                rates[level] = value === "" ? 0 : Number(value);
                return { ...entry, rates };
              }),
            }
      )
    );
  }

  async function save(event: FormEvent) {
    event.preventDefault();
    setSaving(true);
    setMessage(null);
    setError(null);
    try {
      const items = sections.flatMap((section) => section.entries);
      const response = await fetch("/api/admin/enhancement-rates", {
        method: "PUT",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ items }),
      });
      const data = await response.json().catch(() => ({}));
      if (!response.ok) {
        setError(data.error || "Không thể lưu tỉ lệ");
        return;
      }
      setSections(Array.isArray(data.sections) ? data.sections : sections);
      setMessage(data.message || "Đã lưu tỉ lệ.");
    } catch {
      setError("Không thể kết nối máy chủ.");
    } finally {
      setSaving(false);
    }
  }

  return (
    <form onSubmit={save} className="space-y-6">
      <FlashMessage type="error" message={error} />
      <FlashMessage type="success" message={message} />

      <div className="card border-amber-400/40 p-4 text-sm text-amber-100">
        Lưu tại đây sẽ ghi vào cấu hình GameServer. Tỉ lệ <strong>-1</strong> giữ chế độ mặc định của
        server. Sau khi lưu, cần khởi động lại <strong>GameServer</strong> thì người chơi mới nhận tỉ lệ mới.
      </div>

      {loading ? <div className="card p-5 text-mu-muted">Đang tải cấu hình...</div> : null}

      {sections.map((section) => (
        <section key={section.id} className="card-glow overflow-hidden">
          <div className="border-b border-white/10 px-5 py-4">
            <h2 className="panel-title">{section.label}</h2>
            <p className="muted mt-1 text-sm">{section.description}</p>
          </div>
          <div className="table-wrap">
            <table className="data-table min-w-[780px]">
              <thead>
                <tr>
                  <th>Công thức</th>
                  {accountLabels.map((label) => (
                    <th key={label}>{label} (%)</th>
                  ))}
                </tr>
              </thead>
              <tbody>
                {section.entries.map((entry) => (
                  <tr key={`${entry.source}:${entry.key}`}>
                    <td>
                      <div className="font-medium text-white">{entry.label}</div>
                    </td>
                    {entry.rates.map((rate, level) => (
                      <td key={level}>
                        <input
                          className="input min-w-[88px] py-1.5"
                          type="number"
                          min={-1}
                          max={100}
                          step={1}
                          value={rate}
                          aria-label={`${entry.label} ${accountLabels[level]}`}
                          onChange={(event) => updateRate(entry.source, entry.key, level, event.target.value)}
                          required
                        />
                      </td>
                    ))}
                  </tr>
                ))}
              </tbody>
            </table>
          </div>
        </section>
      ))}

      <button type="submit" className="btn-gold" disabled={loading || saving || sections.length === 0}>
        {saving ? "Đang lưu..." : "Lưu toàn bộ tỉ lệ"}
      </button>
    </form>
  );
}
