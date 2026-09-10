import Link from "next/link";
import { VIP_PACKAGES } from "@/lib/vip-shop";

type Props = {
  numberLocale: string;
  loggedIn: boolean;
};

export default function HomeVipShopTeaser({ numberLocale, loggedIn }: Props) {
  return (
    <section className="relative mx-auto max-w-6xl px-4 py-16 md:py-20">
      <div className="pointer-events-none absolute inset-x-0 top-0 h-px mu-section-line" />
      <div className="mb-8 max-w-2xl">
        <p className="mb-2 text-xs font-semibold uppercase tracking-[0.3em] text-mu-gold">
          VIP
        </p>
        <h2 className="section-title">Một loại VIP — mua bằng WCoin</h2>
        <p className="muted mt-2">
          Không còn VIP 1 / 2 / 3. Kích hoạt trên trang chủ hoặc trong game với cùng quyền lợi.
        </p>
      </div>
      <div className="grid gap-4 sm:grid-cols-3">
        {VIP_PACKAGES.map((pack) => (
          <div key={pack.id} className="card p-5">
            <h3 className="panel-title">{pack.titleVi}</h3>
            <p className="mt-3 text-2xl font-bold text-mu-gold">
              {pack.priceWc.toLocaleString(numberLocale)}
              <span className="ml-1 text-sm font-medium text-mu-muted">WC</span>
            </p>
            <p className="mt-1 text-sm text-gray-300">{pack.days} ngày</p>
          </div>
        ))}
      </div>
      <div className="mt-6">
        <Link href={loggedIn ? "/panel" : "/login"} className="btn-gold inline-flex">
          {loggedIn ? "Mua VIP tại Panel" : "Đăng nhập để mua VIP"}
        </Link>
      </div>
    </section>
  );
}
