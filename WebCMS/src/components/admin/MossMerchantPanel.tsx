"use client";

import { useCallback, useEffect, useState } from "react";
import FlashMessage from "@/components/FlashMessage";

type MossSettings = {
  enabled: boolean;
  durationSeconds: number;
  dailyTimes: string[];
  synchronized: boolean;
  subCount: number;
};

export default function MossMerchantPanel() {
  const [settings, setSettings] = useState<MossSettings | null>(null);
  const [enabled, setEnabled] = useState(false);
  const [durationMinutes, setDurationMinutes] = useState(10);
  const [dailyTimes, setDailyTimes] = useState("02:00, 08:00, 14:00, 20:00");
  const [delayMinutes, setDelayMinutes] = useState(2);
  const [busy, setBusy] = useState(false);
  const [error, setError] = useState<string | null>(null);
  const [message, setMessage] = useState<string | null>(null);

  const load = useCallback(async () => {
    try {
      const res = await fetch("/api/admin/events/moss", { cache: "no-store" });
      const data = await res.json().catch(() => ({}));
      if (!res.ok) throw new Error(data.error || "Không đọc được sự kiện Moss Merchant");
      const next = data as MossSettings;
      setSettings(next);
      setEnabled(next.enabled);
      setDurationMinutes(Math.max(1, Math.round(next.durationSeconds / 60)));
      setDailyTimes(next.dailyTimes.join(", "));
      setError(null);
    } catch (e) {
      setError(e instanceof Error ? e.message : "Không đọc được sự kiện Moss Merchant");
    }
  }, []);

  useEffect(() => {
    void load();
  }, [load]);

  async function request(body: Record<string, unknown>) {
    const res = await fetch("/api/admin/events/moss", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(body),
    });
    const data = await res.json().catch(() => ({}));
    if (!res.ok) throw new Error(data.error || "Thao tác thất bại");
    return data as { message?: string; scheduledAt?: string };
  }

  async function save() {
    setBusy(true);
    setError(null);
    setMessage(null);
    try {
      const data = await request({
        action: "save",
        enabled,
        durationSeconds: durationMinutes * 60,
        dailyTimes: dailyTimes.split(",").map((value) => value.trim()),
      });
      setMessage(data.message || "Đã lưu cấu hình Moss Merchant.");
      await load();
    } catch (e) {
      setError(e instanceof Error ? e.message : "Không lưu được cấu hình");
    } finally {
      setBusy(false);
    }
  }

  async function startTest() {
    if (!window.confirm(`Tạo lượt Moss Merchant sau ${delayMinutes} phút và restart toàn bộ GameServer? Người chơi sẽ bị ngắt kết nối ngắn.`)) {
      return;
    }
    setBusy(true);
    setError(null);
    setMessage(null);
    try {
      const data = await request({ action: "test", delayMinutes });
      const when = data.scheduledAt
        ? new Date(data.scheduledAt).toLocaleTimeString("vi-VN", { hour: "2-digit", minute: "2-digit" })
        : "vài phút tới";
      setMessage(`${data.message || "Đã tạo lượt test."} Moss sẽ xuất hiện lúc ${when} tại Elbeland (22, 225).`);
      await load();
    } catch (e) {
      setError(e instanceof Error ? e.message : "Không thể tạo lượt test");
    } finally {
      setBusy(false);
    }
  }

  return (
    <div className="space-y-6">
      <FlashMessage type="error" message={error} />
      <FlashMessage type="success" message={message} />

      <section className="card space-y-5 p-5">
        <div>
          <h2 className="panel-title">Thương gia Moss</h2>
          <p className="muted mt-1 text-sm">
            Quản lý lịch xuất hiện và tạo lượt test nhanh. NPC xuất hiện tại Elbeland (22, 225).
          </p>
        </div>

        {!settings?.synchronized ? (
          <p className="rounded-lg border border-amber-400/30 bg-amber-400/10 p-3 text-sm text-amber-200">
            Cấu hình giữa các Sub đang khác nhau. Bấm Lưu để đồng bộ lại.
          </p>
        ) : null}

        <label className="flex items-center justify-between gap-4 rounded-lg border border-white/10 bg-black/20 p-4">
          <span>
            <span className="block font-semibold">Bật Moss Merchant</span>
            <span className="muted text-sm">Tắt thì NPC không thể quay thưởng, dù đã đến giờ lịch.</span>
          </span>
          <input
            type="checkbox"
            checked={enabled}
            onChange={(event) => setEnabled(event.target.checked)}
            className="h-5 w-5 accent-lime-400"
          />
        </label>

        <div className="grid gap-4 md:grid-cols-2">
          <label className="text-sm font-medium">
            Thời lượng mỗi lượt (phút)
            <input
              type="number"
              min={1}
              max={1440}
              value={durationMinutes}
              onChange={(event) => setDurationMinutes(Number(event.target.value))}
              className="input mt-2 w-full"
            />
          </label>
          <label className="text-sm font-medium">
            Lịch chạy hằng ngày
            <input
              value={dailyTimes}
              onChange={(event) => setDailyTimes(event.target.value)}
              placeholder="02:00, 08:00, 14:00, 20:00"
              className="input mt-2 w-full"
            />
          </label>
        </div>
        <p className="muted -mt-2 text-xs">Nhập giờ theo định dạng 24h, ngăn cách bằng dấu phẩy.</p>

        <div className="flex flex-wrap gap-3">
          <button type="button" className="btn-gold" disabled={busy} onClick={() => void save()}>
            {busy ? "Đang xử lý…" : "Lưu cấu hình"}
          </button>
          <button type="button" className="btn-ghost" disabled={busy} onClick={() => void load()}>
            Tải lại
          </button>
        </div>
      </section>

      <section className="card space-y-4 p-5">
        <div>
          <h2 className="panel-title">Chạy test ngay</h2>
          <p className="muted mt-1 text-sm">
            Web tự thêm một mốc chạy thử, bật Moss Merchant và restart các GameServer.
          </p>
        </div>
        <div className="flex flex-wrap items-end gap-3">
          <label className="text-sm font-medium">
            Xuất hiện sau (phút)
            <input
              type="number"
              min={1}
              max={60}
              value={delayMinutes}
              onChange={(event) => setDelayMinutes(Number(event.target.value))}
              className="input mt-2 w-36"
            />
          </label>
          <button type="button" className="btn-danger" disabled={busy} onClick={() => void startTest()}>
            {busy ? "Đang nạp sự kiện…" : "Mở lượt test Moss"}
          </button>
        </div>
        <p className="text-xs text-mu-muted">Lượt test chỉ diễn ra một lần; lịch hằng ngày vẫn được giữ nguyên.</p>
      </section>
    </div>
  );
}
