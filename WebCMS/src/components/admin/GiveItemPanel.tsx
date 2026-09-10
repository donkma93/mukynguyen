"use client";

import { FormEvent, useEffect, useState } from "react";
import FlashMessage from "@/components/FlashMessage";

type BagItem = {
  slot: number;
  index: number;
  level: number;
  skill: number;
  luck: number;
  option: number;
  excellent: number;
  setOption: number;
  durability: number;
  serial: number;
  name?: string;
};

type ItemHit = {
  index: number;
  section: number;
  type: number;
  width: number;
  height: number;
  name: string;
  baseDurability: number;
  maxSocket: number;
};

type Props = {
  characterName: string | null;
  account?: string | null;
};

const emptyForm = {
  section: "0",
  type: "0",
  level: "0",
  skill: "0",
  luck: "0",
  option: "0",
  excellent: "0",
  setOption: "0",
  socketCount: "0",
  durability: "",
};

export default function GiveItemPanel({ characterName, account }: Props) {
  const [form, setForm] = useState(emptyForm);
  const [itemQuery, setItemQuery] = useState("");
  const [hits, setHits] = useState<ItemHit[]>([]);
  const [selectedItem, setSelectedItem] = useState<ItemHit | null>(null);
  const [bagItems, setBagItems] = useState<BagItem[]>([]);
  const [online, setOnline] = useState(false);
  const [extInventory, setExtInventory] = useState(0);
  const [error, setError] = useState<string | null>(null);
  const [message, setMessage] = useState<string | null>(null);
  const [loading, setLoading] = useState(false);
  const [searching, setSearching] = useState(false);

  async function loadInventory(name: string) {
    try {
      const res = await fetch(
        `/api/admin/characters/inventory?name=${encodeURIComponent(name)}`,
        { cache: "no-store" }
      );
      const data = await res.json().catch(() => ({}));
      if (!res.ok) {
        setError(data.error || "Không đọc được túi đồ");
        return;
      }
      setBagItems((data.bagItems || []) as BagItem[]);
      setOnline(Boolean(data.online));
      setExtInventory(Number(data.extInventory ?? 0));
    } catch {
      setError("Không kết nối được API inventory");
    }
  }

  useEffect(() => {
    if (!characterName) {
      setBagItems([]);
      setOnline(false);
      return;
    }
    void loadInventory(characterName);
  }, [characterName]);

  useEffect(() => {
    const q = itemQuery.trim();
    if (q.length < 1) {
      setHits([]);
      return;
    }
    const t = setTimeout(async () => {
      setSearching(true);
      try {
        const res = await fetch(
          `/api/admin/items?q=${encodeURIComponent(q)}`,
          { cache: "no-store" }
        );
        const data = await res.json().catch(() => ({}));
        setHits(Array.isArray(data.items) ? data.items : []);
      } catch {
        setHits([]);
      } finally {
        setSearching(false);
      }
    }, 250);
    return () => clearTimeout(t);
  }, [itemQuery]);

  function pickItem(it: ItemHit) {
    setSelectedItem(it);
    setForm((f) => ({
      ...f,
      section: String(it.section),
      type: String(it.type),
      // Mặc định mở max lỗ nếu item hỗ trợ socket; không hỗ trợ thì 0
      socketCount: String(it.maxSocket > 0 ? it.maxSocket : 0),
    }));
    setItemQuery(it.name);
    setHits([]);
  }

  async function onGive(e: FormEvent) {
    e.preventDefault();
    if (!characterName) {
      setError("Chưa chọn nhân vật");
      return;
    }
    setLoading(true);
    setError(null);
    setMessage(null);
    try {
      const body: Record<string, unknown> = {
        name: characterName,
        section: Number(form.section),
        type: Number(form.type),
        level: Number(form.level),
        skill: Number(form.skill),
        luck: Number(form.luck),
        option: Number(form.option),
        excellent: Number(form.excellent),
        setOption: Number(form.setOption),
        socketCount: Number(form.socketCount),
      };
      if (form.durability.trim() !== "") {
        body.durability = Number(form.durability);
      }
      const res = await fetch("/api/admin/characters/inventory", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(body),
      });
      const data = await res.json().catch(() => ({}));
      if (!res.ok) {
        setError(data.error || "Give item thất bại");
        return;
      }
      setMessage(data.message || "Đã đưa đồ vào túi.");
      setBagItems((data.bagItems || []) as BagItem[]);
      await loadInventory(characterName);
    } catch {
      setError("Không kết nối được máy chủ.");
    } finally {
      setLoading(false);
    }
  }

  if (!characterName) {
    return (
      <div className="card p-5">
        <h2 className="panel-title">Set đồ (Give Item)</h2>
        <p className="muted mt-2 text-sm">Chọn nhân vật ở trên trước.</p>
      </div>
    );
  }

  return (
    <div className="space-y-4">
      <FlashMessage type="error" message={error} />
      <FlashMessage type="success" message={message} />

      <form onSubmit={onGive} className="card space-y-4 p-5">
        <div className="flex flex-wrap items-start justify-between gap-3">
          <div>
            <h2 className="panel-title">Set đồ (Give Item)</h2>
            <p className="muted mt-1 text-sm">
              {characterName}
              {account ? ` · ${account}` : ""} · ExtInv {extInventory}
            </p>
          </div>
          <span
            className={`rounded-full px-3 py-1 text-xs font-semibold ${
              online
                ? "bg-mu-danger/20 text-red-300"
                : "bg-mu-lime/15 text-mu-lime"
            }`}
          >
            {online ? "ONLINE — không set được" : "OFFLINE — có thể set"}
          </span>
        </div>

        <p className="text-xs text-amber-200/90">
          Chỉ set khi nhân vật offline. Đồ vào ô trống trong túi. Tìm theo tên /
          `section type` / index.
        </p>
        <p className="text-xs text-mu-muted">
          <strong className="text-mu-gold">Socket:</strong> điền{" "}
          <code>Socket count</code> = số lỗ trống (1–5). Chỉ item có trong{" "}
          <code>SocketItemType.txt</code> mới mở lỗ được (giống lệnh{" "}
          <code>/make</code>).
        </p>

        <div className="relative">
          <label className="label">Tìm item (Item.txt)</label>
          <input
            className="input"
            placeholder='VD: "Vĩnh Hằng" hoặc 0 19 hoặc 19'
            value={itemQuery}
            onChange={(e) => setItemQuery(e.target.value)}
          />
          {searching ? (
            <p className="mt-1 text-xs text-mu-muted">Đang tìm…</p>
          ) : null}
          {hits.length > 0 ? (
            <ul className="absolute z-20 mt-1 max-h-56 w-full overflow-auto rounded-lg border border-white/10 bg-mu-panel shadow-lg">
              {hits.map((it) => (
                <li key={it.index}>
                  <button
                    type="button"
                    className="flex w-full items-center justify-between gap-2 px-3 py-2 text-left text-sm hover:bg-white/5"
                    onClick={() => pickItem(it)}
                  >
                    <span>
                      <span className="text-mu-gold">{it.name}</span>
                      <span className="ml-2 text-xs text-mu-muted">
                        {it.section}:{it.type} (#{it.index}) · {it.width}x
                        {it.height}
                        {it.maxSocket > 0
                          ? ` · socket≤${it.maxSocket}`
                          : ""}
                      </span>
                    </span>
                  </button>
                </li>
              ))}
            </ul>
          ) : null}
          {selectedItem ? (
            <p className="mt-2 text-xs text-mu-lime">
              Đã chọn: {selectedItem.name} — section {selectedItem.section} type{" "}
              {selectedItem.type} (index {selectedItem.index})
              {selectedItem.maxSocket > 0
                ? ` — hỗ trợ tối đa ${selectedItem.maxSocket} lỗ socket`
                : " — không hỗ trợ socket"}
            </p>
          ) : null}
        </div>

        <div className="grid gap-3 sm:grid-cols-2 lg:grid-cols-4">
          {(
            [
              ["section", "Section"],
              ["type", "Type"],
              ["level", "Level (+0..15)"],
              ["option", "Option (0..7)"],
              ["skill", "Skill (0/1)"],
              ["luck", "Luck (0/1)"],
              ["excellent", "Excellent bitmask"],
              ["setOption", "Ancient/Set"],
              ["socketCount", "Socket count (lỗ trống)"],
              ["durability", "Durability (trống=auto)"],
            ] as const
          ).map(([key, label]) => (
            <div key={key}>
              <label className="label">{label}</label>
              <input
                className="input"
                type="number"
                min={key === "socketCount" ? 0 : undefined}
                max={key === "socketCount" ? 5 : undefined}
                value={form[key]}
                onChange={(e) =>
                  setForm((f) => ({ ...f, [key]: e.target.value }))
                }
              />
            </div>
          ))}
        </div>
        {selectedItem && selectedItem.maxSocket <= 0 && Number(form.socketCount) > 0 ? (
          <p className="text-xs text-red-300">
            Item này không có trong SocketItemType.txt — hãy đặt Socket count = 0
            hoặc chọn item socket (Seed weapon/armor…).
          </p>
        ) : null}

        <div className="flex flex-wrap gap-3">
          <button
            type="submit"
            className="btn-gold"
            disabled={loading || online}
          >
            {loading ? "Đang đưa…" : "Đưa vào túi đồ"}
          </button>
          <button
            type="button"
            className="btn-ghost"
            disabled={loading}
            onClick={() => void loadInventory(characterName)}
          >
            Tải lại túi
          </button>
        </div>
      </form>

      <div className="card overflow-x-auto p-0">
        <div className="border-b border-white/10 px-4 py-3">
          <h3 className="panel-title">Túi đồ hiện tại ({bagItems.length})</h3>
        </div>
        <table className="w-full min-w-[720px] text-left text-sm">
          <thead className="text-xs uppercase tracking-wider text-mu-muted">
            <tr>
              <th className="px-4 py-2">Slot</th>
              <th className="px-4 py-2">Item</th>
              <th className="px-4 py-2">Lv</th>
              <th className="px-4 py-2">Opt</th>
              <th className="px-4 py-2">S/L</th>
              <th className="px-4 py-2">Exc</th>
              <th className="px-4 py-2">Dur</th>
              <th className="px-4 py-2">Serial</th>
            </tr>
          </thead>
          <tbody>
            {bagItems.map((it) => (
              <tr key={`${it.slot}-${it.serial}`} className="border-t border-white/5">
                <td className="px-4 py-2 font-mono">{it.slot}</td>
                <td className="px-4 py-2">
                  <div className="text-mu-gold">{it.name || `#${it.index}`}</div>
                  <div className="text-xs text-mu-muted">index {it.index}</div>
                </td>
                <td className="px-4 py-2">+{it.level}</td>
                <td className="px-4 py-2">{it.option}</td>
                <td className="px-4 py-2">
                  {it.skill ? "S" : "-"}/{it.luck ? "L" : "-"}
                </td>
                <td className="px-4 py-2">{it.excellent}</td>
                <td className="px-4 py-2">{it.durability}</td>
                <td className="px-4 py-2 font-mono text-xs">{it.serial}</td>
              </tr>
            ))}
            {!bagItems.length && (
              <tr>
                <td colSpan={8} className="px-4 py-6 text-center text-mu-muted">
                  Túi trống hoặc chưa đọc được item.
                </td>
              </tr>
            )}
          </tbody>
        </table>
      </div>
    </div>
  );
}
