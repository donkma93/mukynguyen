import type { Config } from "tailwindcss";

export default {
  content: [
    "./src/pages/**/*.{js,ts,jsx,tsx,mdx}",
    "./src/components/**/*.{js,ts,jsx,tsx,mdx}",
    "./src/app/**/*.{js,ts,jsx,tsx,mdx}",
  ],
  theme: {
    extend: {
      colors: {
        background: "var(--mu-bg)",
        foreground: "#f3f4f6",
        mu: {
          bg: "#0f0e14",
          panel: "#1b1924",
          gold: "#d4af37",
          lime: "#78ff14",
          danger: "#b02323",
          purple: "#6b21a8",
          muted: "#9ca3af",
        },
      },
      fontFamily: {
        display: [
          "var(--font-display)",
          "Source Serif 4",
          "Times New Roman",
          "serif",
        ],
        sans: [
          "var(--font-sans)",
          "Be Vietnam Pro",
          "system-ui",
          "Segoe UI",
          "sans-serif",
        ],
      },
      boxShadow: {
        gold: "0 0 24px rgba(212, 175, 55, 0.35)",
        purple: "0 0 28px rgba(107, 33, 168, 0.4)",
        panel: "0 10px 40px rgba(0, 0, 0, 0.45)",
      },
      backgroundImage: {
        "hero-veil":
          "linear-gradient(180deg, rgba(15,14,20,0.55) 0%, rgba(15,14,20,0.85) 70%, #0f0e14 100%)",
      },
    },
  },
  plugins: [],
} satisfies Config;
