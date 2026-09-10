import { describe, expect, it } from "vitest";
import { collectEnvGuardIssues } from "@/lib/security/env-guards";

describe("env guards", () => {
  it("flags default admin and missing turnstile in production", () => {
    const issues = collectEnvGuardIssues({
      NODE_ENV: "production",
      NEXTAUTH_SECRET: "short",
      ADMIN_BOOTSTRAP_USER: "admin",
      ADMIN_BOOTSTRAP_PASS: "admin123",
      ALLOW_DEV_CAPTCHA: "1",
      GS_OPS_ENABLED: "true",
    } as NodeJS.ProcessEnv);

    const codes = issues.map((i) => i.code);
    expect(codes).toContain("NEXTAUTH_SECRET");
    expect(codes).toContain("DEFAULT_ADMIN");
    expect(codes).toContain("TURNSTILE_REQUIRED");
    expect(codes).toContain("DEV_CAPTCHA_IN_PROD");
    expect(codes).toContain("GS_OPS_PUBLIC");
    expect(issues.some((i) => i.level === "error")).toBe(true);
  });

  it("passes with strong production config", () => {
    const issues = collectEnvGuardIssues({
      NODE_ENV: "production",
      NEXTAUTH_SECRET: "a".repeat(40),
      ADMIN_BOOTSTRAP_USER: "rootops",
      ADMIN_BOOTSTRAP_PASS: "S3cure!Pass",
      TURNSTILE_SITE_KEY: "site",
      TURNSTILE_SECRET_KEY: "secret",
      ALLOW_DEV_CAPTCHA: "0",
      GS_OPS_ENABLED: "false",
    } as NodeJS.ProcessEnv);
    expect(issues.filter((i) => i.level === "error")).toHaveLength(0);
  });
});
