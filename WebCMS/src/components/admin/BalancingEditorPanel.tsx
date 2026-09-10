"use client";

import Link from "next/link";
import { useCallback, useEffect, useMemo, useState } from "react";
import FlashMessage from "@/components/FlashMessage";
import {
  BALANCE_CLASS_CODES,
  BALANCE_CLASS_META,
  buildBalancingUpdates,
  parseIniNumber,
  type BalanceClassCode,
} from "@/lib/gs/balancing";

type DraftMatrix = Record<BalanceClassCode, Record<BalanceClassCode, string>>;
type DraftClassMap = Record<BalanceClassCode, string>;

function emptyMatrixDraft(): DraftMatrix {
  const matrix = {} as DraftMatrix;
  for (const atk of BALANCE_CLASS_CODES) {
    matrix[atk] = {} as Record<BalanceClassCode, string>;
    for (const def of BALANCE_CLASS_CODES) {
      matrix[atk][def] = "";
    }
  }
  return matrix;
}

function emptyClassDraft(): DraftClassMap {
  const out = {} as DraftClassMap;
  for (const code of BALANCE_CLASS_CODES) out[code] = "";
  return out;
}

function entriesToDrafts(entries: { key: string; value: string }[]) {
  const map: Record<string, string> = {};
  for (const e of entries) map[e.key] = e.value;

  const matrix = emptyMatrixDraft();
  const pvp = emptyClassDraft();
  const pvm = emptyClassDraft();

  for (const atk of BALANCE_CLASS_CODES) {
    const pvpN = parseIniNumber(map[`${atk}DamageRatePvP`]);
    const pvmN = parseIniNumber(map[`${atk}DamageRatePvM`]);
    pvp[atk] = pvpN == null ? "" : String(pvpN);
    pvm[atk] = pvmN == null ? "" : String(pvmN);
    for (const def of BALANCE_CLASS_CODES) {
      const n = parseIniNumber(map[`${atk}DamageRateTo${def}`]);
      matrix[atk][def] = n == null ? "" : String(n);
    }
  }

  return { matrix, pvp, pvm };
}

export default function BalancingEditorPanel() {
  const [baseline, setBaseline] = useState<{
    matrix: DraftMatrix;
    pvp: DraftClassMap;
    pvm: DraftClassMap;
  } | null>(null);
  const [matrix, setMatrix] = useState<DraftMatrix>(emptyMatrixDraft);
  const [pvp, setPvp] = useState<DraftClassMap>(emptyClassDraft);
  const [pvm, setPvm] = useState<DraftClassMap>(emptyClassDraft);
  const [fileName, setFileName] = useState("");
  const [loading, setLoading] = useState(false);
  const [saving, setSaving] = useState(false);
  const [error, setError] = useState<string | null>(null);
  const [message, setMessage] = useState<string | null>(null);

  const load = useCallback(async () => {
    setLoading(true);
    setError(null);
    setMessage(null);
    try {
      const res = await fetch("/api/admin/gs/ini/character", { cache: "no-store" });
      const data = await res.json().catch(() => ({}));
      if (!res.ok) {
        setError(data.error || "Không đọc được Character.ini");
        return;
      }
      setFileName(String(data.fileName || "GameServerInfo - Character.ini"));
      const drafts = entriesToDrafts((data.entries || []) as { key: string; value: string }[]);
      setBaseline(drafts);
      setMatrix(drafts.matrix);
      setPvp(drafts.pvp);
      setPvm(drafts.pvm);
    } catch {
      setError("Không kết nối được API INI");
    } finally {
      setLoading(false);
    }
  }, []);

  useEffect(() => {
    void load();
  }, [load]);

  const dirtyCount = useMemo(() => {
    if (!baseline) return 0;
    let n = 0;
    for (const atk of BALANCE_CLASS_CODES) {
      if ((pvp[atk] ?? "") !== (baseline.pvp[atk] ?? "")) n++;
      if ((pvm[atk] ?? "") !== (baseline.pvm[atk] ?? "")) n++;
      for (const def of BALANCE_CLASS_CODES) {
        if ((matrix[atk][def] ?? "") !== (baseline.matrix[atk][def] ?? "")) n++;
      }
    }
    return n;
  }, [baseline, matrix, pvp, pvm]);

  const warnOutside = useMemo(() => {
    const warnings: string[] = [];
    for (const atk of BALANCE_CLASS_CODES) {
      for (const def of BALANCE_CLASS_CODES) {
        const n = Number.parseInt(matrix[atk][def], 10);
        if (Number.isFinite(n) && (n < 1 || n > 200)) {
          warnings.push(`${atk}→${def}=${n}`);
        }
      }
      for (const [label, map] of [
        ["PvP", pvp],
        ["PvM", pvm],
      ] as const) {
        const n = Number.parseInt(map[atk], 10);
        if (Number.isFinite(n) && (n < 1 || n > 200)) {
          warnings.push(`${atk} ${label}=${n}`);
        }
      }
    }
    return warnings.slice(0, 8);
  }, [matrix, pvp, pvm]);

  async function onSave() {
    setSaving(true);
    setError(null);
    setMessage(null);
    try {
      if (!baseline || dirtyCount === 0) {
        setMessage("Không có thay đổi.");
        setSaving(false);
        return;
      }

      const dirtyMatrix: Partial<
        Record<BalanceClassCode, Partial<Record<BalanceClassCode, number>>>
      > = {};
      const dirtyPvp: Partial<Record<BalanceClassCode, number>> = {};
      const dirtyPvm: Partial<Record<BalanceClassCode, number>> = {};

      for (const atk of BALANCE_CLASS_CODES) {
        if ((pvp[atk] ?? "") !== (baseline.pvp[atk] ?? "")) {
          const n = Number.parseInt(pvp[atk], 10);
          if (Number.isFinite(n)) dirtyPvp[atk] = n;
        }
        if ((pvm[atk] ?? "") !== (baseline.pvm[atk] ?? "")) {
          const n = Number.parseInt(pvm[atk], 10);
          if (Number.isFinite(n)) dirtyPvm[atk] = n;
        }
        for (const def of BALANCE_CLASS_CODES) {
          if ((matrix[atk][def] ?? "") !== (baseline.matrix[atk][def] ?? "")) {
            const n = Number.parseInt(matrix[atk][def], 10);
            if (!Number.isFinite(n)) continue;
            if (!dirtyMatrix[atk]) dirtyMatrix[atk] = {};
            dirtyMatrix[atk]![def] = n;
          }
        }
      }

      const updates = buildBalancingUpdates({
        matrix: dirtyMatrix,
        pvp: dirtyPvp,
        pvm: dirtyPvm,
      });
      if (!Object.keys(updates).length) {
        setMessage("Không có giá trị hợp lệ để lưu.");
        setSaving(false);
        return;
      }

      const res = await fetch("/api/admin/gs/ini/character", {
        method: "PUT",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ mode: "keys", updates }),
      });
      const data = await res.json().catch(() => ({}));
      if (!res.ok) {
        setError(data.error || "Lưu thất bại");
        return;
      }
      const changed = Array.isArray(data.changed) ? data.changed.length : 0;
      setMessage(
        data.message ||
          `Đã lưu ${changed} key. Restart GameServer để áp dụng.`
      );
      await load();
    } catch {
      setError("Không kết nối được API lưu INI");
    } finally {
      setSaving(false);
    }
  }

  function setMatrixCell(atk: BalanceClassCode, def: BalanceClassCode, value: string) {
    const cleaned = value.replace(/[^\d-]/g, "");
    setMatrix((prev) => ({
      ...prev,
      [atk]: { ...prev[atk], [def]: cleaned },
    }));
  }

  return (
    <div className="space-y-6">
      <FlashMessage type="error" message={error} />
      <FlashMessage type="success" message={message} />

      <div className="card space-y-4 p-5">
        <div className="flex flex-wrap items-center gap-3">
          <p className="text-sm text-mu-muted">
            File: <span className="text-mu-gold">{fileName || "…"}</span>
            {loading ? " — đang tải…" : ""}
            {dirtyCount > 0 ? ` — ${dirtyCount} ô đã đổi` : ""}
          </p>
          <div className="ml-auto flex flex-wrap gap-2">
            <Link href="/balancing" className="btn-ghost" target="_blank">
              Xem trang public
            </Link>
            <Link href="/admin/gs/ini?slug=character" className="btn-ghost">
              INI đầy đủ
            </Link>
            <Link href="/admin/ops" className="btn-lime">
              Ops / Restart
            </Link>
          </div>
        </div>

        {warnOutside.length > 0 ? (
          <p className="rounded-lg border border-amber-500/40 bg-amber-500/10 px-3 py-2 text-xs text-amber-200">
            Giá trị ngoài khoảng gợi ý 1–200: {warnOutside.join(", ")}
            {warnOutside.length >= 8 ? "…" : ""}. Vẫn lưu được (clamp 0–500).
          </p>
        ) : null}

        <div>
          <h2 className="panel-title">Ma trận DamageRateTo (Atk → Def)</h2>
          <div className="mt-3 overflow-x-auto rounded-lg border border-white/10">
            <table className="w-full min-w-[720px] text-sm">
              <thead className="bg-mu-panel text-xs uppercase tracking-wider text-mu-muted">
                <tr>
                  <th className="px-2 py-2 text-left">Atk \\ Def</th>
                  {BALANCE_CLASS_CODES.map((code) => (
                    <th key={code} className="px-1 py-2 text-center text-mu-gold">
                      {code}
                    </th>
                  ))}
                </tr>
              </thead>
              <tbody>
                {BALANCE_CLASS_CODES.map((atk) => (
                  <tr key={atk} className="border-t border-white/5">
                    <th className="px-2 py-1.5 text-left">
                      <span className="text-mu-lime">{atk}</span>
                      <span className="ml-1 text-[10px] font-normal text-mu-muted">
                        {BALANCE_CLASS_META[atk].full}
                      </span>
                    </th>
                    {BALANCE_CLASS_CODES.map((def) => {
                      const dirty =
                        baseline != null &&
                        (matrix[atk][def] ?? "") !== (baseline.matrix[atk][def] ?? "");
                      return (
                        <td key={def} className="px-1 py-1">
                          <input
                            className={`input px-1 py-1 text-center font-mono text-sm ${
                              dirty ? "border-mu-gold/60 bg-mu-gold/10" : ""
                            }`}
                            inputMode="numeric"
                            value={matrix[atk][def]}
                            onChange={(e) => setMatrixCell(atk, def, e.target.value)}
                            aria-label={`${atk} to ${def}`}
                          />
                        </td>
                      );
                    })}
                  </tr>
                ))}
              </tbody>
            </table>
          </div>
        </div>

        <div className="grid gap-4 lg:grid-cols-2">
          {(
            [
              ["PvP — DamageRatePvP", pvp, setPvp],
              ["PvM — DamageRatePvM", pvm, setPvm],
            ] as const
          ).map(([title, map, setter]) => (
            <div key={title}>
              <h2 className="panel-title">{title}</h2>
              <div className="mt-3 grid grid-cols-2 gap-2 sm:grid-cols-3 xl:grid-cols-4">
                {BALANCE_CLASS_CODES.map((code) => {
                  const dirty =
                    baseline != null &&
                    (map[code] ?? "") !==
                      (title.startsWith("PvP")
                        ? baseline.pvp[code]
                        : baseline.pvm[code]);
                  return (
                    <label key={code} className="block">
                      <span className="label">
                        {code}{" "}
                        <span className="font-normal normal-case text-mu-muted">
                          {BALANCE_CLASS_META[code].short}
                        </span>
                      </span>
                      <input
                        className={`input font-mono ${
                          dirty ? "border-mu-gold/60 bg-mu-gold/10" : ""
                        }`}
                        inputMode="numeric"
                        value={map[code]}
                        onChange={(e) =>
                          setter((prev) => ({
                            ...prev,
                            [code]: e.target.value.replace(/[^\d-]/g, ""),
                          }))
                        }
                      />
                    </label>
                  );
                })}
              </div>
            </div>
          ))}
        </div>

        <div className="flex flex-wrap gap-3">
          <button
            type="button"
            className="btn-gold"
            disabled={saving || loading}
            onClick={() => void onSave()}
          >
            {saving ? "Đang lưu…" : "Lưu + backup"}
          </button>
          <button
            type="button"
            className="btn-ghost"
            disabled={loading || saving}
            onClick={() => void load()}
          >
            Tải lại
          </button>
        </div>
      </div>
    </div>
  );
}
