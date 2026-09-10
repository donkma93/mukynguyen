import Link from "next/link";
import { getDictionary } from "@/lib/i18n/get-dictionary";
import { getLocale } from "@/lib/i18n/locale";
import { getPublicServerEndpoint } from "@/lib/public-server";

export async function generateMetadata() {
  const locale = await getLocale();
  const t = getDictionary(locale);
  return { title: t.nav.download };
}

export default async function DownloadPage() {
  const locale = await getLocale();
  const t = getDictionary(locale);
  const endpoint = getPublicServerEndpoint();

  return (
    <div className="mx-auto max-w-4xl px-4 py-10">
      <h1 className="section-title">{t.download.title}</h1>
      <p className="muted mt-2 mb-8">{t.download.subtitle}</p>

      <div className="card-glow mb-8 p-6">
        <p className="text-sm uppercase tracking-wider text-mu-muted">
          {t.download.serverAddress}
        </p>
        <p className="mt-2 font-display text-3xl font-bold text-mu-lime">
          {endpoint}
        </p>
        <div className="mt-6 flex flex-wrap gap-3">
          <a
            className="btn-gold"
            href="https://drive.google.com"
            target="_blank"
            rel="noopener noreferrer"
          >
            {t.download.downloadDrive}
          </a>
          <Link href="/register" className="btn-lime">
            {t.download.registerAccount}
          </Link>
        </div>
      </div>

      <ol className="grid gap-4">
        {t.download.steps.map((step, idx) => (
          <li key={step.title} className="card flex gap-4 p-5">
            <div className="flex h-10 w-10 shrink-0 items-center justify-center rounded-full border border-mu-gold/40 bg-mu-gold/10 font-display text-mu-gold">
              {idx + 1}
            </div>
            <div>
              <h2 className="panel-title">{step.title}</h2>
              <p className="mt-1 text-sm text-gray-300">
                {step.body.replaceAll("{server}", endpoint)}
              </p>
            </div>
          </li>
        ))}
      </ol>
    </div>
  );
}
