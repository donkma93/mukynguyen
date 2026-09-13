import { NextResponse } from "next/server";
import { requireAdmin } from "@/lib/auth";
import { writeAudit } from "@/lib/gs/audit";
import {
  getMossMerchantSettings,
  saveMossMerchantSettings,
  scheduleMossMerchantTest,
} from "@/lib/gs/moss-merchant";
import { restartServers } from "@/lib/gs/ops";
import { clientSafeError } from "@/lib/security/client-error";

export const dynamic = "force-dynamic";
export const runtime = "nodejs";
export const maxDuration = 300;

export async function GET() {
  try {
    await requireAdmin();
    return NextResponse.json({ ok: true, ...(await getMossMerchantSettings()) });
  } catch (e) {
    if (e instanceof Error && e.message === "UNAUTHORIZED_ADMIN") {
      return NextResponse.json({ ok: false, error: "Bạn không có quyền admin" }, { status: 401 });
    }
    return NextResponse.json(
      { ok: false, error: clientSafeError(e, "Không đọc được cấu hình Moss Merchant") },
      { status: 500 }
    );
  }
}

export async function POST(req: Request) {
  try {
    const session = await requireAdmin();
    const body = await req.json().catch(() => ({}));
    const action = String(body.action || "");
    const adminUser = session.user.name || session.user.id;

    if (action === "save") {
      const result = await saveMossMerchantSettings({
        enabled: Boolean(body.enabled),
        durationSeconds: Number(body.durationSeconds),
        dailyTimes: Array.isArray(body.dailyTimes) ? body.dailyTimes.map(String) : [],
      });
      await writeAudit({
        adminUser,
        action: "event.moss.save",
        target: "Moss Merchant",
        detail: { enabled: Boolean(body.enabled), durationSeconds: Number(body.durationSeconds), dailyTimes: body.dailyTimes },
      });
      return NextResponse.json({
        ok: true,
        message: "Đã lưu cấu hình cho 5 Sub. Restart GameServer để áp dụng.",
        backupCount: result.backups.length,
      });
    }

    if (action === "test") {
      const result = await scheduleMossMerchantTest(Number(body.delayMinutes));
      const restart = await restartServers("gameserver");
      await writeAudit({
        adminUser,
        action: "event.moss.test",
        target: "Moss Merchant",
        detail: { delayMinutes: Number(body.delayMinutes), scheduledAt: result.scheduledAt },
      });
      return NextResponse.json({
        ok: true,
        scheduledAt: result.scheduledAt,
        message: "Đã tạo lượt test, bật Moss Merchant và restart GameServer.",
        restartLog: restart.log,
      });
    }

    return NextResponse.json({ ok: false, error: "Thao tác không hợp lệ" }, { status: 400 });
  } catch (e) {
    if (e instanceof Error && e.message === "UNAUTHORIZED_ADMIN") {
      return NextResponse.json({ ok: false, error: "Bạn không có quyền admin" }, { status: 401 });
    }
    return NextResponse.json(
      { ok: false, error: clientSafeError(e, "Không thể cập nhật Moss Merchant") },
      { status: 500 }
    );
  }
}
