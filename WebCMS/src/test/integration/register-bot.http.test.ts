import { describe, expect, it } from "vitest";

const enabled = process.env.RUN_INTEGRATION === "1";
const base = (process.env.BASE_URL || "http://localhost:5000").replace(/\/$/, "");

describe.skipIf(!enabled)("HTTP integration: register anti-bot", () => {
  it("bot-mode endpoint returns a known mode", async () => {
    const res = await fetch(`${base}/api/bot-mode`);
    expect(res.status).toBe(200);
    const data = await res.json();
    expect(["turnstile", "dev-captcha", "blocked"]).toContain(data.mode);
  });

  it("register without captcha/turnstile is rejected", async () => {
    const res = await fetch(`${base}/api/register`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({
        account: "intbot01",
        password: "pass12",
        confirmPassword: "pass12",
      }),
    });
    expect([400, 429, 503]).toContain(res.status);
    const data = await res.json().catch(() => ({}));
    expect(data.ok).not.toBe(true);
  });
});
