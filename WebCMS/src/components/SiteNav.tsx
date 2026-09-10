"use client";

import Link from "next/link";
import { usePathname } from "next/navigation";
import { useEffect, useRef, useState } from "react";
import type { Dictionary } from "@/lib/i18n/get-dictionary";

type Props = {
  t: Dictionary;
  showPanel?: boolean;
  showAdmin?: boolean;
};

type NavItem = { href: string; label: string };

export default function SiteNav({ t, showPanel, showAdmin }: Props) {
  const pathname = usePathname();
  const [guideOpen, setGuideOpen] = useState(false);
  const [mobileOpen, setMobileOpen] = useState(false);
  const guideRef = useRef<HTMLDivElement>(null);

  const primary: NavItem[] = [
    { href: "/", label: t.nav.home },
    { href: "/ranking", label: t.nav.ranking },
    { href: "/download", label: t.nav.download },
  ];

  const guide: NavItem[] = [
    { href: "/guide", label: t.nav.guideHub },
    { href: "/guide/mix", label: t.nav.guideMix },
    { href: "/guide/farm", label: t.nav.guideFarm },
    { href: "/events", label: t.nav.events },
    { href: "/classes", label: t.nav.classes },
    { href: "/balancing", label: t.nav.balancing },
    { href: "/gameplay", label: t.nav.gameplay },
    { href: "/commands", label: t.nav.commands },
  ];

  const account: NavItem[] = [
    ...(showPanel ? [{ href: "/panel", label: t.nav.panel }] : []),
    ...(showAdmin ? [{ href: "/admin", label: t.nav.admin }] : []),
  ];

  const guideActive = guide.some(
    (item) => pathname === item.href || pathname.startsWith(`${item.href}/`),
  );

  useEffect(() => {
    setGuideOpen(false);
    setMobileOpen(false);
  }, [pathname]);

  useEffect(() => {
    if (!guideOpen) return;
    function onPointerDown(e: MouseEvent) {
      if (!guideRef.current?.contains(e.target as Node)) setGuideOpen(false);
    }
    function onKey(e: KeyboardEvent) {
      if (e.key === "Escape") setGuideOpen(false);
    }
    window.addEventListener("mousedown", onPointerDown);
    window.addEventListener("keydown", onKey);
    return () => {
      window.removeEventListener("mousedown", onPointerDown);
      window.removeEventListener("keydown", onKey);
    };
  }, [guideOpen]);

  useEffect(() => {
    if (!mobileOpen) return;
    function onKey(e: KeyboardEvent) {
      if (e.key === "Escape") setMobileOpen(false);
    }
    window.addEventListener("keydown", onKey);
    document.body.style.overflow = "hidden";
    return () => {
      window.removeEventListener("keydown", onKey);
      document.body.style.overflow = "";
    };
  }, [mobileOpen]);

  function isActive(href: string) {
    if (href === "/") return pathname === "/";
    return pathname === href || pathname.startsWith(`${href}/`);
  }

  return (
    <div className="flex items-center gap-1">
      {/* Desktop nav */}
      <nav className="hidden items-center gap-0.5 lg:flex">
        {primary.slice(0, 1).map((item) => (
          <Link
            key={item.href}
            href={item.href}
            className={isActive(item.href) ? "nav-link-active" : "nav-link"}
          >
            {item.label}
          </Link>
        ))}

        <div className="relative" ref={guideRef}>
          <button
            type="button"
            className={`${guideActive || guideOpen ? "nav-link-active" : "nav-link"} inline-flex items-center gap-1`}
            aria-expanded={guideOpen}
            aria-haspopup="menu"
            onClick={() => setGuideOpen((v) => !v)}
          >
            {t.nav.guide}
            <svg
              viewBox="0 0 12 12"
              className={`h-3 w-3 transition ${guideOpen ? "rotate-180 text-mu-gold" : "text-mu-muted"}`}
              aria-hidden
            >
              <path
                d="M2.5 4.5 6 8l3.5-3.5"
                fill="none"
                stroke="currentColor"
                strokeWidth="1.6"
                strokeLinecap="round"
                strokeLinejoin="round"
              />
            </svg>
          </button>
          {guideOpen ? (
            <div className="nav-dropdown" role="menu">
              {guide.map((item) => (
                <Link
                  key={item.href}
                  href={item.href}
                  role="menuitem"
                  className={
                    isActive(item.href)
                      ? "nav-dropdown-item-active"
                      : "nav-dropdown-item"
                  }
                  onClick={() => setGuideOpen(false)}
                >
                  {item.label}
                </Link>
              ))}
            </div>
          ) : null}
        </div>

        {primary.slice(1).map((item) => (
          <Link
            key={item.href}
            href={item.href}
            className={isActive(item.href) ? "nav-link-active" : "nav-link"}
          >
            {item.label}
          </Link>
        ))}

        {account.map((item) => (
          <Link
            key={item.href}
            href={item.href}
            className={`${isActive(item.href) ? "nav-link-active" : "nav-link"} ${
              item.href === "/admin" ? "text-purple-200" : ""
            }`}
          >
            {item.label}
          </Link>
        ))}
      </nav>

      {/* Mobile / tablet toggle */}
      <button
        type="button"
        className="btn-ghost inline-flex h-10 w-10 items-center justify-center px-0 lg:hidden"
        aria-label={mobileOpen ? t.nav.close : t.nav.menu}
        aria-expanded={mobileOpen}
        onClick={() => setMobileOpen((v) => !v)}
      >
        {mobileOpen ? (
          <svg viewBox="0 0 24 24" className="h-5 w-5" aria-hidden>
            <path
              d="M6 6l12 12M18 6 6 18"
              fill="none"
              stroke="currentColor"
              strokeWidth="1.8"
              strokeLinecap="round"
            />
          </svg>
        ) : (
          <svg viewBox="0 0 24 24" className="h-5 w-5" aria-hidden>
            <path
              d="M4 7h16M4 12h16M4 17h16"
              fill="none"
              stroke="currentColor"
              strokeWidth="1.8"
              strokeLinecap="round"
            />
          </svg>
        )}
      </button>

      {/* Mobile drawer */}
      {mobileOpen ? (
        <div className="fixed inset-0 top-[65px] z-40 lg:hidden">
          <button
            type="button"
            className="absolute inset-0 bg-black/60 backdrop-blur-[2px]"
            aria-label={t.nav.close}
            onClick={() => setMobileOpen(false)}
          />
          <div className="absolute inset-x-0 top-0 max-h-[calc(100vh-65px)] overflow-y-auto border-b border-mu-gold/20 bg-mu-panel/98 px-4 py-4 shadow-panel">
            <div className="mx-auto flex max-w-6xl flex-col gap-1">
              {primary.map((item) => (
                <Link
                  key={item.href}
                  href={item.href}
                  className={
                    isActive(item.href)
                      ? "nav-dropdown-item-active"
                      : "nav-dropdown-item"
                  }
                  onClick={() => setMobileOpen(false)}
                >
                  {item.label}
                </Link>
              ))}

              <div className="my-2 border-t border-white/10 pt-3">
                <p className="mb-1 px-3 text-[11px] font-semibold uppercase tracking-[0.22em] text-mu-muted">
                  {t.nav.guide}
                </p>
                {guide.map((item) => (
                  <Link
                    key={item.href}
                    href={item.href}
                    className={
                      isActive(item.href)
                        ? "nav-dropdown-item-active"
                        : "nav-dropdown-item"
                    }
                    onClick={() => setMobileOpen(false)}
                  >
                    {item.label}
                  </Link>
                ))}
              </div>

              {account.length ? (
                <div className="my-2 border-t border-white/10 pt-3">
                  {account.map((item) => (
                    <Link
                      key={item.href}
                      href={item.href}
                      className={`${
                        isActive(item.href)
                          ? "nav-dropdown-item-active"
                          : "nav-dropdown-item"
                      } ${item.href === "/admin" ? "text-purple-200" : ""}`}
                      onClick={() => setMobileOpen(false)}
                    >
                      {item.label}
                    </Link>
                  ))}
                </div>
              ) : null}
            </div>
          </div>
        </div>
      ) : null}
    </div>
  );
}
