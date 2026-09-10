"use client";

import Link from "next/link";
import { usePathname } from "next/navigation";

type NavItem = { href: string; label: string };

type Props = {
  title: string;
  userName: string;
  items: NavItem[];
};

export default function AdminSidebar({ title, userName, items }: Props) {
  const pathname = usePathname();

  function isActive(href: string) {
    if (href === "/admin") return pathname === "/admin";
    return pathname === href || pathname.startsWith(`${href}/`);
  }

  return (
    <aside className="card h-fit border-mu-purple/40 p-4 shadow-purple">
      <p className="mb-3 text-xs uppercase tracking-wider text-purple-200">
        {title}
      </p>
      <p className="mb-4 truncate font-semibold text-mu-gold">{userName}</p>
      <nav className="flex flex-row gap-1 overflow-x-auto lg:flex-col">
        {items.map((item) => (
          <Link
            key={item.href}
            href={item.href}
            className={`${
              isActive(item.href) ? "nav-link-active" : "nav-link"
            } whitespace-nowrap`}
          >
            {item.label}
          </Link>
        ))}
      </nav>
    </aside>
  );
}
