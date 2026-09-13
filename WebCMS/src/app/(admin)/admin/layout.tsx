import { redirect } from "next/navigation";
import AdminSidebar from "@/components/admin/AdminSidebar";
import { getServerAuthSession } from "@/lib/auth";
import { getDictionary } from "@/lib/i18n/get-dictionary";
import { getLocale } from "@/lib/i18n/locale";

export default async function AdminLayout({
  children,
}: {
  children: React.ReactNode;
}) {
  const session = await getServerAuthSession();
  if (!session?.user) redirect("/login?admin=1");
  if (session.user.role !== "admin") redirect("/");

  const locale = await getLocale();
  const t = getDictionary(locale);
  const nav = [
    { href: "/admin", label: t.admin.dashboard },
    { href: "/admin/accounts", label: t.admin.accounts },
    { href: "/admin/characters", label: t.admin.characters },
    { href: "/admin/hack", label: t.admin.hack },
    { href: "/admin/ops", label: t.admin.ops },
    { href: "/admin/events", label: "Sự kiện Game" },
    { href: "/admin/gs/ini", label: t.admin.gsIni },
    { href: "/admin/gs/groups", label: t.admin.gsGroups },
    { href: "/admin/gs/balancing", label: t.admin.balancing },
    { href: "/admin/enhancement-rates", label: "Tỉ lệ đập đồ" },
    { href: "/admin/giftcodes", label: t.admin.giftcodes },
  ];

  return (
    <div className="mx-auto grid max-w-6xl gap-6 px-4 py-8 lg:grid-cols-[220px_1fr]">
      <AdminSidebar
        title={t.admin.sidebarTitle}
        userName={session.user.name || "Admin"}
        items={nav}
      />
      <div>{children}</div>
    </div>
  );
}
