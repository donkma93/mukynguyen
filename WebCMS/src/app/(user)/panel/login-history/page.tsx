import { redirect } from "next/navigation";
import { getServerAuthSession } from "@/lib/auth";
import { getDictionary } from "@/lib/i18n/get-dictionary";
import { getLocale } from "@/lib/i18n/locale";
import { getGameLoginActivity, listLoginActivity } from "@/lib/login-activity";
import { publicIp } from "@/lib/security/client-ip";

export const dynamic = "force-dynamic";

function checkHostUrl(ip: string) {
  return `https://check-host.net/ip-info?host=${encodeURIComponent(ip)}`;
}

export default async function LoginHistoryPage() {
  const session = await getServerAuthSession();
  if (!session?.user?.name || session.user.role !== "user") redirect("/login");

  const locale = await getLocale();
  const t = getDictionary(locale);
  const dateLocale =
    locale === "vi" ? "vi-VN" : locale === "pt" ? "pt-BR" : locale === "es" ? "es-ES" : "en-US";
  let entries: Awaited<ReturnType<typeof listLoginActivity>> = [];
  let game: Awaited<ReturnType<typeof getGameLoginActivity>> = null;
  let unavailable = false;

  try {
    [entries, game] = await Promise.all([
      listLoginActivity(session.user.name),
      getGameLoginActivity(session.user.name),
    ]);
  } catch {
    unavailable = true;
  }

  const website = entries[0] ?? null;
  const summaryIp = publicIp(game?.ipAddress) || publicIp(website?.ipAddress);
  const formatTime = (value: Date | null | undefined) =>
    value ? new Date(value).toLocaleString(dateLocale) : t.panel.noAuthentication;

  return (
    <div className="space-y-6">
      <div>
        <h1 className="section-title">{t.panel.loginHistoryTitle}</h1>
        <p className="muted mt-1">{t.panel.loginHistoryHint}</p>
      </div>

      <div className="overflow-hidden rounded-sm border border-white/5 bg-[#07021c] shadow-[0_12px_36px_rgba(0,0,0,0.28)]">
        {unavailable ? (
          <p className="p-5 text-sm text-red-300">{t.panel.loginHistoryUnavailable}</p>
        ) : (
          <div className="overflow-x-auto">
            <table className="w-full min-w-[660px] text-left text-sm">
              <thead>
                <tr className="border-b border-[#544d78] bg-[#24203f] text-white">
                  <th className="px-4 py-2.5 font-bold uppercase">{t.panel.ipAddress}</th>
                  <th className="px-4 py-2.5 font-bold">
                    <span className="block uppercase">{t.panel.game}</span>
                    <span className="mt-1 block text-xs font-semibold normal-case text-white/95">
                      {t.panel.lastAuthentication}
                    </span>
                  </th>
                  <th className="px-4 py-2.5 font-bold">
                    <span className="block uppercase">{t.panel.website}</span>
                    <span className="mt-1 block text-xs font-semibold normal-case text-white/95">
                      {t.panel.lastAuthentication}
                    </span>
                  </th>
                </tr>
              </thead>
              <tbody>
                <tr className="align-top text-white">
                  <td className="px-4 py-3 font-mono font-semibold">
                    {summaryIp ? (
                      <div className="flex flex-wrap items-center gap-x-2 gap-y-1">
                        <span>{summaryIp}</span>
                        <a
                          href={checkHostUrl(summaryIp)}
                          target="_blank"
                          rel="noopener noreferrer"
                          className="font-sans text-xs font-semibold text-mu-lime underline decoration-mu-lime/50 underline-offset-4 transition hover:text-white"
                        >
                          {t.panel.checkIpLocation}
                        </a>
                      </div>
                    ) : (
                      <span className="font-sans text-mu-muted">{t.panel.noPublicIp}</span>
                    )}
                  </td>
                  <td className="px-4 py-3">
                    <p className="font-mono font-semibold">{formatTime(game?.connectedAt)}</p>
                    <p className="mt-1 text-xs font-semibold text-white/95">
                      {game?.serverName || t.panel.noGameAuthentication}
                    </p>
                  </td>
                  <td className="px-4 py-3">
                    <p className="font-mono font-semibold">{formatTime(website?.loggedInAt)}</p>
                    <p className="mt-1 text-xs font-semibold text-white/95">
                      {website?.device || t.panel.noWebsiteAuthentication}
                    </p>
                  </td>
                </tr>
              </tbody>
            </table>
          </div>
        )}
      </div>

      {!unavailable && (
        <div className="card p-4 md:p-6">
          <h2 className="panel-title mb-4">{t.panel.websiteHistoryTitle}</h2>
          <div className="table-wrap">
            <table className="data-table">
              <thead>
                <tr>
                  <th>{t.panel.device}</th>
                  <th>{t.panel.ipAddress}</th>
                  <th>{t.panel.loginTime}</th>
                </tr>
              </thead>
              <tbody>
                {entries.length === 0 ? (
                  <tr>
                    <td colSpan={3} className="py-6 text-center text-mu-muted">
                      {t.panel.noLoginHistory}
                    </td>
                  </tr>
                ) : (
                  entries.map((entry) => {
                    const ip = publicIp(entry.ipAddress);
                    return (
                      <tr key={entry.id}>
                        <td className="font-medium text-white">{entry.device}</td>
                        <td className="font-mono text-mu-lime">
                          {ip ? (
                            <a
                              href={checkHostUrl(ip)}
                              target="_blank"
                              rel="noopener noreferrer"
                              className="underline decoration-mu-lime/50 underline-offset-4 hover:text-white"
                            >
                              {ip}
                            </a>
                          ) : (
                            <span className="text-mu-muted">{t.panel.noPublicIp}</span>
                          )}
                        </td>
                        <td>{new Date(entry.loggedInAt).toLocaleString(dateLocale)}</td>
                      </tr>
                    );
                  })
                )}
              </tbody>
            </table>
          </div>
        </div>
      )}
    </div>
  );
}
