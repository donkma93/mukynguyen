import { NextResponse } from "next/server";
import {
  defaultLocale,
  isLocale,
  localeCookieName,
} from "@/lib/i18n/config";

export const dynamic = "force-dynamic";

export async function POST(req: Request) {
  try {
    const body = await req.json().catch(() => ({}));
    const locale = isLocale(body.locale) ? body.locale : defaultLocale;
    const res = NextResponse.json({ ok: true, locale });
    res.cookies.set(localeCookieName, locale, {
      path: "/",
      maxAge: 60 * 60 * 24 * 365,
      sameSite: "lax",
    });
    return res;
  } catch {
    return NextResponse.json(
      { ok: false, error: "Could not set locale" },
      { status: 500 }
    );
  }
}
