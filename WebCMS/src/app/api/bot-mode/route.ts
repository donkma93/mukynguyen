import { NextResponse } from "next/server";
import {
  getPublicTurnstileSiteKey,
  getRegisterBotMode,
} from "@/lib/security/env-guards";

export const dynamic = "force-dynamic";

/** Public: tell the register UI which anti-bot mode to render. */
export async function GET() {
  const mode = getRegisterBotMode();
  return NextResponse.json({
    ok: true,
    mode,
    turnstileSiteKey: mode === "turnstile" ? getPublicTurnstileSiteKey() : null,
  });
}
