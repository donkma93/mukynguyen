import type { NextConfig } from "next";

const nextConfig: NextConfig = {
  serverExternalPackages: ["msnodesqlv8", "mssql"],
  allowedDevOrigins: ["127.0.0.2"],
  async headers() {
    const isProd = process.env.NODE_ENV === "production";
    const base: { key: string; value: string }[] = [
      { key: "X-Content-Type-Options", value: "nosniff" },
      { key: "X-Frame-Options", value: "DENY" },
      { key: "Referrer-Policy", value: "strict-origin-when-cross-origin" },
      {
        key: "Permissions-Policy",
        value: "camera=(), microphone=(), geolocation=()",
      },
    ];
    if (isProd) {
      base.push({
        key: "Strict-Transport-Security",
        value: "max-age=31536000; includeSubDomains",
      });
    }
    return [
      {
        source: "/:path*",
        headers: base,
      },
    ];
  },
};

export default nextConfig;
