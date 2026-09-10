"use client";

import { useEffect, useState } from "react";
import { useRouter } from "next/navigation";
import FlashMessage from "@/components/FlashMessage";
import { VIP_PACKAGES, type VipPackage, type VipPackageId } from "@/lib/vip-shop";

type Props = {
  wc: number;
  vipActive: boolean;
  expireLabel: string;
  numberLocale: string;
};

export default function VipShopCard({
  wc,
  vipActive,
  expireLabel,
  numberLocale,
}: Props) {
  const router = useRouter();
  const [loadingId, setLoadingId] = useState<VipPackageId | null>(null);
  const [pending, setPending] = useState<VipPackage | null>(null);
  const [error, setError] = useState<string | null>(null);
  const [success, setSuccess] = useState<string | null>(null);
  const [balance, setBalance] = useState(wc);

  useEffect(() => {
    setBalance(wc);
  }, [wc]);

  useEffect(() => {
    if (!pending) return;
    function onKey(e: KeyboardEvent) {
      if (e.key === "Escape" && loadingId === null) setPending(null);
    }
    window.addEventListener("keydown", onKey);
    return () => window.removeEventListener("keydown", onKey);
  }, [pending, loadingId]);

  function requestBuy(pack: VipPackage) {
    if (balance < pack.priceWc || loadingId !== null) return;
    setError(null);
    setSuccess(null);
    setPending(pack);
  }

  function cancelConfirm() {
    if (loadingId !== null) return;
    setPending(null);
  }

  async function confirmBuy() {
    if (!pending || loadingId !== null) return;
    const pack = pending;
    setLoadingId(pack.id);
    setError(null);
    setSuccess(null);
    try {
      const res = await fetch("/api/vip/purchase", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ packageId: pack.id }),
      });
      const data = await res.json().catch(() => ({}));
      if (!res.ok) {
        setError(data.error || "Mua VIP thất bại");
        return;
      }
      if (typeof data.wcLeft === "number") setBalance(data.wcLeft);
      setSuccess(data.message || "Kích hoạt VIP thành công!");
      setPending(null);
      router.refresh();
    } catch {
      setError("Không thể kết nối máy chủ.");
    } finally {
      setLoadingId(null);
    }
  }

  const afterBuy = pending ? balance - pending.priceWc : 0;

  return (
    <div className="card relative space-y-4 p-4 md:p-6">
      <div className="flex flex-wrap items-start justify-between gap-3">
        <div>
          <h2 className="panel-title">Cửa hàng VIP</h2>
          <p className="muted mt-1 text-sm">
            Một loại VIP duy nhất. Mua bằng WCoin (WC) — gia hạn cộng dồn ngày còn lại.
          </p>
        </div>
        <div className="text-right text-sm">
          <p className="text-gray-300">
            WCoin:{" "}
            <span className="font-semibold text-mu-gold">
              {balance.toLocaleString(numberLocale)}
            </span>
          </p>
          <p className="mt-1 text-gray-300">
            Trạng thái:{" "}
            <span className={vipActive ? "text-mu-lime" : "text-mu-muted"}>
              {vipActive ? "VIP" : "Thường"}
            </span>
          </p>
          {expireLabel ? (
            <p className="mt-1 text-xs text-mu-muted">Hết hạn: {expireLabel}</p>
          ) : null}
        </div>
      </div>

      <FlashMessage type="error" message={error} />
      <FlashMessage type="success" message={success} />

      <div className="grid gap-3 sm:grid-cols-3">
        {VIP_PACKAGES.map((pack) => {
          const canBuy = balance >= pack.priceWc;
          const busy = loadingId === pack.id;
          return (
            <div
              key={pack.id}
              className="rounded-lg border border-white/10 bg-black/30 p-4"
            >
              <p className="text-sm font-semibold text-white">{pack.titleVi}</p>
              <p className="mt-1 text-2xl font-bold text-mu-gold">
                {pack.priceWc.toLocaleString(numberLocale)}
                <span className="ml-1 text-sm font-medium text-mu-muted">WC</span>
              </p>
              <p className="mt-1 text-xs text-mu-muted">{pack.days} ngày VIP</p>
              <button
                type="button"
                className="btn-gold mt-4 w-full"
                disabled={!canBuy || loadingId !== null}
                onClick={() => requestBuy(pack)}
              >
                {busy ? "Đang mua..." : canBuy ? "Mua & kích hoạt" : "Không đủ WC"}
              </button>
            </div>
          );
        })}
      </div>

      {pending ? (
        <div
          className="fixed inset-0 z-50 flex items-center justify-center bg-black/70 p-4"
          role="dialog"
          aria-modal="true"
          aria-labelledby="vip-confirm-title"
          onClick={cancelConfirm}
        >
          <div
            className="card w-full max-w-md space-y-4 p-5 shadow-xl"
            onClick={(e) => e.stopPropagation()}
          >
            <h3 id="vip-confirm-title" className="panel-title">
              Xác nhận mua VIP
            </h3>
            <p className="text-sm text-gray-300">
              Bạn sắp mua{" "}
              <span className="font-semibold text-white">{pending.titleVi}</span> (
              {pending.days} ngày) với giá{" "}
              <span className="font-semibold text-mu-gold">
                {pending.priceWc.toLocaleString(numberLocale)} WC
              </span>
              .
            </p>
            <ul className="space-y-1 text-sm text-gray-300">
              <li>
                WCoin hiện tại:{" "}
                <span className="text-mu-gold">{balance.toLocaleString(numberLocale)}</span>
              </li>
              <li>
                Sau khi mua còn:{" "}
                <span className="text-mu-lime">{afterBuy.toLocaleString(numberLocale)} WC</span>
              </li>
              {vipActive ? (
                <li className="text-xs text-mu-muted">
                  Đã có VIP — thời hạn sẽ được cộng thêm vào ngày hết hạn hiện tại.
                </li>
              ) : null}
            </ul>
            <div className="flex flex-wrap gap-2 pt-1">
              <button
                type="button"
                className="btn-gold flex-1"
                disabled={loadingId !== null}
                onClick={confirmBuy}
              >
                {loadingId === pending.id ? "Đang mua..." : "Xác nhận mua"}
              </button>
              <button
                type="button"
                className="btn-ghost flex-1"
                disabled={loadingId !== null}
                onClick={cancelConfirm}
              >
                Hủy
              </button>
            </div>
          </div>
        </div>
      ) : null}
    </div>
  );
}
