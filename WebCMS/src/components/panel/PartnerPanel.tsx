"use client";

import { FormEvent, useCallback, useEffect, useState } from "react";
import FlashMessage from "@/components/FlashMessage";

type BudgetRemaining = {
  wc: number;
  wp: number;
  wg: number;
  liveSessions: number;
  highlight: number;
  caps: {
    labelVi: string;
    wc: number;
    wp: number;
    wg: number;
    liveSessions: number;
    highlight: number;
  };
};

type SessionRow = {
  id: number;
  packageId: string;
  code: string;
  maxUses: number;
  highlightCount: number;
  expiresAt: string | Date | null;
  isActive: boolean;
  createdAt: string | Date;
};

type ClaimRow = {
  id: number;
  packageId: string;
  claimerAccount: string;
  claimerCharacter: string;
  wc: number;
  wp: number;
  wg: number;
  claimedAt: string | Date;
};

type MeResponse = {
  ok: boolean;
  error?: string;
  partner?: { account: string; tier: string };
  yearMonth?: string;
  budget?: {
    used: unknown;
    remaining: BudgetRemaining;
    caps: BudgetRemaining["caps"];
    yearMonth: string;
  };
  activeLive?: SessionRow | null;
  sessions?: SessionRow[];
  claims?: ClaimRow[];
};

function fmtDate(value: string | Date | null | undefined) {
  if (!value) return "—";
  const d = typeof value === "string" ? new Date(value) : value;
  if (Number.isNaN(d.getTime())) return "—";
  return d.toLocaleString("vi-VN");
}

export default function PartnerPanel() {
  const [loading, setLoading] = useState(true);
  const [busy, setBusy] = useState(false);
  const [error, setError] = useState<string | null>(null);
  const [message, setMessage] = useState<string | null>(null);
  const [data, setData] = useState<MeResponse | null>(null);
  const [lastCode, setLastCode] = useState<string | null>(null);
  const [targetAccount, setTargetAccount] = useState("");
  const [characterName, setCharacterName] = useState("");

  const load = useCallback(async () => {
    setLoading(true);
    setError(null);
    try {
      const res = await fetch("/api/partner/me", { cache: "no-store" });
      const json = (await res.json().catch(() => ({}))) as MeResponse;
      if (!res.ok) {
        setError(json.error || "Không tải được panel đối tác");
        setData(null);
        return;
      }
      setData(json);
    } catch {
      setError("Không thể kết nối máy chủ.");
      setData(null);
    } finally {
      setLoading(false);
    }
  }, []);

  useEffect(() => {
    void load();
  }, [load]);

  async function createSession(kind: "live" | "newbie") {
    setBusy(true);
    setError(null);
    setMessage(null);
    try {
      const res = await fetch(
        kind === "live" ? "/api/partner/live-session" : "/api/partner/newbie-session",
        { method: "POST" }
      );
      const json = await res.json().catch(() => ({}));
      if (!res.ok) {
        setError(json.error || "Tạo mã thất bại");
        return;
      }
      setLastCode(String(json.code || ""));
      setMessage(json.message || "Đã tạo mã");
      await load();
    } catch {
      setError("Không thể kết nối máy chủ.");
    } finally {
      setBusy(false);
    }
  }

  async function onHighlight(e: FormEvent) {
    e.preventDefault();
    setBusy(true);
    setError(null);
    setMessage(null);
    try {
      const res = await fetch("/api/partner/highlight", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({
          targetAccount,
          characterName,
        }),
      });
      const json = await res.json().catch(() => ({}));
      if (!res.ok) {
        setError(json.error || "Phát Highlight thất bại");
        return;
      }
      setMessage(json.message || "Đã phát Highlight");
      setTargetAccount("");
      setCharacterName("");
      await load();
    } catch {
      setError("Không thể kết nối máy chủ.");
    } finally {
      setBusy(false);
    }
  }

  if (loading) {
    return <p className="muted">Đang tải panel đối tác...</p>;
  }

  if (!data?.ok || !data.budget) {
    return <FlashMessage type="error" message={error || "Không có quyền đối tác"} />;
  }

  const rem = data.budget.remaining;

  return (
    <div className="space-y-6">
      <FlashMessage type="error" message={error} />
      <FlashMessage type="success" message={message} />

      <div className="card p-4 md:p-6">
        <h2 className="panel-title">Ngân sách tháng {data.yearMonth}</h2>
        <p className="muted mt-1 text-sm">
          Hạng: <span className="text-mu-gold">{rem.caps.labelVi}</span> · Account:{" "}
          <span className="text-mu-lime">{data.partner?.account}</span>
        </p>
        <div className="mt-4 grid gap-3 sm:grid-cols-2 lg:grid-cols-5">
          <Stat label="WC còn" value={`${rem.wc}/${rem.caps.wc}`} />
          <Stat label="WP còn" value={`${rem.wp}/${rem.caps.wp}`} />
          <Stat label="WG còn" value={`${rem.wg}/${rem.caps.wg}`} />
          <Stat label="Live còn" value={`${rem.liveSessions}/${rem.caps.liveSessions}`} />
          <Stat label="Highlight còn" value={`${rem.highlight}/${rem.caps.highlight}`} />
        </div>
      </div>

      <div className="grid gap-4 lg:grid-cols-2">
        <div className="card space-y-3 p-4 md:p-6">
          <h2 className="panel-title">Tạo mã phát quà</h2>
          <p className="muted text-sm">
            Live Drop: WP 80 + WG 30 + Chaos + bùa EXP 30 phút (max 30 người, 2 giờ).
            Newbie: WP 100 + Bless/Soul ×2 (1 lần / acc / 7 ngày).
          </p>
          <div className="flex flex-wrap gap-2">
            <button
              type="button"
              className="btn-gold"
              disabled={busy || rem.liveSessions <= 0}
              onClick={() => void createSession("live")}
            >
              Tạo mã Live Drop
            </button>
            <button
              type="button"
              className="btn-lime"
              disabled={busy}
              onClick={() => void createSession("newbie")}
            >
              Tạo mã Newbie
            </button>
          </div>
          {lastCode ? (
            <div className="rounded-lg border border-mu-gold/30 bg-black/40 p-3">
              <p className="text-xs text-mu-muted">Mã vừa tạo</p>
              <p className="mt-1 font-mono text-lg tracking-wider text-mu-gold">{lastCode}</p>
              <button
                type="button"
                className="btn-ghost mt-2"
                onClick={() => void navigator.clipboard?.writeText(lastCode)}
              >
                Sao chép
              </button>
            </div>
          ) : null}
          {data.activeLive ? (
            <p className="text-sm text-gray-300">
              Phiên Live đang mở:{" "}
              <span className="font-mono text-mu-lime">{data.activeLive.code}</span> · Highlight đã
              phát {data.activeLive.highlightCount}/5 · Hết hạn{" "}
              {fmtDate(data.activeLive.expiresAt)}
            </p>
          ) : (
            <p className="text-sm text-mu-muted">Chưa có phiên Live đang mở.</p>
          )}
        </div>

        <form onSubmit={onHighlight} className="card space-y-3 p-4 md:p-6">
          <h2 className="panel-title">Phát Highlight</h2>
          <p className="muted text-sm">
            WC 100 + WP 200 + Bless/Soul ×3 + Life ×1. Không tự phát cho chính mình. Tối đa 5 /
            phiên Live.
          </p>
          <div>
            <label className="label">Tài khoản nhận</label>
            <input
              className="input"
              value={targetAccount}
              onChange={(e) => setTargetAccount(e.target.value)}
              required
              minLength={4}
              maxLength={10}
            />
          </div>
          <div>
            <label className="label">Nhân vật nhận</label>
            <input
              className="input"
              value={characterName}
              onChange={(e) => setCharacterName(e.target.value)}
              required
              maxLength={10}
            />
          </div>
          <button type="submit" className="btn-gold" disabled={busy || rem.highlight <= 0}>
            {busy ? "Đang phát..." : "Phát Highlight"}
          </button>
        </form>
      </div>

      <div className="card p-4 md:p-6">
        <h2 className="panel-title mb-3">Mã gần đây</h2>
        <div className="table-wrap">
          <table className="data-table">
            <thead>
              <tr>
                <th>Gói</th>
                <th>Mã</th>
                <th>HL</th>
                <th>Hết hạn</th>
                <th>Trạng thái</th>
              </tr>
            </thead>
            <tbody>
              {(data.sessions || []).length === 0 ? (
                <tr>
                  <td colSpan={5} className="text-mu-muted">
                    Chưa có phiên nào.
                  </td>
                </tr>
              ) : (
                (data.sessions || []).map((s) => (
                  <tr key={s.id}>
                    <td>{s.packageId}</td>
                    <td className="font-mono text-mu-gold">{s.code}</td>
                    <td>{s.highlightCount}</td>
                    <td>{fmtDate(s.expiresAt)}</td>
                    <td>{s.isActive ? "Active" : "Off"}</td>
                  </tr>
                ))
              )}
            </tbody>
          </table>
        </div>
      </div>

      <div className="card p-4 md:p-6">
        <h2 className="panel-title mb-3">Lịch sử phát / nhận</h2>
        <div className="table-wrap">
          <table className="data-table">
            <thead>
              <tr>
                <th>Gói</th>
                <th>Account</th>
                <th>Nhân vật</th>
                <th>Coin</th>
                <th>Thời gian</th>
              </tr>
            </thead>
            <tbody>
              {(data.claims || []).length === 0 ? (
                <tr>
                  <td colSpan={5} className="text-mu-muted">
                    Chưa có giao dịch.
                  </td>
                </tr>
              ) : (
                (data.claims || []).map((c) => (
                  <tr key={c.id}>
                    <td>{c.packageId}</td>
                    <td>{c.claimerAccount}</td>
                    <td>{c.claimerCharacter}</td>
                    <td className="text-xs">
                      {c.wc ? `${c.wc}WC ` : ""}
                      {c.wp ? `${c.wp}WP ` : ""}
                      {c.wg ? `${c.wg}WG` : ""}
                      {!c.wc && !c.wp && !c.wg ? "—" : ""}
                    </td>
                    <td>{fmtDate(c.claimedAt)}</td>
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

function Stat({ label, value }: { label: string; value: string }) {
  return (
    <div className="rounded-lg border border-white/10 bg-black/30 p-3">
      <p className="text-xs text-mu-muted">{label}</p>
      <p className="mt-1 font-semibold text-white">{value}</p>
    </div>
  );
}
