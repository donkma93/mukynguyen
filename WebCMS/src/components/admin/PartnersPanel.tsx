"use client";

import { FormEvent, useEffect, useState } from "react";
import FlashMessage from "@/components/FlashMessage";

type PartnerTier = "new" | "stable" | "top";

type PartnerItem = {
  account: string;
  tier: PartnerTier;
  isActive: boolean;
  yearMonth?: string;
  budget?: {
    wc: number;
    wp: number;
    wg: number;
    liveSessions: number;
    highlight: number;
  };
  used?: {
    wcUsed: number;
    wpUsed: number;
    wgUsed: number;
    liveSessionsUsed: number;
    highlightUsed: number;
  };
  caps?: {
    labelVi: string;
    wc: number;
    wp: number;
    wg: number;
    liveSessions: number;
    highlight: number;
  };
};

const TIER_LABEL: Record<PartnerTier, string> = {
  new: "Partner mới",
  stable: "Partner ổn định",
  top: "Partner top",
};

export default function PartnersPanel() {
  const [rows, setRows] = useState<PartnerItem[]>([]);
  const [account, setAccount] = useState("");
  const [tier, setTier] = useState<PartnerTier>("new");
  const [error, setError] = useState<string | null>(null);
  const [message, setMessage] = useState<string | null>(null);
  const [loading, setLoading] = useState(false);

  async function load() {
    try {
      const res = await fetch("/api/admin/partners");
      const data = await res.json().catch(() => ({}));
      if (!res.ok) {
        setError(data.error || "Không tải được danh sách đối tác");
        return;
      }
      setRows(Array.isArray(data.items) ? (data.items as PartnerItem[]) : []);
    } catch {
      setRows([]);
    }
  }

  useEffect(() => {
    void load();
  }, []);

  async function onSubmit(e: FormEvent) {
    e.preventDefault();
    setLoading(true);
    setError(null);
    setMessage(null);
    try {
      const res = await fetch("/api/admin/partners", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ account, tier, isActive: true }),
      });
      const data = await res.json().catch(() => ({}));
      if (!res.ok) {
        setError(data.error || "Cập nhật đối tác thất bại");
        return;
      }
      setMessage(data.message || "Đã cập nhật đối tác");
      setAccount("");
      await load();
    } catch {
      setError("Không thể kết nối máy chủ.");
    } finally {
      setLoading(false);
    }
  }

  async function setActive(row: PartnerItem, isActive: boolean) {
    setLoading(true);
    setError(null);
    setMessage(null);
    try {
      const res = await fetch("/api/admin/partners", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({
          account: row.account,
          tier: row.tier,
          isActive,
        }),
      });
      const data = await res.json().catch(() => ({}));
      if (!res.ok) {
        setError(data.error || "Cập nhật thất bại");
        return;
      }
      setMessage(isActive ? "Đã kích hoạt đối tác" : "Đã tắt đối tác");
      await load();
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

      <form onSubmit={onSubmit} className="card grid gap-3 p-5 md:grid-cols-3">
        <h2 className="panel-title md:col-span-3">Thêm / cập nhật đối tác</h2>
        <div>
          <label className="label">Tài khoản game</label>
          <input
            className="input"
            value={account}
            onChange={(e) => setAccount(e.target.value)}
            required
            minLength={4}
            maxLength={10}
            placeholder="VD: streamer1"
          />
        </div>
        <div>
          <label className="label">Hạng</label>
          <select
            className="input"
            value={tier}
            onChange={(e) => setTier(e.target.value as PartnerTier)}
          >
            <option value="new">{TIER_LABEL.new}</option>
            <option value="stable">{TIER_LABEL.stable}</option>
            <option value="top">{TIER_LABEL.top}</option>
          </select>
        </div>
        <div className="flex items-end">
          <button type="submit" className="btn-gold w-full" disabled={loading}>
            {loading ? "Đang lưu..." : "Lưu đối tác"}
          </button>
        </div>
      </form>

      <div className="card p-4">
        <div className="table-wrap">
          <table className="data-table">
            <thead>
              <tr>
                <th>Account</th>
                <th>Hạng</th>
                <th>Trạng thái</th>
                <th>Còn lại (tháng)</th>
                <th></th>
              </tr>
            </thead>
            <tbody>
              {rows.length === 0 ? (
                <tr>
                  <td colSpan={5} className="text-mu-muted">
                    Chưa có đối tác nào.
                  </td>
                </tr>
              ) : (
                rows.map((row) => (
                  <tr key={row.account}>
                    <td className="font-semibold text-mu-gold">{row.account}</td>
                    <td>{TIER_LABEL[row.tier] || row.tier}</td>
                    <td>
                      <span className={row.isActive ? "text-mu-lime" : "text-mu-muted"}>
                        {row.isActive ? "Active" : "Off"}
                      </span>
                    </td>
                    <td className="text-xs text-gray-300">
                      {row.budget ? (
                        <>
                          WC {row.budget.wc}/{row.caps?.wc ?? "?"} · WP{" "}
                          {row.budget.wp}/{row.caps?.wp ?? "?"} · WG{" "}
                          {row.budget.wg}/{row.caps?.wg ?? "?"}
                          <br />
                          Live {row.budget.liveSessions}/{row.caps?.liveSessions ?? "?"} · HL{" "}
                          {row.budget.highlight}/{row.caps?.highlight ?? "?"}
                        </>
                      ) : (
                        "—"
                      )}
                    </td>
                    <td className="space-x-2 whitespace-nowrap">
                      {row.isActive ? (
                        <button
                          type="button"
                          className="btn-ghost"
                          disabled={loading}
                          onClick={() => void setActive(row, false)}
                        >
                          Tắt
                        </button>
                      ) : (
                        <button
                          type="button"
                          className="btn-lime"
                          disabled={loading}
                          onClick={() => void setActive(row, true)}
                        >
                          Bật
                        </button>
                      )}
                    </td>
                  </tr>
                ))
              )}
            </tbody>
          </table>
        </div>
      </div>
    </div>
  );
}
