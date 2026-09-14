export type RateLimitResult =
  | { ok: true; remaining: number; resetMs: number }
  | { ok: false; retryAfterSec: number; reason: string };

type Bucket = {
  /** Timestamps of events in the current window (ms). */
  hits: number[];
  /** Soft lock until (ms), set when limit exceeded. */
  lockedUntil: number;
};

const store = new Map<string, Bucket>();

export type RateLimitOptions = {
  /** Max events allowed inside the window before lock/deny. */
  limit: number;
  /** Window length in ms for counting hits. */
  windowMs: number;
  /** Extra lockout after exceeding limit (ms). 0 = just reject until window slides. */
  lockoutMs?: number;
  /** Message returned when blocked. */
  message?: string;
  /** Clock override for tests. */
  now?: () => number;
};

/** Test helper — clear all buckets. */
export function resetRateLimitStore() {
  store.clear();
}

function getBucket(key: string): Bucket {
  let b = store.get(key);
  if (!b) {
    b = { hits: [], lockedUntil: 0 };
    store.set(key, b);
  }
  return b;
}

/**
 * Fixed/sliding hybrid: count hits in the last `windowMs`.
 * On exceed, optionally apply `lockoutMs` and reject with retryAfter.
 */
export function consumeRateLimit(
  key: string,
  opts: RateLimitOptions
): RateLimitResult {
  const now = (opts.now ?? Date.now)();
  const lockoutMs = opts.lockoutMs ?? 0;
  const message =
    opts.message || "Quá nhiều yêu cầu. Vui lòng thử lại sau.";
  const b = getBucket(key);

  if (b.lockedUntil > now) {
    return {
      ok: false,
      retryAfterSec: Math.max(1, Math.ceil((b.lockedUntil - now) / 1000)),
      reason: message,
    };
  }

  // Drop hits outside window
  const cutoff = now - opts.windowMs;
  b.hits = b.hits.filter((t) => t > cutoff);

  if (b.hits.length >= opts.limit) {
    if (lockoutMs > 0) {
      b.lockedUntil = now + lockoutMs;
    }
    const retryFrom =
      lockoutMs > 0
        ? b.lockedUntil
        : (b.hits[0] ?? now) + opts.windowMs;
    return {
      ok: false,
      retryAfterSec: Math.max(1, Math.ceil((retryFrom - now) / 1000)),
      reason: message,
    };
  }

  b.hits.push(now);
  return {
    ok: true,
    remaining: Math.max(0, opts.limit - b.hits.length),
    resetMs: (b.hits[0] ?? now) + opts.windowMs,
  };
}

/** Peek whether a key is currently blocked, without recording a hit. */
export function isRateLimited(
  key: string,
  opts: Pick<RateLimitOptions, "limit" | "windowMs" | "message" | "now">
): RateLimitResult {
  const now = (opts.now ?? Date.now)();
  const message = opts.message || "Quá nhiều yêu cầu. Vui lòng thử lại sau.";
  const b = store.get(key);
  if (!b) {
    return { ok: true, remaining: opts.limit, resetMs: now + opts.windowMs };
  }
  if (b.lockedUntil > now) {
    return {
      ok: false,
      retryAfterSec: Math.max(1, Math.ceil((b.lockedUntil - now) / 1000)),
      reason: message,
    };
  }
  const cutoff = now - opts.windowMs;
  const hits = b.hits.filter((t) => t > cutoff);
  if (hits.length >= opts.limit) {
    const retryFrom = (hits[0] ?? now) + opts.windowMs;
    return {
      ok: false,
      retryAfterSec: Math.max(1, Math.ceil((retryFrom - now) / 1000)),
      reason: message,
    };
  }
  return {
    ok: true,
    remaining: Math.max(0, opts.limit - hits.length),
    resetMs: (hits[0] ?? now) + opts.windowMs,
  };
}

/** Record a failed auth attempt; returns whether the identity is now locked. */
export function recordAuthFailure(
  key: string,
  opts: RateLimitOptions
): RateLimitResult {
  return consumeRateLimit(key, opts);
}

export function clearRateLimitKey(key: string) {
  store.delete(key);
}

/** Presets used by routes. */
export const RATE_LIMITS = {
  register: {
    limit: 5,
    windowMs: 15 * 60 * 1000,
    lockoutMs: 30 * 60 * 1000,
    message: "Quá nhiều lần đăng ký từ IP này. Thử lại sau 30 phút.",
  },
  loginFail: {
    limit: 10,
    windowMs: 15 * 60 * 1000,
    lockoutMs: 15 * 60 * 1000,
    message: "Đăng nhập sai quá nhiều lần. Tài khoản/IP bị khóa tạm thời.",
  },
  vipPurchase: {
    limit: 10,
    windowMs: 60 * 1000,
    lockoutMs: 0,
    message: "Quá nhiều lần mua VIP. Thử lại sau ít phút.",
  },
  giftcode: {
    limit: 10,
    windowMs: 60 * 1000,
    lockoutMs: 0,
    message: "Quá nhiều lần nhập giftcode. Thử lại sau ít phút.",
  },
  partnerClaim: {
    limit: 10,
    windowMs: 60 * 1000,
    lockoutMs: 0,
    message: "Quá nhiều lần nhận quà đối tác. Thử lại sau ít phút.",
  },
  partnerHighlight: {
    limit: 20,
    windowMs: 60 * 1000,
    lockoutMs: 0,
    message: "Quá nhiều lần phát Highlight. Thử lại sau ít phút.",
  },
  captcha: {
    limit: 20,
    windowMs: 60 * 1000,
    lockoutMs: 0,
    message: "Quá nhiều lần tải captcha. Thử lại sau ít phút.",
  },
} as const satisfies Record<string, RateLimitOptions>;
