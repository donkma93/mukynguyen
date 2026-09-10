"use client";

import { FormEvent, useState } from "react";
import FlashMessage from "@/components/FlashMessage";

export default function PasswordForm() {
  const [oldPassword, setOldPassword] = useState("");
  const [newPassword, setNewPassword] = useState("");
  const [confirmPassword, setConfirmPassword] = useState("");
  const [error, setError] = useState<string | null>(null);
  const [success, setSuccess] = useState<string | null>(null);
  const [loading, setLoading] = useState(false);

  async function onSubmit(e: FormEvent) {
    e.preventDefault();
    setError(null);
    setSuccess(null);

    if (newPassword !== confirmPassword) {
      setError("Mật khẩu mới xác nhận không khớp");
      return;
    }

    setLoading(true);
    try {
      const res = await fetch("/api/password", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ oldPassword, newPassword, confirmPassword }),
      });
      const data = await res.json().catch(() => ({}));
      if (!res.ok) {
        setError(data.error || data.message || "Đổi mật khẩu thất bại");
        return;
      }
      setSuccess("Đổi mật khẩu thành công.");
      setOldPassword("");
      setNewPassword("");
      setConfirmPassword("");
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
        <label className="label" htmlFor="oldPassword">
          Mật khẩu hiện tại
        </label>
        <input
          id="oldPassword"
          type="password"
          className="input"
          value={oldPassword}
          onChange={(e) => setOldPassword(e.target.value)}
          required
          minLength={4}
          maxLength={10}
        />
      </div>
      <div>
        <label className="label" htmlFor="newPassword">
          Mật khẩu mới
        </label>
        <input
          id="newPassword"
          type="password"
          className="input"
          value={newPassword}
          onChange={(e) => setNewPassword(e.target.value)}
          required
          minLength={4}
          maxLength={10}
        />
      </div>
      <div>
        <label className="label" htmlFor="confirmPassword">
          Xác nhận mật khẩu mới
        </label>
        <input
          id="confirmPassword"
          type="password"
          className="input"
          value={confirmPassword}
          onChange={(e) => setConfirmPassword(e.target.value)}
          required
          minLength={4}
          maxLength={10}
        />
      </div>
      <button type="submit" className="btn-gold" disabled={loading}>
        {loading ? "Đang lưu..." : "Cập nhật mật khẩu"}
      </button>
    </form>
  );
}
