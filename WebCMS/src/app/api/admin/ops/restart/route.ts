import { NextResponse } from "next/server";
import { requireAdmin } from "@/lib/auth";
import { writeAudit } from "@/lib/gs/audit";
import { restartServers, type RestartScope } from "@/lib/gs/ops";
import { clientSafeError } from "@/lib/security/client-error";

export const dynamic = "force-dynamic";
export const maxDuration = 300;

export async function POST(req: Request) {
  try {
    const session = await requireAdmin();
    const body = await req.json().catch(() => ({}));
    const scope = String(body.scope || "gameserver").toLowerCase() as RestartScope;
    if (scope !== "gameserver" && scope !== "all") {
      return NextResponse.json(
        { ok: false, error: "scope phải là gameserver hoặc all" },
        { status: 400 }
      );
    }

    const result = await restartServers(scope);
    await writeAudit({
      adminUser: session.user.name || session.user.id,
      action: "ops.restart",
      target: scope,
      detail: { log: result.log.slice(0, 4000) },
    });

    return NextResponse.json({
      ok: true,
      scope: result.scope,
      log: result.log,
      status: result.status,
    });
  } catch (e) {
    if (e instanceof Error && e.message === "UNAUTHORIZED_ADMIN") {
      return NextResponse.json(
        { ok: false, error: "Bạn không có quyền admin" },
        { status: 401 }
      );
    }
    return NextResponse.json(
      { ok: false, error: clientSafeError(e, "Restart thất bại") },
      { status: 500 }
    );
  }
}
