import { createHmac, randomBytes, timingSafeEqual } from "crypto";

const COOKIE = "mu_captcha";
const TTL_MS = 10 * 60 * 1000;

function secret() {
  return process.env.NEXTAUTH_SECRET || "thangcuoi-captcha-dev-secret";
}

function sign(payload: string) {
  return createHmac("sha256", secret()).update(payload).digest("hex");
}

export type CaptchaChallenge = {
  id: string;
  question: string;
  answer: string;
  svg: string;
};

export function createCaptchaChallenge(): CaptchaChallenge {
  const a = 3 + Math.floor(Math.random() * 9); // 3..11
  const b = 1 + Math.floor(Math.random() * 9); // 1..9
  const op = Math.random() > 0.5 ? "+" : "-";
  const left = op === "-" && a < b ? b : a;
  const right = op === "-" && a < b ? a : b;
  const answer = String(op === "+" ? left + right : left - right);
  const question = `${left} ${op} ${right} = ?`;
  const id = randomBytes(8).toString("hex");

  const svg = renderCaptchaSvg(question);
  return { id, question, answer, svg };
}

export function encodeCaptchaCookie(answer: string, id: string) {
  const exp = Date.now() + TTL_MS;
  const payload = `${id}.${exp}.${answer}`;
  const sig = sign(payload);
  return `${payload}.${sig}`;
}

export function verifyCaptchaCookie(cookieValue: string | undefined, userAnswer: string) {
  if (!cookieValue || !userAnswer?.trim()) {
    return { ok: false as const, reason: "Vui lòng nhập mã captcha" };
  }
  const parts = cookieValue.split(".");
  if (parts.length !== 4) {
    return { ok: false as const, reason: "Captcha không hợp lệ, hãy làm mới" };
  }
  const [id, expStr, answer, sig] = parts;
  const payload = `${id}.${expStr}.${answer}`;
  const expect = sign(payload);
  try {
    const a = Buffer.from(sig);
    const b = Buffer.from(expect);
    if (a.length !== b.length || !timingSafeEqual(a, b)) {
      return { ok: false as const, reason: "Captcha không hợp lệ, hãy làm mới" };
    }
  } catch {
    return { ok: false as const, reason: "Captcha không hợp lệ, hãy làm mới" };
  }
  const exp = Number(expStr);
  if (!Number.isFinite(exp) || Date.now() > exp) {
    return { ok: false as const, reason: "Captcha đã hết hạn, hãy làm mới" };
  }
  if (String(userAnswer).trim() !== answer) {
    return { ok: false as const, reason: "Mã captcha không đúng" };
  }
  return { ok: true as const };
}

export const CAPTCHA_COOKIE = COOKIE;

function escapeXml(s: string) {
  return s
    .replace(/&/g, "&amp;")
    .replace(/</g, "&lt;")
    .replace(/>/g, "&gt;")
    .replace(/"/g, "&quot;");
}

function renderCaptchaSvg(text: string) {
  const noise = Array.from({ length: 8 }, (_, i) => {
    const x1 = Math.floor(Math.random() * 180);
    const y1 = Math.floor(Math.random() * 56);
    const x2 = Math.floor(Math.random() * 180);
    const y2 = Math.floor(Math.random() * 56);
    const c = i % 2 === 0 ? "#6b21a8" : "#d4af37";
    return `<line x1="${x1}" y1="${y1}" x2="${x2}" y2="${y2}" stroke="${c}" stroke-opacity="0.35" stroke-width="1"/>`;
  }).join("");

  const dots = Array.from({ length: 18 }, () => {
    const cx = Math.floor(Math.random() * 180);
    const cy = Math.floor(Math.random() * 56);
    return `<circle cx="${cx}" cy="${cy}" r="1.2" fill="#78ff14" fill-opacity="0.45"/>`;
  }).join("");

  const rotate = (Math.random() * 8 - 4).toFixed(1);
  const label = escapeXml(text);

  return `<?xml version="1.0" encoding="UTF-8"?>
<svg xmlns="http://www.w3.org/2000/svg" width="180" height="56" viewBox="0 0 180 56">
  <defs>
    <linearGradient id="g" x1="0" y1="0" x2="1" y2="1">
      <stop offset="0%" stop-color="#1b1924"/>
      <stop offset="100%" stop-color="#0f0e14"/>
    </linearGradient>
  </defs>
  <rect width="180" height="56" rx="8" fill="url(#g)" stroke="#d4af37" stroke-opacity="0.45"/>
  ${noise}
  ${dots}
  <g transform="rotate(${rotate} 90 28)">
    <text x="90" y="34" text-anchor="middle" font-family="Georgia, serif" font-size="22" font-weight="700" fill="#f3f4f6" letter-spacing="1">${label}</text>
  </g>
</svg>`;
}
