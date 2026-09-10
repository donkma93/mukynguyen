export async function register() {
  if (process.env.NEXT_RUNTIME === "edge") return;
  const { logEnvGuardWarnings, assertProductionSecurityOrThrow } = await import(
    "@/lib/security/env-guards"
  );
  logEnvGuardWarnings();
  // Opt-in hard fail for public deploy: SECURITY_FAIL_CLOSED=1
  if ((process.env.SECURITY_FAIL_CLOSED || "").trim() === "1") {
    assertProductionSecurityOrThrow();
  }
}
