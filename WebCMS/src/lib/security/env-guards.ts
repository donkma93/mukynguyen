import {
  allowDevCaptchaFallback,
  getTurnstileSecretKey,
  getTurnstileSiteKey,
  isProductionRuntime,
  isTurnstileConfigured,
} from "@/lib/security/turnstile";

export type EnvGuardIssue = {
  level: "error" | "warn";
  code: string;
  message: string;
};

/** Collect production / public-hardening issues. Pure — no I/O. */
export function collectEnvGuardIssues(
  env: NodeJS.ProcessEnv = process.env
): EnvGuardIssue[] {
  const issues: EnvGuardIssue[] = [];
  const isProd = (env.NODE_ENV || "") === "production";
  const secret = (env.NEXTAUTH_SECRET || "").trim();
  const bootstrapUser = (env.ADMIN_BOOTSTRAP_USER || "admin").trim().toLowerCase();
  const bootstrapPass = env.ADMIN_BOOTSTRAP_PASS || "admin123";
  const turnstileSite =
    env.TURNSTILE_SITE_KEY?.trim() || env.NEXT_PUBLIC_TURNSTILE_SITE_KEY?.trim() || "";
  const turnstileSecret = env.TURNSTILE_SECRET_KEY?.trim() || "";
  const allowDev = (env.ALLOW_DEV_CAPTCHA || "").trim().toLowerCase();
  const ops = (env.GS_OPS_ENABLED || "true").trim().toLowerCase();

  if (!secret || secret.length < 32) {
    issues.push({
      level: isProd ? "error" : "warn",
      code: "NEXTAUTH_SECRET",
      message:
        "NEXTAUTH_SECRET phải là chuỗi ngẫu nhiên ≥ 32 ký tự (bắt buộc khi production).",
    });
  }

  if (bootstrapPass === "admin123" || (bootstrapUser === "admin" && bootstrapPass === "admin123")) {
    issues.push({
      level: isProd ? "error" : "warn",
      code: "DEFAULT_ADMIN",
      message:
        "ADMIN_BOOTSTRAP_PASS đang dùng mặc định admin123 — đổi trước khi public.",
    });
  }

  if (isProd && (!turnstileSite || !turnstileSecret)) {
    issues.push({
      level: "error",
      code: "TURNSTILE_REQUIRED",
      message:
        "Production bắt buộc TURNSTILE_SITE_KEY và TURNSTILE_SECRET_KEY để chặn bot đăng ký.",
    });
  }

  if (isProd && (allowDev === "1" || allowDev === "true" || allowDev === "yes")) {
    issues.push({
      level: "error",
      code: "DEV_CAPTCHA_IN_PROD",
      message: "ALLOW_DEV_CAPTCHA không được bật khi production.",
    });
  }

  if (isProd && (ops === "true" || ops === "1" || ops === "yes")) {
    issues.push({
      level: "warn",
      code: "GS_OPS_PUBLIC",
      message:
        "GS_OPS_ENABLED đang bật — nên tắt trên VPS public hoặc chỉ cho IP/VPN admin.",
    });
  }

  return issues;
}

export function assertProductionSecurityOrThrow() {
  if (!isProductionRuntime()) return;
  const errors = collectEnvGuardIssues().filter((i) => i.level === "error");
  if (errors.length === 0) return;
  const msg = errors.map((e) => `[${e.code}] ${e.message}`).join("\n");
  throw new Error(`Security env guards failed:\n${msg}`);
}

export function logEnvGuardWarnings() {
  const issues = collectEnvGuardIssues();
  for (const i of issues) {
    const line = `[security:${i.level}] ${i.code}: ${i.message}`;
    if (i.level === "error") console.error(line);
    else console.warn(line);
  }
  return issues;
}

/** Helper for register route: which anti-bot mode is active. */
export function getRegisterBotMode(): "turnstile" | "dev-captcha" | "blocked" {
  if (isTurnstileConfigured()) return "turnstile";
  if (allowDevCaptchaFallback()) return "dev-captcha";
  return "blocked";
}

export function getPublicTurnstileSiteKey(): string | null {
  return getTurnstileSiteKey();
}
