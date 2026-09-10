"use client";

import { signOut } from "next-auth/react";
import { useI18n } from "@/components/I18nProvider";

export default function LogoutButton() {
  const { t } = useI18n();
  return (
    <button
      type="button"
      className="btn-ghost px-3 py-2 text-xs sm:text-sm"
      onClick={() => signOut({ callbackUrl: "/" })}
    >
      {t.nav.logout}
    </button>
  );
}
