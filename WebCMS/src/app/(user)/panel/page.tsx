import { redirect } from "next/navigation";
import StatBadge from "@/components/StatBadge";
import VipShopCard from "@/components/panel/VipShopCard";
import { getServerAuthSession } from "@/lib/auth";
import { className, getCash, getCharacters, findAccount } from "@/lib/game";
import { getDictionary } from "@/lib/i18n/get-dictionary";
import { getLocale } from "@/lib/i18n/locale";

export const dynamic = "force-dynamic";

export async function generateMetadata() {
  const locale = await getLocale();
  const t = getDictionary(locale);
  return { title: t.nav.panel };
}

export default async function PanelPage() {
  const session = await getServerAuthSession();
  if (!session?.user?.name) redirect("/login");

  const locale = await getLocale();
  const t = getDictionary(locale);
  const account = session.user.name;
  let cash = { WC: 0, WP: 0, WG: 0, RD: 0, AT: 0 };
  let chars: Awaited<ReturnType<typeof getCharacters>> = [];
  let vip = 0;
  let expireDate: Date | null = null;

  try {
    const [cashData, charData, memb] = await Promise.all([
      getCash(account),
      getCharacters(account),
      findAccount(account),
    ]);
    cash = cashData;
    chars = charData;
    vip = memb?.AccountLevel ?? 0;
    expireDate = memb?.AccountExpireDate ?? null;
  } catch {
    // DB may be offline during UI-only boot
  }

  const numberLocale =
    locale === "vi" ? "vi-VN" : locale === "pt" ? "pt-BR" : locale === "es" ? "es-ES" : "en-US";

  const vipActive = vip >= 1 && (!expireDate || new Date(expireDate).getTime() > Date.now());
  const expireLabel =
    vipActive && expireDate
      ? new Date(expireDate).toLocaleString(numberLocale)
      : vip >= 1 && expireDate
        ? new Date(expireDate).toLocaleString(numberLocale)
        : "";

  return (
    <div className="space-y-6">
      <div>
        <h1 className="section-title">{t.panel.overviewTitle}</h1>
        <p className="muted mt-1">
          {t.panel.welcome} {account}
        </p>
      </div>

      <div className="grid gap-3 sm:grid-cols-2 lg:grid-cols-4">
        <StatBadge
          label="WCoin (WC)"
          value={cash.WC.toLocaleString(numberLocale)}
          tone="gold"
        />
        <StatBadge
          label="WCoinP (WP)"
          value={cash.WP.toLocaleString(numberLocale)}
          tone="lime"
        />
        <StatBadge
          label="Goblin (WG)"
          value={cash.WG.toLocaleString(numberLocale)}
          tone="purple"
        />
        <StatBadge label="VIP" value={vipActive ? "VIP" : "Thường"} tone="danger" />
      </div>

      <VipShopCard
        wc={cash.WC}
        vipActive={vipActive}
        expireLabel={expireLabel}
        numberLocale={numberLocale}
      />

      <div className="grid gap-3 sm:grid-cols-2">
        <div className="card p-4">
          <h2 className="panel-title">{t.panel.extraCash}</h2>
          <p className="mt-3 text-sm text-gray-300">
            Ruud (RD): <span className="text-mu-gold">{cash.RD}</span>
          </p>
          <p className="mt-1 text-sm text-gray-300">
            AT Point: <span className="text-mu-gold">{cash.AT}</span>
          </p>
        </div>
        <div className="card p-4">
          <h2 className="panel-title">{t.panel.goblinAtm}</h2>
          <p className="mt-3 text-sm text-gray-300">
            Goblin Point (WG): <span className="text-mu-gold">{cash.WG}</span>
          </p>
          <p className="mt-1 text-sm text-gray-300">
            {t.panel.account}:{" "}
            <span className="text-mu-lime">{account}</span>
          </p>
        </div>
      </div>

      <div className="card p-4 md:p-6">
        <h2 className="panel-title mb-4">{t.panel.characters}</h2>
        <div className="table-wrap">
          <table className="data-table">
            <thead>
              <tr>
                <th>{t.panel.name}</th>
                <th>{t.panel.class}</th>
                <th>{t.panel.level}</th>
                <th>{t.panel.reset}</th>
                <th>{t.panel.master}</th>
                <th>{t.panel.kill}</th>
              </tr>
            </thead>
            <tbody>
              {chars.length === 0 ? (
                <tr>
                  <td colSpan={6} className="py-6 text-center text-mu-muted">
                    {t.panel.noCharacters}
                  </td>
                </tr>
              ) : (
                chars.map((c) => (
                  <tr key={c.Name}>
                    <td className="font-medium text-white">{c.Name}</td>
                    <td>{className(c.Class)}</td>
                    <td>{c.cLevel}</td>
                    <td className="text-mu-lime">{c.ResetCount}</td>
                    <td>{c.MasterResetCount}</td>
                    <td>{c.Kills}</td>
                  </tr>
                ))
              )}
            </tbody>
          </table>
        </div>
      </div>
    </div>
  );
}
