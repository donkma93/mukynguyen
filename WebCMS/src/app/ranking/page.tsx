import Link from "next/link";
import RankTable from "@/components/RankTable";
import { getRanking } from "@/lib/game";
import { getDictionary } from "@/lib/i18n/get-dictionary";
import { getLocale } from "@/lib/i18n/locale";

export const dynamic = "force-dynamic";

const tabTypes = ["reset", "master", "level", "zen", "kill"] as const;
type RankType = (typeof tabTypes)[number];

type Props = {
  searchParams: Promise<{ type?: string }>;
};

function isRankType(v?: string): v is RankType {
  return tabTypes.some((t) => t === v);
}

export async function generateMetadata() {
  const locale = await getLocale();
  const t = getDictionary(locale);
  return { title: t.ranking.title };
}

export default async function RankingPage({ searchParams }: Props) {
  const locale = await getLocale();
  const t = getDictionary(locale);
  const sp = await searchParams;
  const type: RankType = isRankType(sp.type) ? sp.type : "reset";

  const tabs = tabTypes.map((tabType) => ({
    type: tabType,
    label: t.ranking.tabs[tabType],
  }));

  let rows: Awaited<ReturnType<typeof getRanking>> = [];
  try {
    rows = await getRanking(type, 50);
  } catch {
    rows = [];
  }

  return (
    <div className="mx-auto max-w-6xl px-4 py-10">
      <h1 className="section-title">{t.ranking.title}</h1>
      <p className="muted mt-2 mb-6">{t.ranking.subtitle}</p>

      <div className="mb-6 flex flex-wrap gap-2">
        {tabs.map((tab) => (
          <Link
            key={tab.type}
            href={`/ranking?type=${tab.type}`}
            className={tab.type === type ? "nav-link-active" : "nav-link"}
          >
            {tab.label}
          </Link>
        ))}
      </div>

      <div className="card p-4 md:p-6">
        <RankTable
          rows={rows}
          type={type}
          labels={t.ranking}
          numberLocale={
            locale === "vi"
              ? "vi-VN"
              : locale === "pt"
                ? "pt-BR"
                : locale === "es"
                  ? "es-ES"
                  : "en-US"
          }
        />
      </div>
    </div>
  );
}
