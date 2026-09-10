import { cookies, headers } from "next/headers";
import {
  defaultLocale,
  isLocale,
  localeCookieName,
  type Locale,
} from "@/lib/i18n/config";

function fromAcceptLanguage(header: string | null): Locale | null {
  if (!header) return null;
  const parts = header.split(",").map((part) => {
    const [tag, ...params] = part.trim().split(";");
    const q = params
      .map((p) => p.trim())
      .find((p) => p.startsWith("q="));
    const quality = q ? Number(q.slice(2)) || 0 : 1;
    return { tag: tag.toLowerCase(), quality };
  });
  parts.sort((a, b) => b.quality - a.quality);
  for (const { tag } of parts) {
    const base = tag.split("-")[0];
    if (isLocale(base)) return base;
    if (tag.startsWith("pt")) return "pt";
    if (tag.startsWith("es")) return "es";
    if (tag.startsWith("en")) return "en";
    if (tag.startsWith("vi")) return "vi";
  }
  return null;
}

export async function getLocale(): Promise<Locale> {
  const jar = await cookies();
  const fromCookie = jar.get(localeCookieName)?.value;
  if (isLocale(fromCookie)) return fromCookie;

  const hdrs = await headers();
  const fromHeader = fromAcceptLanguage(hdrs.get("accept-language"));
  return fromHeader ?? defaultLocale;
}
