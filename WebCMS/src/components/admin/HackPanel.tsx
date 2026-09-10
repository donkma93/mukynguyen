"use client";

import { FormEvent, useCallback, useEffect, useState } from "react";
import FlashMessage from "@/components/FlashMessage";

type HackLogEntry = {
  source: "gs" | "antihack";
  date: string;
  time: string;
  account: string | null;
  character: string | null;
  ip: string | null;
  hwid: string | null;
  kind: string;
  message: string;
  raw: string;
  file: string;
};

type BlackListSnapshot = {
  id: string;
  label: string;
  ips: string[];
  hwids: string[];
};

type Payload = {
  entries: HackLogEntry[];
  scannedFiles: number;
  days: number;
  query: string;
  sources: string[];
  blacklists: BlackListSnapshot[];
};

export default function HackPanel() {
  const [q, setQ] = useState("");
  const [days, setDays] = useState("14");
  const [source, setSource] = useState("all");
  const [data, setData] = useState<Payload | null>(null);
  const [error, setError] = useState<string | null>(null);
  const [loading, setLoading] = useState(false);

  const load = useCallback(async (qq: string, dd: string, ss: string) => {
    setLoading(true);
    setError(null);
    try {
      const params = new URLSearchParams();
      if (qq.trim()) params.set("q", qq.trim());
      params.set("days", dd || "14");
      params.set("source", ss || "all");
      params.set("limit", "200");
      const res = await fetch(`/api/admin/hack?${params.toString()}`, {
        cache: "no-store",
      });
      const json = await res.json().catch(() => ({}));
      if (!res.ok) {
        setError(json.error || "Không tải được hack log");
        return;
      }
      setData(json as Payload);
    } catch {
      setError("Không kết nối được API hack log");
    } finally {
      setLoading(false);
    }
  }, []);

  useEffect(() => {
    void load("", "14", "all");
  }, [load]);

  function onSearch(e?: FormEvent) {
    e?.preventDefault();
    void load(q, days, source);
  }

  function searchValue(value: string) {
    setQ(value);
    void load(value, days, source);
  }

  return (
    <div className="space-y-6">
      <FlashMessage type="error" message={error} />

      <form onSubmit={onSearch} className="card flex flex-wrap items-end gap-3 p-4">
        <div className="min-w-[200px] flex-1">
          <label className="label">Tìm tài khoản / IP / HWID / loại</label>
          <input
            className="input"
            placeholder="VD: account, 192.168.1.6, Speed Hack..."
            value={q}
            onChange={(e) => setQ(e.target.value)}
          />
        </div>
        <div className="w-28">
          <label className="label">Số ngày</label>
          <select
            className="input"
            value={days}
            onChange={(e) => setDays(e.target.value)}
          >
            <option value="3">3</option>
            <option value="7">7</option>
            <option value="14">14</option>
            <option value="30">30</option>
            <option value="60">60</option>
          </select>
        </div>
        <div className="w-40">
          <label className="label">Nguồn</label>
          <select
            className="input"
            value={source}
            onChange={(e) => setSource(e.target.value)}
          >
            <option value="all">GS + AntiHack</option>
            <option value="gs">GameServer</option>
            <option value="antihack">AntiHack</option>
          </select>
        </div>
        <button type="submit" className="btn-gold" disabled={loading}>
          {loading ? "Đang tải..." : "Tìm kiếm"}
        </button>
        <button
          type="button"
          className="btn-ghost"
          disabled={loading}
          onClick={() => {
            setQ("");
            void load("", days, source);
          }}
        >
          Xóa lọc
        </button>
      </form>

      <div className="card space-y-2 p-4 text-sm">
        <p className="muted">
          Đã quét <span className="text-mu-gold">{data?.scannedFiles ?? 0}</span>{" "}
          file log · Hiển thị{" "}
          <span className="text-mu-lime">{data?.entries?.length ?? 0}</span> dòng
          · {data?.days ?? days} ngày gần nhất
        </p>
      </div>

      <div className="card p-4">
        <h2 className="panel-title mb-3">Nhật ký phát hiện hack</h2>
        <div className="table-wrap">
          <table className="data-table">
            <thead>
              <tr>
                <th>Thời gian</th>
                <th>Nguồn</th>
                <th>Loại</th>
                <th>Account</th>
                <th>Nhân vật</th>
                <th>IP</th>
                <th>Chi tiết</th>
              </tr>
            </thead>
            <tbody>
              {(data?.entries || []).map((row, idx) => (
                <tr key={`${row.date}-${row.time}-${idx}`}>
                  <td className="whitespace-nowrap">
                    {row.date}
                    <br />
                    <span className="muted text-xs">{row.time}</span>
                  </td>
                  <td>
                    <span
                      className={
                        row.source === "gs" ? "badge-gold" : "badge-lime"
                      }
                    >
                      {row.source === "gs" ? "GS" : "AH"}
                    </span>
                  </td>
                  <td className="font-medium text-mu-gold">{row.kind}</td>
                  <td>
                    {row.account ? (
                      <button
                        type="button"
                        className="text-left text-mu-lime hover:underline"
                        onClick={() => searchValue(row.account!)}
                        title="Lọc theo account"
                      >
                        {row.account}
                      </button>
                    ) : (
                      <span className="muted">—</span>
                    )}
                  </td>
                  <td>{row.character || <span className="muted">—</span>}</td>
                  <td>
                    {row.ip ? (
                      <button
                        type="button"
                        className="text-left hover:underline"
                        onClick={() => searchValue(row.ip!)}
                        title="Lọc theo IP"
                      >
                        {row.ip}
                      </button>
                    ) : (
                      <span className="muted">—</span>
                    )}
                  </td>
                  <td className="max-w-[360px] break-words text-xs text-gray-300">
                    {row.message}
                    {row.hwid ? (
                      <div className="muted mt-1 truncate" title={row.hwid}>
                        HWID: {row.hwid}
                      </div>
                    ) : null}
                  </td>
                </tr>
              ))}
              {!loading && (data?.entries?.length ?? 0) === 0 ? (
                <tr>
                  <td colSpan={7} className="py-8 text-center text-mu-muted">
                    {q
                      ? "Không tìm thấy dòng log khớp bộ lọc."
                      : "Chưa có sự kiện hack trong khoảng ngày đã chọn (log đang trống hoặc chưa bị phát hiện)."}
                  </td>
                </tr>
              ) : null}
            </tbody>
          </table>
        </div>
      </div>

      <div className="grid gap-4 md:grid-cols-2">
        {(data?.blacklists || []).map((bl) => (
          <div key={bl.id} className="card p-4">
            <h2 className="panel-title">{bl.label}</h2>
            <div className="space-y-3 text-sm">
              <div>
                <p className="mb-1 font-semibold text-mu-gold">
                  IP bị chặn ({bl.ips.length})
                </p>
                {bl.ips.length === 0 ? (
                  <p className="muted">Trống</p>
                ) : (
                  <ul className="max-h-40 space-y-1 overflow-y-auto">
                    {bl.ips.map((ip) => (
                      <li key={ip}>
                        <button
                          type="button"
                          className="text-mu-lime hover:underline"
                          onClick={() => searchValue(ip)}
                        >
                          {ip}
                        </button>
                      </li>
                    ))}
                  </ul>
                )}
              </div>
              <div>
                <p className="mb-1 font-semibold text-mu-gold">
                  HWID bị chặn ({bl.hwids.length})
                </p>
                {bl.hwids.length === 0 ? (
                  <p className="muted">Trống</p>
                ) : (
                  <ul className="max-h-40 space-y-1 overflow-y-auto">
                    {bl.hwids.map((id) => (
                      <li key={id} className="break-all">
                        <button
                          type="button"
                          className="text-left hover:underline"
                          onClick={() => searchValue(id)}
                        >
                          {id}
                        </button>
                      </li>
                    ))}
                  </ul>
                )}
              </div>
            </div>
          </div>
        ))}
      </div>
    </div>
  );
}
