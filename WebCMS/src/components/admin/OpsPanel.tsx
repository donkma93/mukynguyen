"use client";

import { useCallback, useEffect, useState } from "react";
import FlashMessage from "@/components/FlashMessage";

type ProcessStatus = {
  name: string;
  running: boolean;
  pid: number | null;
  startTime: string | null;
};

type StatusPayload = {
  opsEnabled: boolean;
  online: number;
  processes: ProcessStatus[];
  onlineHint: string;
};

export default function OpsPanel() {
  const [status, setStatus] = useState<StatusPayload | null>(null);
  const [error, setError] = useState<string | null>(null);
  const [message, setMessage] = useState<string | null>(null);
  const [log, setLog] = useState<string>("");
  const [busy, setBusy] = useState(false);

  const load = useCallback(async () => {
    try {
      const res = await fetch("/api/admin/ops/status", { cache: "no-store" });
      const data = await res.json().catch(() => ({}));
      if (!res.ok) {
        setError(data.error || "Không lấy được trạng thái");
        return;
      }
      setStatus(data as StatusPayload);
      setError(null);
    } catch {
      setError("Không kết nối được API ops");
    }
  }, []);

  useEffect(() => {
    void load();
    const t = setInterval(() => void load(), 4000);
    return () => clearInterval(t);
  }, [load]);

  async function restart(scope: "gameserver" | "all") {
    const label =
      scope === "gameserver"
        ? "Restart GameServer (giữ DS/JS/CS)?"
        : "Restart TOÀN BỘ stack (DS/JS/CS/GS/AntiHack)?";
    if (!window.confirm(label)) return;
    setBusy(true);
    setError(null);
    setMessage(null);
    setLog("");
    try {
      const res = await fetch("/api/admin/ops/restart", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ scope }),
      });
      const data = await res.json().catch(() => ({}));
      if (!res.ok) {
        setError(data.error || "Restart thất bại");
        return;
      }
      setMessage(
        scope === "gameserver"
          ? "Đã gửi lệnh restart GameServer."
          : "Đã gửi lệnh restart full stack."
      );
      setLog(String(data.log || ""));
      if (data.status) {
        setStatus({
          ...(data.status as StatusPayload),
          online: status?.online ?? 0,
        });
      }
      await load();
    } catch {
      setError("Không kết nối được API restart");
    } finally {
      setBusy(false);
    }
  }

  return (
    <div className="space-y-6">
      <FlashMessage type="error" message={error} />
      <FlashMessage type="success" message={message} />

      <div className="card p-5 space-y-4">
        <div className="flex flex-wrap items-center justify-between gap-3">
          <div>
            <h2 className="panel-title">Trạng thái tiến trình</h2>
            <p className="muted mt-1 text-sm">
              Tiến trình Mu Server đang chạy trên máy chủ.
            </p>
          </div>
          <div className="text-right text-sm">
            <div>
              Online DB:{" "}
              <span className="font-semibold text-mu-lime">
                {status?.online ?? "—"}
              </span>
            </div>
            <div className="muted">Ops: {status?.opsEnabled ? "ON" : "OFF"}</div>
          </div>
        </div>

        <div className="overflow-x-auto">
          <table className="w-full min-w-[480px] text-left text-sm">
            <thead className="text-xs uppercase tracking-wider text-mu-muted">
              <tr>
                <th className="py-2 pr-3">Process</th>
                <th className="py-2 pr-3">Status</th>
                <th className="py-2 pr-3">PID</th>
                <th className="py-2">Start</th>
              </tr>
            </thead>
            <tbody>
              {(status?.processes || []).map((p) => (
                <tr key={p.name} className="border-t border-white/5">
                  <td className="py-2 pr-3 font-medium">{p.name}</td>
                  <td className="py-2 pr-3">
                    <span
                      className={
                        p.running ? "text-mu-lime" : "text-mu-danger"
                      }
                    >
                      {p.running ? "Running" : "Stopped"}
                    </span>
                  </td>
                  <td className="py-2 pr-3 text-mu-muted">{p.pid ?? "—"}</td>
                  <td className="py-2 text-mu-muted">
                    {p.startTime
                      ? new Date(p.startTime).toLocaleString("vi-VN")
                      : "—"}
                  </td>
                </tr>
              ))}
              {!status?.processes?.length && (
                <tr>
                  <td colSpan={4} className="py-4 text-mu-muted">
                    Đang tải…
                  </td>
                </tr>
              )}
            </tbody>
          </table>
        </div>

        <div className="flex flex-wrap gap-3 pt-2">
          <button
            type="button"
            className="btn-gold"
            disabled={busy || status?.opsEnabled === false}
            onClick={() => void restart("gameserver")}
          >
            {busy ? "Đang xử lý…" : "Restart GameServer"}
          </button>
          <button
            type="button"
            className="btn-danger"
            disabled={busy || status?.opsEnabled === false}
            onClick={() => void restart("all")}
          >
            Restart full stack
          </button>
          <button
            type="button"
            className="btn-ghost"
            disabled={busy}
            onClick={() => void load()}
          >
            Làm mới
          </button>
        </div>

        <p className="text-xs text-mu-muted">
          Sau khi sửa INI/Data trên WebCMS, dùng Restart GameServer để áp dụng.
          Client kết nối: {status?.onlineHint || "ConnectServer"}
        </p>
      </div>

      {log ? (
        <div className="card p-5">
          <h2 className="panel-title mb-3">Log restart</h2>
          <pre className="max-h-80 overflow-auto rounded-lg bg-black/50 p-3 text-xs text-gray-300 whitespace-pre-wrap">
            {log}
          </pre>
        </div>
      ) : null}
    </div>
  );
}
