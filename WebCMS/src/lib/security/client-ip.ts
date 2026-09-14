import { isIP } from "node:net";

type HeaderSource = Headers | Record<string, unknown>;

function headerValue(headers: HeaderSource | undefined, name: string): string | null {
  if (!headers) return null;
  if (headers instanceof Headers) return headers.get(name);
  const value = headers[name] ?? headers[name.toLowerCase()];
  return typeof value === "string" ? value : Array.isArray(value) ? value[0] ?? null : null;
}

function isPublicIpv4(ip: string): boolean {
  const parts = ip.split(".").map(Number);
  if (parts.length !== 4 || parts.some((part) => !Number.isInteger(part) || part < 0 || part > 255)) {
    return false;
  }
  const [a, b] = parts;
  return !(
    a === 0 ||
    a === 10 ||
    a === 127 ||
    (a === 100 && b >= 64 && b <= 127) ||
    (a === 169 && b === 254) ||
    (a === 172 && b >= 16 && b <= 31) ||
    (a === 192 && b === 168) ||
    (a === 198 && (b === 18 || b === 19)) ||
    a >= 224
  );
}

/** Returns a valid public IPv4/IPv6 address, rejecting local and reserved network addresses. */
export function publicIp(value?: string | null): string | null {
  const ip = (value || "").trim().replace(/^\[|\]$/g, "").replace(/^::ffff:/i, "");
  const family = isIP(ip);
  if (family === 4) return isPublicIpv4(ip) ? ip : null;
  if (family !== 6) return null;

  const lower = ip.toLowerCase();
  if (
    lower === "::" ||
    lower === "::1" ||
    lower.startsWith("fc") ||
    lower.startsWith("fd") ||
    lower.startsWith("fe80:") ||
    lower.startsWith("ff")
  ) {
    return null;
  }
  return ip;
}

/** Public IP from trusted proxy/CDN headers. Local addresses are never recorded. */
export function getClientIp(
  req?: { headers?: HeaderSource } | null
): string {
  const directHeaders = [
    headerValue(req?.headers, "cf-connecting-ip"),
    headerValue(req?.headers, "x-real-ip"),
    headerValue(req?.headers, "x-vercel-forwarded-for"),
  ];
  for (const value of directHeaders) {
    const ip = publicIp(value);
    if (ip) return ip;
  }

  const forwarded = headerValue(req?.headers, "x-forwarded-for");
  for (const value of forwarded?.split(",") || []) {
    const ip = publicIp(value);
    if (ip) return ip;
  }
  return "unknown";
}

export function getUserAgent(req?: { headers?: HeaderSource } | null): string {
  return (headerValue(req?.headers, "user-agent") || "unknown").slice(0, 512);
}
