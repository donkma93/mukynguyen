import Link from "next/link";
import { getPublicDictionary } from "@/lib/i18n/get-dictionary";
import { getLocale } from "@/lib/i18n/locale";
import { getPublicServerEndpoint } from "@/lib/public-server";

export default async function SiteFooter() {
  const locale = await getLocale();
  const t = getPublicDictionary(locale);
  const endpoint = getPublicServerEndpoint();

  const exploreLinks = [
    { href: "/guide", label: t.nav.guideHub },
    { href: "/guide/mix", label: t.nav.guideMix },
    { href: "/guide/farm", label: t.nav.guideFarm },
    { href: "/events", label: t.nav.events },
    { href: "/classes", label: t.nav.classes },
    { href: "/balancing", label: t.nav.balancing },
    { href: "/commands", label: t.nav.commands },
  ];

  return (
    <footer className="mt-auto border-t border-mu-gold/15 bg-black/30">
      <div className="mx-auto grid max-w-6xl gap-6 px-4 py-10 md:grid-cols-4">
        <div>
          <h3 className="font-display text-lg text-mu-gold">{t.brand.name}</h3>
          <p className="mt-2 text-sm text-mu-muted">{t.footer.blurb}</p>
        </div>
        <div>
          <h4 className="text-sm font-semibold uppercase tracking-wider text-gray-300">
            {t.footer.explore}
          </h4>
          <ul className="mt-2 space-y-1.5">
            {exploreLinks.map((link) => (
              <li key={link.href}>
                <Link
                  href={link.href}
                  className="text-sm text-mu-muted hover:text-mu-gold"
                >
                  {link.label}
                </Link>
              </li>
            ))}
          </ul>
        </div>
        <div>
          <h4 className="text-sm font-semibold uppercase tracking-wider text-gray-300">
            {t.footer.connect}
          </h4>
          <p className="mt-2 text-sm text-mu-lime">IP: {endpoint}</p>
          <Link
            href="/download"
            className="mt-2 inline-block text-sm text-mu-gold hover:underline"
          >
            {t.footer.downloadGuide}
          </Link>
        </div>
        <div>
          <h4 className="text-sm font-semibold uppercase tracking-wider text-gray-300">
            {t.footer.credit}
          </h4>
          <p className="mt-2 text-sm text-mu-muted">
            {t.footer.developedBy}{" "}
            <span className="text-mu-gold">DONPV</span> / {t.brand.name}
          </p>
        </div>
      </div>
      <div className="border-t border-white/5 py-4 text-center text-xs text-mu-muted">
        © {new Date().getFullYear()} {t.brand.name}. {t.footer.rights}
      </div>
    </footer>
  );
}
