"use client";

import { FormEvent, useEffect, useState } from "react";
import FlashMessage from "@/components/FlashMessage";

type GiftcodeRow = {
  id: number;
  code: string;
  maxUses: number;
  usedCount: number;
  description?: string | null;
  isActive: boolean;
  rewardWc: number;
  rewardWp: number;
};

export default function GiftcodesPanel() {
  const [rows, setRows] = useState<GiftcodeRow[]>([]);
  const [code, setCode] = useState("");
  const [maxUses, setMaxUses] = useState("1");
  const [rewardWc, setRewardWc] = useState("0");
  const [rewardWp, setRewardWp] = useState("0");
  const [description, setDescription] = useState("");
  const [error, setError] = useState<string | null>(null);
  const [message, setMessage] = useState<string | null>(null);
  const [loading, setLoading] = useState(false);

  async function load() {
    try {
      const res = await fetch("/api/admin/giftcodes");
      const data = await res.json().catch(() => ({}));
      const list = (data.items || []) as GiftcodeRow[];
      setRows(Array.isArray(list) ? list : []);
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
      const res = await fetch("/api/admin/giftcodes", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({
          code,
          maxUses: Number(maxUses),
          rewardWc: Number(rewardWc),
          rewardWp: Number(rewardWp),
          description,
        }),
      });
      const data = await res.json().catch(() => ({}));
      if (!res.ok) {
        setError(data.error || data.message || "Tạo giftcode thất bại");
        return;
      }
      setMessage("Đã tạo giftcode.");
      setCode("");
      setDescription("");
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

      <form onSubmit={onSubmit} className="card grid gap-3 p-5 md:grid-cols-2">
        <h2 className="panel-title md:col-span-2">Tạo giftcode</h2>
        <div>
          <label className="label">Mã</label>
          <input
            className="input uppercase"
            value={code}
            onChange={(e) => setCode(e.target.value)}
            required
            minLength={3}
            maxLength={32}
          />
        </div>
        <div>
          <label className="label">Số lượt dùng</label>
          <input
            className="input"
            type="number"
            min={1}
            value={maxUses}
            onChange={(e) => setMaxUses(e.target.value)}
            required
          />
        </div>
        <div>
          <label className="label">Thưởng WC</label>
          <input
            className="input"
            type="number"
            value={rewardWc}
            onChange={(e) => setRewardWc(e.target.value)}
          />
        </div>
        <div>
          <label className="label">Thưởng WP</label>
          <input
            className="input"
            type="number"
            value={rewardWp}
            onChange={(e) => setRewardWp(e.target.value)}
          />
        </div>
        <div className="md:col-span-2">
          <label className="label">Mô tả</label>
          <input
            className="input"
            value={description}
            onChange={(e) => setDescription(e.target.value)}
          />
        </div>
        <div className="md:col-span-2">
          <button type="submit" className="btn-gold" disabled={loading}>
            {loading ? "Đang tạo..." : "Tạo giftcode"}
          </button>
        </div>
      </form>

      <div className="card p-5">
        <h2 className="panel-title mb-4">Danh sách giftcode</h2>
        <div className="table-wrap">
          <table className="data-table">
            <thead>
              <tr>
                <th>Code</th>
                <th>Đã dùng</th>
                <th>WC/WP</th>
                <th>Mô tả</th>
                <th>TT</th>
              </tr>
            </thead>
            <tbody>
              {rows.length === 0 ? (
                <tr>
                  <td colSpan={5} className="py-6 text-center text-mu-muted">
                    Chưa có giftcode.
                  </td>
                </tr>
              ) : (
                rows.map((row) => (
                  <tr key={row.id}>
                    <td className="font-semibold text-mu-gold">{row.code}</td>
                    <td>
                      {row.usedCount}/{row.maxUses}
                    </td>
                    <td>
                      {row.rewardWc}/{row.rewardWp}
                    </td>
                    <td className="max-w-[220px] truncate">{row.description || "—"}</td>
                    <td>
                      {row.isActive ? (
                        <span className="badge-lime">Active</span>
                      ) : (
                        <span className="badge-danger">Tắt</span>
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
