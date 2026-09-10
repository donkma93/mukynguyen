"use client";

import { useRouter } from "next/navigation";
import { useEffect, useRef, useState, useTransition } from "react";
import {
  locales,
  localeLabels,
  localeShort,
  type Locale,
} from "@/lib/i18n/config";
import { useI18n } from "@/components/I18nProvider";

export default function LanguageSwitcher() {
  const router = useRouter();
  const { locale, t } = useI18n();
  const [open, setOpen] = useState(false);
  const [pending, startTransition] = useTransition();
  const rootRef = useRef<HTMLDivElement>(null);

  useEffect(() => {
    if (!open) return;
    function onPointerDown(e: MouseEvent) {
      if (!rootRef.current?.contains(e.target as Node)) setOpen(false);
    }
    window.addEventListener("mousedown", onPointerDown);
    return () => window.removeEventListener("mousedown", onPointerDown);
  }, [open]);

  async function choose(next: Locale) {
    if (next === locale) {
      setOpen(false);
      return;
    }
    setOpen(false);
    try {
      await fetch("/api/locale", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ locale: next }),
      });
      startTransition(() => {
        router.refresh();
      });
    } catch {
      // ignore network errors; user can retry
    }
  }

  return (
    <div className="relative" ref={rootRef}>
      <button
        type="button"
        className="btn-ghost px-2.5 py-2 text-xs sm:text-sm"
        aria-label={t.language.label}
        aria-expanded={open}
        disabled={pending}
        onClick={() => setOpen((v) => !v)}
      >
        <span className="font-semibold text-mu-gold">{localeShort[locale]}</span>
        <span className="ml-1 hidden text-mu-muted sm:inline">▾</span>
      </button>
      {open ? (
        <div className="absolute right-0 z-50 mt-2 min-w-[160px] overflow-hidden rounded-lg border border-mu-gold/30 bg-mu-panel shadow-panel">
          {locales.map((code) => (
            <button
              key={code}
              type="button"
              className={`flex w-full items-center justify-between px-3 py-2 text-left text-sm transition hover:bg-white/5 ${
                code === locale ? "bg-mu-gold/10 text-mu-gold" : "text-gray-200"
              }`}
              onClick={() => void choose(code)}
            >
              <span>{localeLabels[code]}</span>
              <span className="text-xs text-mu-muted">{localeShort[code]}</span>
            </button>
          ))}
        </div>
      ) : null}
    </div>
  );
}
