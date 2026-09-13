import Link from "next/link";
import StatBadge from "@/components/StatBadge";
import { getAdminStats } from "@/lib/cms";
import { getDictionary } from "@/lib/i18n/get-dictionary";
import { getLocale } from "@/lib/i18n/locale";

export const dynamic = "force-dynamic";

export async function generateMetadata() {
  const locale = await getLocale();
  const t = getDictionary(locale);
  return { title: t.nav.admin };
}

export default async function AdminDashboardPage() {
  const locale = await getLocale();
  const t = getDictionary(locale);
  let stats = {
    accounts: 0,
    online: 0,
  };

  try {
    stats = await getAdminStats();
  } catch {
    // ignore offline DB during UI boot
  }

  return (
    <div className="space-y-6">
      <div>
        <h1 className="section-title">{t.admin.dashboardTitle}</h1>
        <p className="muted mt-1">{t.admin.dashboardSubtitle}</p>
      </div>

      <div className="grid gap-3 sm:grid-cols-2">
        <StatBadge label={t.admin.online} value={stats.online} tone="lime" />
        <StatBadge
          label={t.admin.accountsStat}
          value={stats.accounts}
          tone="gold"
        />
      </div>

      <div className="grid gap-3 sm:grid-cols-2 lg:grid-cols-3">
        <Link
          href="/admin/gs/ini"
          className="card p-4 transition hover:border-mu-gold/50"
        >
          <h2 className="panel-title">{t.admin.gsIniCard}</h2>
          <p className="muted mt-2 text-sm">{t.admin.gsIniCardHint}</p>
        </Link>
        <Link
          href="/admin/gs/groups"
          className="card p-4 transition hover:border-mu-gold/50"
        >
          <h2 className="panel-title">{t.admin.gsGroupsCard}</h2>
          <p className="muted mt-2 text-sm">{t.admin.gsGroupsCardHint}</p>
        </Link>
        <Link
          href="/admin/gs/balancing"
          className="card p-4 transition hover:border-mu-gold/50"
        >
          <h2 className="panel-title">{t.admin.balancingCard}</h2>
          <p className="muted mt-2 text-sm">{t.admin.balancingCardHint}</p>
        </Link>
        <Link
          href="/admin/enhancement-rates"
          className="card p-4 transition hover:border-mu-gold/50"
        >
          <h2 className="panel-title">Tỉ lệ đập đồ</h2>
          <p className="muted mt-2 text-sm">
            Điều chỉnh Chaos Mix, tỉ lệ ngọc và Luck cho từng cấp tài khoản.
          </p>
        </Link>
        <Link
          href="/admin/ops"
          className="card p-4 transition hover:border-mu-gold/50"
        >
          <h2 className="panel-title">{t.admin.opsCard}</h2>
          <p className="muted mt-2 text-sm">{t.admin.opsCardHint}</p>
        </Link>
        <Link
          href="/admin/hack"
          className="card p-4 transition hover:border-mu-gold/50"
        >
          <h2 className="panel-title">{t.admin.hackCard}</h2>
          <p className="muted mt-2 text-sm">{t.admin.hackCardHint}</p>
        </Link>
      </div>
    </div>
  );
}
