"use client";

import { FormEvent, useState } from "react";
import FlashMessage from "@/components/FlashMessage";

type AccountRow = {
  account: string;
  email?: string | null;
  blocked?: boolean;
  vip?: number;
  expireDate?: string | Date | null;
};

export default function AccountsPanel() {
  const [q, setQ] = useState("");
  const [rows, setRows] = useState<AccountRow[]>([]);
  const [selected, setSelected] = useState("");
  const [vipLevel, setVipLevel] = useState("1");
  const [vipDays, setVipDays] = useState("30");
  const [wc, setWc] = useState("0");
  const [wp, setWp] = useState("0");
  const [message, setMessage] = useState<string | null>(null);
  const [error, setError] = useState<string | null>(null);
  const [loading, setLoading] = useState(false);

  async function search(e?: FormEvent) {
    e?.preventDefault();
    setLoading(true);
    setError(null);
    setMessage(null);
    try {
      const res = await fetch(`/api/admin/accounts?q=${encodeURIComponent(q)}`);
      const data = await res.json().catch(() => ({}));
      if (!res.ok) {
        setError(data.error || "Tìm kiếm thất bại");
        return;
      }
      const list = (data.items || []) as AccountRow[];
      setRows(Array.isArray(list) ? list : []);
      if (Array.isArray(list) && list[0]) setSelected(list[0].account);
    } catch {
      setError("Không thể kết nối máy chủ.");
    } finally {
      setLoading(false);
    }
  }

  async function postJson(url: string, body: Record<string, unknown>) {
    setLoading(true);
    setError(null);
    setMessage(null);
    try {
      const res = await fetch(url, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(body),
      });
      const data = await res.json().catch(() => ({}));
      if (!res.ok) {
        setError(data.error || data.message || "Thao tác thất bại");
        return;
      }
      setMessage(data.message || "Thành công");
      await search();
    } catch {
      setError("Không thể kết nối máy chủ.");
    } finally {
      setLoading(false);
    }
  }

  return (
    <div className="space-y-6">
      <FlashMessage type="error" message={error} />
      <FlashMessage type="success" message={message} />

      <form onSubmit={search} className="card flex flex-wrap gap-3 p-4">
        <input
          className="input max-w-sm"
          placeholder="Tìm tài khoản..."
          value={q}
          onChange={(e) => setQ(e.target.value)}
        />
        <button type="submit" className="btn-gold" disabled={loading}>
          Tìm kiếm
        </button>
      </form>

      <div className="card p-4">
        <div className="table-wrap">
          <table className="data-table">
            <thead>
              <tr>
                <th>Account</th>
                <th>VIP</th>
                <th>Block</th>
                <th>Email</th>
                <th></th>
              </tr>
            </thead>
            <tbody>
              {rows.length === 0 ? (
                <tr>
                  <td colSpan={5} className="py-6 text-center text-mu-muted">
                    Nhập từ khóa để tìm tài khoản.
                  </td>
                </tr>
              ) : (
                rows.map((row) => (
                  <tr key={row.account}>
                    <td className="text-mu-gold">{row.account}</td>
                    <td>{row.vip ?? 0}</td>
                    <td>
                      {row.blocked ? (
                        <span className="badge-danger">Khóa</span>
                      ) : (
                        <span className="badge-lime">OK</span>
                      )}
                    </td>
                    <td>{row.email || "—"}</td>
                    <td>
                      <button
                        type="button"
                        className="btn-ghost px-2 py-1 text-xs"
                        onClick={() => setSelected(row.account)}
                      >
                        Chọn
                      </button>
                    </td>
                  </tr>
                ))
              )}
            </tbody>
          </table>
        </div>
      </div>

      <div className="grid gap-4 md:grid-cols-3">
        <div className="card space-y-3 p-4">
          <h3 className="panel-title">Khóa / Mở</h3>
          <input
            className="input"
            value={selected}
            onChange={(e) => setSelected(e.target.value)}
            placeholder="Account"
          />
          <div className="flex gap-2">
            <button
              type="button"
              className="btn-danger flex-1"
              disabled={!selected || loading}
              onClick={() => postJson("/api/admin/accounts", { account: selected, action: "block" })}
            >
              Khóa
            </button>
            <button
              type="button"
              className="btn-lime flex-1"
              disabled={!selected || loading}
              onClick={() => postJson("/api/admin/accounts", { account: selected, action: "unblock" })}
            >
              Mở
            </button>
          </div>
        </div>

        <div className="card space-y-3 p-4">
          <h3 className="panel-title">Set VIP</h3>
          <p className="text-xs text-mu-muted">Một loại VIP: 0 = Thường, 1 = VIP</p>
          <input className="input" value={selected} onChange={(e) => setSelected(e.target.value)} />
          <select
            className="input"
            value={vipLevel}
            onChange={(e) => setVipLevel(e.target.value)}
          >
            <option value="0">0 — Thường</option>
            <option value="1">1 — VIP</option>
          </select>
          <input
            className="input"
            type="number"
            min={0}
            value={vipDays}
            onChange={(e) => setVipDays(e.target.value)}
            placeholder="Số ngày"
          />
          <button
            type="button"
            className="btn-gold w-full"
            disabled={!selected || loading}
            onClick={() =>
              postJson("/api/admin/accounts", {
                account: selected,
                action: "vip",
                level: Number(vipLevel) > 0 ? 1 : 0,
                days: Number(vipDays),
              })
            }
          >
            Cập nhật VIP
          </button>
        </div>

        <div className="card space-y-3 p-4">
          <h3 className="panel-title">Cộng coin</h3>
          <input className="input" value={selected} onChange={(e) => setSelected(e.target.value)} />
          <input
            className="input"
            type="number"
            value={wc}
            onChange={(e) => setWc(e.target.value)}
            placeholder="WC"
          />
          <input
            className="input"
            type="number"
            value={wp}
            onChange={(e) => setWp(e.target.value)}
            placeholder="WP"
          />
          <button
            type="button"
            className="btn-lime w-full"
            disabled={!selected || loading}
            onClick={() =>
              postJson("/api/admin/coins", {
                account: selected,
                wc: Number(wc),
                wp: Number(wp),
              })
            }
          >
            Cộng coin
          </button>
        </div>
      </div>
    </div>
  );
}
