import Link from "next/link";
import { redirect } from "next/navigation";
import { getServerAuthSession } from "@/lib/auth";
import { getDictionary } from "@/lib/i18n/get-dictionary";
import { getLocale } from "@/lib/i18n/locale";
import { isActivePartner } from "@/lib/partner/partners";

export default async function PanelLayout({
  children,
}: {
  children: React.ReactNode;
}) {
  const session = await getServerAuthSession();
  if (!session?.user) redirect("/login");

  const locale = await getLocale();
  const t = getDictionary(locale);
  let showPartner = false;
  if (session.user.role === "user" && session.user.id) {
    try {
      showPartner = await isActivePartner(session.user.id);
    } catch {
      showPartner = false;
    }
  }
  const nav = [
    { href: "/panel", label: t.panel.overview },
    { href: "/panel/giftcode", label: t.panel.giftcode },
    ...(showPartner ? [{ href: "/panel/partner", label: t.panel.partner }] : []),
    { href: "/panel/password", label: t.panel.password },
  ];

  return (
    <div className="mx-auto grid max-w-6xl gap-6 px-4 py-8 lg:grid-cols-[220px_1fr]">
      <aside className="card h-fit p-4">
        <p className="mb-3 text-xs uppercase tracking-wider text-mu-muted">
          {t.panel.sidebarTitle}
        </p>
        <p className="mb-4 truncate font-semibold text-mu-gold">
          {session.user.name}
        </p>
        <nav className="flex flex-row gap-1 overflow-x-auto lg:flex-col">
          {nav.map((item) => (
            <Link
              key={item.href}
              href={item.href}
              className="nav-link whitespace-nowrap"
            >
              {item.label}
            </Link>
          ))}
        </nav>
      </aside>
      <div>{children}</div>
    </div>
  );
}
