"use client";

import { FormEvent, useState } from "react";
import FlashMessage from "@/components/FlashMessage";
import GiveItemPanel from "@/components/admin/GiveItemPanel";
import { CHARACTER_CLASS_OPTIONS } from "@/lib/character-classes";

type CharRow = {
  account: string;
  name: string;
  classId: number;
  className: string;
  cLevel: number;
  levelUpPoint: number;
  strength: number;
  dexterity: number;
  vitality: number;
  energy: number;
  leadership: number;
  resetCount: number;
  masterResetCount: number;
  money: number;
  mapNumber: number | null;
  mapPosX: number | null;
  mapPosY: number | null;
  ctlCode: number;
  rDanhHieu: number;
  rQuanHam: number;
  rTuLuyen: number;
  rHonHoan: number;
  rNewVip: number;
  rHuyChuong: number;
};

const emptyForm = {
  classId: "0",
  cLevel: "1",
  levelUpPoint: "0",
  strength: "0",
  dexterity: "0",
  vitality: "0",
  energy: "0",
  leadership: "0",
  resetCount: "0",
  masterResetCount: "0",
  money: "0",
  mapNumber: "0",
  mapPosX: "125",
  mapPosY: "125",
  ctlCode: "0",
  rDanhHieu: "0",
  rQuanHam: "0",
  rTuLuyen: "0",
  rHonHoan: "0",
  rNewVip: "0",
  rHuyChuong: "0",
};

export default function CharactersPanel() {
  const [q, setQ] = useState("");
  const [rows, setRows] = useState<CharRow[]>([]);
  const [selected, setSelected] = useState<CharRow | null>(null);
  const [form, setForm] = useState(emptyForm);
  const [message, setMessage] = useState<string | null>(null);
  const [error, setError] = useState<string | null>(null);
  const [loading, setLoading] = useState(false);

  function fillForm(row: CharRow) {
    setSelected(row);
    setForm({
      classId: String(row.classId ?? 0),
      cLevel: String(row.cLevel ?? 1),
      levelUpPoint: String(row.levelUpPoint ?? 0),
      strength: String(row.strength ?? 0),
      dexterity: String(row.dexterity ?? 0),
      vitality: String(row.vitality ?? 0),
      energy: String(row.energy ?? 0),
      leadership: String(row.leadership ?? 0),
      resetCount: String(row.resetCount ?? 0),
      masterResetCount: String(row.masterResetCount ?? 0),
      money: String(row.money ?? 0),
      mapNumber: String(row.mapNumber ?? 0),
      mapPosX: String(row.mapPosX ?? 125),
      mapPosY: String(row.mapPosY ?? 125),
      ctlCode: String(row.ctlCode ?? 0),
      rDanhHieu: String(row.rDanhHieu ?? 0),
      rQuanHam: String(row.rQuanHam ?? 0),
      rTuLuyen: String(row.rTuLuyen ?? 0),
      rHonHoan: String(row.rHonHoan ?? 0),
      rNewVip: String(row.rNewVip ?? 0),
      rHuyChuong: String(row.rHuyChuong ?? 0),
    });
  }

  async function search(e?: FormEvent) {
    e?.preventDefault();
    setLoading(true);
    setError(null);
    setMessage(null);
    try {
      const res = await fetch(`/api/admin/characters?q=${encodeURIComponent(q)}`);
      const data = await res.json().catch(() => ({}));
      if (!res.ok) {
        setError(data.error || "Tìm kiếm thất bại");
        return;
      }
      const list = (data.items || []) as CharRow[];
      setRows(Array.isArray(list) ? list : []);
      if (Array.isArray(list) && list[0]) fillForm(list[0]);
      else setSelected(null);
    } catch {
      setError("Không thể kết nối máy chủ.");
    } finally {
      setLoading(false);
    }
  }

  async function save(e: FormEvent) {
    e.preventDefault();
    if (!selected?.name) {
      setError("Chưa chọn nhân vật");
      return;
    }
    setLoading(true);
    setError(null);
    setMessage(null);
    try {
      const res = await fetch("/api/admin/characters", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({
          name: selected.name,
          classId: Number(form.classId),
          cLevel: Number(form.cLevel),
          levelUpPoint: Number(form.levelUpPoint),
          strength: Number(form.strength),
          dexterity: Number(form.dexterity),
          vitality: Number(form.vitality),
          energy: Number(form.energy),
          leadership: Number(form.leadership),
          resetCount: Number(form.resetCount),
          masterResetCount: Number(form.masterResetCount),
          money: Number(form.money),
          mapNumber: Number(form.mapNumber),
          mapPosX: Number(form.mapPosX),
          mapPosY: Number(form.mapPosY),
          ctlCode: Number(form.ctlCode),
          rDanhHieu: Number(form.rDanhHieu),
          rQuanHam: Number(form.rQuanHam),
          rTuLuyen: Number(form.rTuLuyen),
          rHonHoan: Number(form.rHonHoan),
          rNewVip: Number(form.rNewVip),
          rHuyChuong: Number(form.rHuyChuong),
          resetPointBonusApplied: Number(form.resetCount) > (selected.resetCount ?? 0),
        }),
      });
      const data = await res.json().catch(() => ({}));
      if (!res.ok) {
        setError(data.error || "Cập nhật thất bại");
        return;
      }
      setMessage(data.message || "Đã cập nhật");
      if (data.item) {
        fillForm(data.item as CharRow);
        setRows((prev) =>
          prev.map((r) => (r.name === data.item.name ? (data.item as CharRow) : r))
        );
      }
    } catch {
      setError("Không thể kết nối máy chủ.");
    } finally {
      setLoading(false);
    }
  }

  function field(
    key: keyof typeof emptyForm,
    label: string,
    opts?: { min?: number; max?: number }
  ) {
    return (
      <div>
        <label className="label" htmlFor={key}>
          {label}
        </label>
        <input
          id={key}
          className="input"
          type="number"
          value={form[key]}
          min={opts?.min}
          max={opts?.max}
          onChange={(e) => setForm((f) => ({ ...f, [key]: e.target.value }))}
          required
        />
      </div>
    );
  }

  function updateResetCount(value: string) {
    const resetCount = Number(value);
    const currentReset = selected?.resetCount ?? 0;
    const bonus = Number.isFinite(resetCount)
      ? Math.max(0, Math.trunc(resetCount) - currentReset) * 300
      : 0;
    const basePoints = selected?.levelUpPoint ?? 0;

    setForm((f) => ({
      ...f,
      resetCount: value,
      levelUpPoint: String(basePoints + bonus),
    }));
  }

  const classOptions = (() => {
    const known = CHARACTER_CLASS_OPTIONS.map((o) => o.value);
    const current = Number(form.classId);
    if (!Number.isNaN(current) && !known.includes(current as (typeof CHARACTER_CLASS_OPTIONS)[number]["value"])) {
      return [...CHARACTER_CLASS_OPTIONS, { value: current, label: `Class ${current}` }];
    }
    return [...CHARACTER_CLASS_OPTIONS];
  })();

  return (
    <div className="space-y-6">
      <FlashMessage type="error" message={error} />
      <FlashMessage type="success" message={message} />

      <form onSubmit={search} className="card flex flex-wrap gap-3 p-4">
        <input
          className="input max-w-sm"
          placeholder="Tìm theo tên NV hoặc Account..."
          value={q}
          onChange={(e) => setQ(e.target.value)}
        />
        <button type="submit" className="btn-gold" disabled={loading}>
          {loading ? "Đang tìm..." : "Tìm nhân vật"}
        </button>
      </form>

      <div className="grid gap-6 lg:grid-cols-[280px_1fr]">
        <div className="card p-4">
          <h2 className="panel-title mb-3">Kết quả</h2>
          {rows.length === 0 ? (
            <p className="muted text-sm">Chưa có kết quả.</p>
          ) : (
            <ul className="space-y-1">
              {rows.map((r) => (
                <li key={r.name}>
                  <button
                    type="button"
                    className={`w-full rounded-md px-3 py-2 text-left text-sm transition ${
                      selected?.name === r.name
                        ? "bg-mu-gold/15 text-mu-gold"
                        : "hover:bg-white/5"
                    }`}
                    onClick={() => fillForm(r)}
                  >
                    <div className="font-semibold">{r.name}</div>
                    <div className="text-xs text-mu-muted">
                      {r.account} · {r.className} · Lv{r.cLevel} · RS{r.resetCount}
                    </div>
                  </button>
                </li>
              ))}
            </ul>
          )}
        </div>

        <form onSubmit={save} className="card-glow space-y-5 p-5">
          <div>
            <h2 className="panel-title">Chỉnh set & chỉ số nhân vật</h2>
            {selected ? (
              <p className="muted mt-1">
                {selected.name} ({selected.className}) — Account: {selected.account}
              </p>
            ) : (
              <p className="muted mt-1">Chọn một nhân vật để sửa.</p>
            )}
            <p className="mt-2 text-xs text-amber-300/90">
              Lưu ý: nhân vật nên offline trước khi sửa chỉ số hoặc set đồ.
            </p>
          </div>

          <div>
            <h3 className="mb-2 text-sm font-semibold text-mu-gold">Set nhân vật</h3>
            <div className="grid gap-3 sm:grid-cols-2 lg:grid-cols-3">
              <div>
                <label className="label" htmlFor="classId">
                  Class
                </label>
                <select
                  id="classId"
                  className="input"
                  value={form.classId}
                  onChange={(e) => setForm((f) => ({ ...f, classId: e.target.value }))}
                >
                  {classOptions.map((o) => (
                    <option key={o.value} value={o.value}>
                      {o.label}
                    </option>
                  ))}
                </select>
              </div>
              {field("ctlCode", "CtlCode", { min: 0, max: 255 })}
              {field("rDanhHieu", "Danh hiệu (rDanhHieu)", { min: 0 })}
              {field("rQuanHam", "Quân hàm (rQuanHam)", { min: 0 })}
              {field("rTuLuyen", "Tu luyện (rTuLuyen)", { min: 0 })}
              {field("rHonHoan", "Hồn hoàn (rHonHoan)", { min: 0 })}
              {field("rNewVip", "VIP (rNewVip)", { min: 0 })}
              {field("rHuyChuong", "Huy chương (rHuyChuong)", { min: 0 })}
            </div>
          </div>

          <div>
            <h3 className="mb-2 text-sm font-semibold text-mu-gold">Chỉ số & tiến độ</h3>
            <div className="grid gap-3 sm:grid-cols-2 lg:grid-cols-3">
              {field("cLevel", "Level", { min: 1, max: 400 })}
              {field("levelUpPoint", "Điểm cộng còn lại (+300 mỗi Reset tăng)", { min: 0 })}
              <div>
                <label className="label" htmlFor="resetCount">
                  Reset
                </label>
                <input
                  id="resetCount"
                  className="input"
                  type="number"
                  value={form.resetCount}
                  min={0}
                  max={200}
                  onChange={(e) => updateResetCount(e.target.value)}
                  required
                />
              </div>
              {field("masterResetCount", "Master Reset", { min: 0 })}
              {field("money", "Zen (Money)", { min: 0 })}
              {field("strength", "Strength", { min: 0, max: 65000 })}
              {field("dexterity", "Dexterity", { min: 0, max: 65000 })}
              {field("vitality", "Vitality", { min: 0, max: 65000 })}
              {field("energy", "Energy", { min: 0, max: 65000 })}
              {field("leadership", "Leadership (DL)", { min: 0, max: 65000 })}
              {field("mapNumber", "MapNumber", { min: 0, max: 255 })}
              {field("mapPosX", "MapPosX", { min: 0, max: 255 })}
              {field("mapPosY", "MapPosY", { min: 0, max: 255 })}
            </div>
          </div>

          <button type="submit" className="btn-gold" disabled={loading || !selected}>
            {loading ? "Đang lưu..." : "Lưu set & chỉ số"}
          </button>
        </form>
      </div>

      <GiveItemPanel
        characterName={selected?.name ?? null}
        account={selected?.account ?? null}
      />
    </div>
  );
}
