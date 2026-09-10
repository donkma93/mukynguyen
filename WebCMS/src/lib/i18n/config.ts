export const locales = ["vi", "en", "pt", "es"] as const;

export type Locale = (typeof locales)[number];

export const defaultLocale: Locale = "vi";

export const localeCookieName = "mu_locale";

export const localeLabels: Record<Locale, string> = {
  vi: "Tiếng Việt",
  en: "English",
  pt: "Português",
  es: "Español",
};

export const localeShort: Record<Locale, string> = {
  vi: "VI",
  en: "EN",
  pt: "PT",
  es: "ES",
};

export function isLocale(value: string | null | undefined): value is Locale {
  return !!value && (locales as readonly string[]).includes(value);
}
