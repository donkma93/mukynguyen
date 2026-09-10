"use client";

import { FormEvent, useEffect, useState } from "react";
import FlashMessage from "@/components/FlashMessage";

type Character = { name: string; account: string; className: string; cLevel: number; resetCount: number };
type GameItem = { section: number; type: number; index: number; name: string; maxSocket: number };
type Delivery = {
  id: number;
  characterName: string;
  accountId: string;
  itemIndex: number;
  itemLevel: number;
  durability: number;
  skill: boolean;
  luck: boolean;
  option: number;
  excellent: number;
  ancient: number;
  socketCount: number;
  durationSeconds: number;
  quantity: number;
  status: "pending" | "dispatched" | "cancelled";
  note: string | null;
  createdAt: string;
  dispatchedAt: string | null;
};

const initialItem = {
  itemLevel: "0",
  durability: "255",
  option: "0",
  excellent: "0",
  ancient: "0",
  socketCount: "0",
  durationSeconds: "0",
  quantity: "1",
  note: "",
  skill: false,
  luck: false,
};

type NumericItemKey = Exclude<keyof typeof initialItem, "note" | "skill" | "luck">;

const inputClass = "input";
const asNumber = (value: string) => Number(value || 0);
const statusLabel: Record<Delivery["status"], string> = {
  pending: "Đang chờ",
  dispatched: "Đã gửi GS",
  cancelled: "Đã hủy",
};

export default function ItemDeliveryPanel() {
  const [query, setQuery] = useState("");
  const [characters, setCharacters] = useState<Character[]>([]);
  const [selected, setSelected] = useState<Character | null>(null);
  const [itemSearch, setItemSearch] = useState("");
  const [itemMatches, setItemMatches] = useState<GameItem[]>([]);
  const [selectedItem, setSelectedItem] = useState<GameItem | null>(null);
  const [item, setItem] = useState(initialItem);
  const [deliveries, setDeliveries] = useState<Delivery[]>([]);
  const [loading, setLoading] = useState(false);
  const [message, setMessage] = useState<string | null>(null);
  const [error, setError] = useState<string | null>(null);

  const itemIndex = selectedItem?.index ?? -1;

  async function loadDeliveries(characterName?: string) {
    const params = characterName ? `?character=${encodeURIComponent(characterName)}` : "";
    const res = await fetch(`/api/admin/item-deliveries${params}`);
    const data = await res.json().catch(() => ({}));
    if (!res.ok) throw new Error(data.error || "Không tải được lịch sử");
    setDeliveries(Array.isArray(data.items) ? data.items : []);
  }

  useEffect(() => {
    loadDeliveries().catch((cause: unknown) =>
      setError(cause instanceof Error ? cause.message : "Không tải được lịch sử")
    );
  }, []);

  useEffect(() => {
    const term = itemSearch.trim();
    if (term.length < 2 || (selectedItem && term === selectedItem.name)) {
      setItemMatches([]);
      return;
    }
    const timer = window.setTimeout(() => {
      fetch(`/api/admin/items?q=${encodeURIComponent(term)}`)
        .then((res) => res.json())
        .then((data) => setItemMatches(Array.isArray(data.items) ? data.items : []))
        .catch(() => setItemMatches([]));
    }, 180);
    return () => window.clearTimeout(timer);
  }, [itemSearch, selectedItem]);

  async function search(e: FormEvent) {
    e.preventDefault();
    setLoading(true);
    setError(null);
    setMessage(null);
    try {
      const res = await fetch(`/api/admin/characters?q=${encodeURIComponent(query)}`);
      const data = await res.json().catch(() => ({}));
      if (!res.ok) throw new Error(data.error || "Không tìm được nhân vật");
      const found = Array.isArray(data.items) ? (data.items as Character[]) : [];
      setCharacters(found);
      if (found[0]) {
        setSelected(found[0]);
        await loadDeliveries(found[0].name);
      } else {
        setSelected(null);
      }
    } catch (cause) {
      setError(cause instanceof Error ? cause.message : "Không thể kết nối máy chủ");
    } finally {
      setLoading(false);
    }
  }

  async function deliver(e: FormEvent) {
    e.preventDefault();
    if (!selected) {
      setError("Hãy tìm và chọn nhân vật trước");
      return;
    }
    if (!selectedItem) {
      setError("Hãy chọn vật phẩm từ danh sách tên vật phẩm");
      return;
    }
    setLoading(true);
    setError(null);
    setMessage(null);
    try {
      const res = await fetch("/api/admin/item-deliveries", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({
          characterName: selected.name,
          itemIndex,
          itemLevel: asNumber(item.itemLevel),
          durability: asNumber(item.durability),
          skill: item.skill,
          luck: item.luck,
          option: asNumber(item.option),
          excellent: asNumber(item.excellent),
          ancient: asNumber(item.ancient),
          socketCount: asNumber(item.socketCount),
          durationSeconds: asNumber(item.durationSeconds),
          quantity: asNumber(item.quantity),
          note: item.note,
        }),
      });
      const data = await res.json().catch(() => ({}));
      if (!res.ok) throw new Error(data.error || "Không thể tạo lệnh đẩy đồ");
      setMessage(data.message || "Đã xếp vật phẩm vào hàng đợi");
      await loadDeliveries(selected.name);
    } catch (cause) {
      setError(cause instanceof Error ? cause.message : "Không thể kết nối máy chủ");
    } finally {
      setLoading(false);
    }
  }

  async function cancel(id: number) {
    setLoading(true);
    setError(null);
    setMessage(null);
    try {
      const res = await fetch(`/api/admin/item-deliveries?id=${id}`, { method: "DELETE" });
      const data = await res.json().catch(() => ({}));
      if (!res.ok) throw new Error(data.error || "Không thể hủy lệnh");
      setMessage(data.message || "Đã hủy lệnh");
      await loadDeliveries(selected?.name);
    } catch (cause) {
      setError(cause instanceof Error ? cause.message : "Không thể kết nối máy chủ");
    } finally {
      setLoading(false);
    }
  }

  function numeric(key: NumericItemKey, label: string, min: number, max: number) {
    return (
      <div>
        <label className="label" htmlFor={key}>{label}</label>
        <input id={key} className={inputClass} type="number" min={min} max={max} value={String(item[key])}
          onChange={(e) => setItem((current) => ({ ...current, [key]: e.target.value }))} required />
      </div>
    );
  }

  return (
    <div className="space-y-6">
      <FlashMessage type="error" message={error} />
      <FlashMessage type="success" message={message} />

      <form onSubmit={search} className="card flex flex-wrap gap-3 p-4">
        <input className="input max-w-sm" placeholder="Tìm tên nhân vật hoặc Account..." value={query}
          onChange={(e) => setQuery(e.target.value)} />
        <button type="submit" className="btn-gold" disabled={loading}>{loading ? "Đang tìm..." : "Tìm nhân vật"}</button>
      </form>

      {characters.length > 0 && (
        <div className="card p-4">
          <p className="mb-3 text-sm font-semibold text-mu-gold">Chọn nhân vật nhận đồ</p>
          <div className="flex flex-wrap gap-2">
            {characters.map((character) => (
              <button key={character.name} type="button" className={`btn-ghost text-left ${selected?.name === character.name ? "border-mu-gold text-mu-gold" : ""}`}
                onClick={() => { setSelected(character); loadDeliveries(character.name).catch(() => undefined); }}>
                {character.name} <span className="text-xs text-mu-muted">({character.account} · {character.className} · Lv{character.cLevel} · RS{character.resetCount})</span>
              </button>
            ))}
          </div>
        </div>
      )}

      <form onSubmit={deliver} className="card-glow space-y-5 p-5">
        <div>
          <h2 className="panel-title">Đẩy vật phẩm trực tiếp</h2>
          <p className="muted mt-1">
            {selected ? <>Người nhận: <span className="text-mu-gold">{selected.name}</span> ({selected.account})</> : "Chưa chọn nhân vật."}
          </p>
          <p className="mt-2 text-xs text-amber-300/90">Vật phẩm được GameServer tạo khi nhân vật đăng nhập. Nên bảo người chơi chừa chỗ trống trong hòm đồ.</p>
        </div>

        <div className="grid gap-3 sm:grid-cols-2 lg:grid-cols-4">
          <div className="relative sm:col-span-2">
            <label className="label" htmlFor="item-name">Tên vật phẩm</label>
            <input id="item-name" className="input" autoComplete="off" value={itemSearch}
              onChange={(e) => { setItemSearch(e.target.value); setSelectedItem(null); }}
              placeholder="Gõ tên, ví dụ: Thiên Ma Kiếm, Cánh..." required />
            {itemMatches.length > 0 && <div className="absolute z-20 mt-1 max-h-64 w-full overflow-y-auto rounded-md border border-mu-purple/50 bg-[#14131d] p-1 shadow-xl">
              {itemMatches.map((candidate) => <button key={candidate.index} type="button" className="block w-full rounded px-3 py-2 text-left text-sm hover:bg-mu-gold/15"
                onClick={() => { setSelectedItem(candidate); setItemSearch(candidate.name); setItemMatches([]); setItem((current) => ({ ...current, socketCount: String(Math.min(asNumber(current.socketCount), candidate.maxSocket)) })); }}>
                <span className="font-semibold text-mu-gold">{candidate.name}</span><span className="ml-2 text-xs text-mu-muted">Nhóm {candidate.section} · Mã {candidate.type} · Index {candidate.index}</span>
              </button>)}
            </div>}
          </div>
          <div className="rounded-md border border-mu-purple/30 bg-black/20 px-3 py-2">
            <div className="text-xs text-mu-muted">Mã GameServer</div><div className="font-semibold text-mu-gold">{selectedItem ? `Nhóm ${selectedItem.section} · Mã ${selectedItem.type} · Index ${itemIndex}` : "Chưa chọn vật phẩm"}</div>
          </div>
          {numeric("quantity", "Số lượng", 1, 25)}
          {numeric("itemLevel", "Level (+)", 0, 15)}
          {numeric("durability", "Độ bền", 0, 255)}
          {numeric("option", "Option", 0, 7)}
          {numeric("excellent", "Excellent mask", 0, 63)}
          {numeric("ancient", "Ancient", 0, 255)}
          <div>
            <label className="label" htmlFor="socketCount">Số socket</label>
            <input id="socketCount" className={inputClass} type="number" min={0} max={selectedItem?.maxSocket ?? 0} value={item.socketCount}
              disabled={!selectedItem || selectedItem.maxSocket === 0}
              onChange={(e) => setItem((current) => ({ ...current, socketCount: e.target.value }))} required />
            <p className="mt-1 text-xs text-mu-muted">{selectedItem ? (selectedItem.maxSocket > 0 ? `Hỗ trợ tối đa ${selectedItem.maxSocket} socket.` : "Vật phẩm này không hỗ trợ socket.") : "Chọn vật phẩm để kiểm tra socket."}</p>
          </div>
          {numeric("durationSeconds", "Thời hạn (giây, 0=vĩnh viễn)", 0, 2147483647)}
          <div className="flex items-end gap-4 pb-2">
            <label className="flex items-center gap-2 text-sm"><input type="checkbox" checked={item.skill} onChange={(e) => setItem((v) => ({ ...v, skill: e.target.checked }))} /> Skill</label>
            <label className="flex items-center gap-2 text-sm"><input type="checkbox" checked={item.luck} onChange={(e) => setItem((v) => ({ ...v, luck: e.target.checked }))} /> Luck</label>
          </div>
        </div>
        <div>
          <label className="label" htmlFor="delivery-note">Ghi chú GM (tùy chọn)</label>
          <input id="delivery-note" className="input" maxLength={300} value={item.note}
            onChange={(e) => setItem((current) => ({ ...current, note: e.target.value }))} placeholder="Ví dụ: đền bù sự cố 07/09" />
        </div>
        <button type="submit" className="btn-lime" disabled={loading || !selected}>{loading ? "Đang xếp lệnh..." : "Đẩy đồ cho nhân vật"}</button>
      </form>

      <div className="card p-4">
        <div className="mb-3 flex items-center justify-between gap-3"><h2 className="panel-title">Lệnh đẩy đồ {selected ? `— ${selected.name}` : "gần đây"}</h2><button type="button" className="btn-ghost px-3 py-1 text-xs" onClick={() => loadDeliveries(selected?.name).catch((cause) => setError(cause instanceof Error ? cause.message : "Không tải được lịch sử"))}>Làm mới</button></div>
        <div className="table-wrap"><table className="data-table"><thead><tr><th>ID</th><th>Nhân vật</th><th>Item</th><th>Thuộc tính</th><th>Trạng thái</th><th>Thời gian</th><th /></tr></thead>
          <tbody>{deliveries.length === 0 ? <tr><td colSpan={7} className="py-6 text-center text-mu-muted">Chưa có lệnh nào.</td></tr> : deliveries.map((delivery) => <tr key={delivery.id}>
            <td>#{delivery.id}</td><td><span className="text-mu-gold">{delivery.characterName}</span><div className="text-xs text-mu-muted">{delivery.accountId}</div></td>
            <td>Index {delivery.itemIndex} +{delivery.itemLevel} ×{delivery.quantity}<div className="text-xs text-mu-muted">{delivery.note || "—"}</div></td>
            <td>Opt {delivery.option} · Exc {delivery.excellent} · Socket {delivery.socketCount}</td>
            <td><span className={delivery.status === "pending" ? "badge-gold" : delivery.status === "dispatched" ? "badge-lime" : "badge-danger"}>{statusLabel[delivery.status]}</span></td>
            <td className="text-xs text-mu-muted">{new Date(delivery.createdAt).toLocaleString("vi-VN")}</td>
            <td>{delivery.status === "pending" && <button type="button" className="btn-danger px-2 py-1 text-xs" disabled={loading} onClick={() => cancel(delivery.id)}>Hủy</button>}</td>
          </tr>)}</tbody></table></div>
      </div>
    </div>
  );
}
