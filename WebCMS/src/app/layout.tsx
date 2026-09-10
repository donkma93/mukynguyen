import type { Metadata } from "next";
import { Be_Vietnam_Pro, Source_Serif_4 } from "next/font/google";
import { I18nProvider } from "@/components/I18nProvider";
import Providers from "@/components/Providers";
import SiteFooter from "@/components/SiteFooter";
import SiteHeader from "@/components/SiteHeader";
import { getPublicDictionary } from "@/lib/i18n/get-dictionary";
import { getLocale } from "@/lib/i18n/locale";
import "./globals.css";

const display = Source_Serif_4({
  variable: "--font-display",
  subsets: ["latin", "vietnamese"],
  weight: ["500", "600", "700"],
});

const sans = Be_Vietnam_Pro({
  variable: "--font-sans",
  subsets: ["latin", "vietnamese"],
  weight: ["400", "500", "600", "700"],
});

export const metadata: Metadata = {
  title: {
    default: "MU Kỷ Nguyên",
    template: "%s | MU Kỷ Nguyên",
  },
  description:
    "WebCMS MU Kỷ Nguyên — bảng xếp hạng, giftcode và quản trị máy chủ.",
};

export default async function RootLayout({
  children,
}: Readonly<{ children: React.ReactNode }>) {
  const locale = await getLocale();
  const dictionary = getPublicDictionary(locale);

  return (
    <html lang={locale}>
      <body className={`${display.variable} ${sans.variable} font-sans`}>
        <Providers>
          <I18nProvider locale={locale} dictionary={dictionary}>
            <div className="flex min-h-screen flex-col">
              <SiteHeader />
              <main className="flex-1">{children}</main>
              <SiteFooter />
            </div>
          </I18nProvider>
        </Providers>
      </body>
    </html>
  );
}
