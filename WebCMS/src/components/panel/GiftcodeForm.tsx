"use client";

import { FormEvent, useState } from "react";
import FlashMessage from "@/components/FlashMessage";

export default function GiftcodeForm() {
  const [code, setCode] = useState("");
  const [error, setError] = useState<string | null>(null);
  const [success, setSuccess] = useState<string | null>(null);
  const [loading, setLoading] = useState(false);

  async function onSubmit(e: FormEvent) {
    e.preventDefault();
    setLoading(true);
    setError(null);
    setSuccess(null);
    try {
      const res = await fetch("/api/giftcode", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ code }),
      });
      const data = await res.json().catch(() => ({}));
      if (!res.ok) {
        setError(data.error || data.message || "Giftcode không hợp lệ");
        return;
      }
      setSuccess(data.message || "Nhận giftcode thành công!");
      setCode("");
    } catch {
      setError("Không thể kết nối máy chủ.");
    } finally {
      setLoading(false);
    }
  }

  return (
    <form onSubmit={onSubmit} className="space-y-4">
      <FlashMessage type="error" message={error} />
      <FlashMessage type="success" message={success} />
      <div>
        <label className="label" htmlFor="code">
          Mã giftcode
        </label>
        <input
          id="code"
          className="input uppercase tracking-wider"
          value={code}
          onChange={(e) => setCode(e.target.value)}
          required
          minLength={3}
          maxLength={32}
          placeholder="VD: THANGCUOI2026"
        />
      </div>
      <button type="submit" className="btn-lime" disabled={loading}>
        {loading ? "Đang nhận..." : "Nhận thưởng"}
      </button>
    </form>
  );
}
