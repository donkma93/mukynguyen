"use client";

import { FormEvent, useEffect, useState } from "react";
import FlashMessage from "@/components/FlashMessage";

type CharacterOption = {
  Name: string;
  cLevel?: number;
  ResetCount?: number;
};

type Props = {
  characters: CharacterOption[];
};

export default function PartnerClaimForm({ characters }: Props) {
  const [code, setCode] = useState("");
  const [characterName, setCharacterName] = useState(characters[0]?.Name || "");
  const [error, setError] = useState<string | null>(null);
  const [success, setSuccess] = useState<string | null>(null);
  const [itemSummary, setItemSummary] = useState<string[]>([]);
  const [loading, setLoading] = useState(false);

  useEffect(() => {
    if (!characterName && characters[0]?.Name) {
      setCharacterName(characters[0].Name);
    }
  }, [characters, characterName]);

  async function onSubmit(e: FormEvent) {
    e.preventDefault();
    setLoading(true);
    setError(null);
    setSuccess(null);
    setItemSummary([]);
    try {
      const res = await fetch("/api/partner/claim", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ code, characterName }),
      });
      const data = await res.json().catch(() => ({}));
      if (!res.ok) {
        setError(data.error || "Nhận quà đối tác thất bại");
        return;
      }
      setSuccess(data.message || "Nhận quà đối tác thành công!");
      setItemSummary(Array.isArray(data.itemSummary) ? data.itemSummary : []);
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
      {itemSummary.length > 0 ? (
        <ul className="list-disc space-y-1 pl-5 text-sm text-gray-300">
          {itemSummary.map((line) => (
            <li key={line}>{line}</li>
          ))}
        </ul>
      ) : null}

      <div>
        <label className="label" htmlFor="partner-code">
          Mã quà đối tác
        </label>
        <input
          id="partner-code"
          className="input uppercase tracking-wider"
          value={code}
          onChange={(e) => setCode(e.target.value)}
          required
          minLength={3}
          maxLength={32}
          placeholder="VD: LIVE-A1B2C3D4"
        />
      </div>

      <div>
        <label className="label" htmlFor="partner-character">
          Nhân vật nhận ngọc / bùa
        </label>
        {characters.length === 0 ? (
          <p className="muted text-sm">Bạn chưa có nhân vật để nhận đồ.</p>
        ) : (
          <select
            id="partner-character"
            className="input"
            value={characterName}
            onChange={(e) => setCharacterName(e.target.value)}
            required
          >
            {characters.map((c) => (
              <option key={c.Name} value={c.Name}>
                {c.Name}
                {typeof c.ResetCount === "number" ? ` · RS ${c.ResetCount}` : ""}
                {typeof c.cLevel === "number" ? ` · Lv ${c.cLevel}` : ""}
              </option>
            ))}
          </select>
        )}
      </div>

      <button
        type="submit"
        className="btn-lime"
        disabled={loading || characters.length === 0}
      >
        {loading ? "Đang nhận..." : "Nhận quà đối tác"}
      </button>
    </form>
  );
}
