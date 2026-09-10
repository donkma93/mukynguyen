import { NextResponse } from "next/server";
import { requireAdmin } from "@/lib/auth";
import { writeAudit } from "@/lib/gs/audit";
import {
  GS_GROUPS,
  GS_SUBS,
  loadGroupRatesSnapshot,
  writeGroupRates,
  type GsGroupId,
} from "@/lib/gs/groups";
import { clientSafeError } from "@/lib/security/client-error";

export const dynamic = "force-dynamic";

export async function GET() {
  try {
    await requireAdmin();
    const snapshot = await loadGroupRatesSnapshot();
    return NextResponse.json({
      ok: true,
      groups: GS_GROUPS,
      subs: GS_SUBS,
      rates: {
        normal: snapshot.normal,
        vip: snapshot.vip,
      },
      available: snapshot.available,
      missing: snapshot.missing,
    });
  } catch (e) {
    if (e instanceof Error && e.message === "UNAUTHORIZED_ADMIN") {
      return NextResponse.json(
        { ok: false, error: "Bạn không có quyền admin" },
        { status: 401 }
      );
    }
    return NextResponse.json(
      { ok: false, error: clientSafeError(e, "Không đọc được nhóm GS") },
      { status: 500 }
    );
  }
}

export async function PUT(req: Request) {
  try {
    const session = await requireAdmin();
    const body = await req.json().catch(() => ({}));
    const group = String(body.group || "").toLowerCase() as GsGroupId;
    if (group !== "normal" && group !== "vip") {
      return NextResponse.json(
        { ok: false, error: "group phải là normal hoặc vip" },
        { status: 400 }
      );
    }

    const updates: { itemDropRate?: number; moneyDropRate?: number } = {};
    if (body.itemDropRate != null && body.itemDropRate !== "") {
      const n = Number(body.itemDropRate);
      if (!Number.isFinite(n) || n < 0) {
        return NextResponse.json(
          { ok: false, error: "itemDropRate không hợp lệ" },
          { status: 400 }
        );
      }
      updates.itemDropRate = Math.floor(n);
    }
    if (body.moneyDropRate != null && body.moneyDropRate !== "") {
      const n = Number(body.moneyDropRate);
      if (!Number.isFinite(n) || n < 0) {
        return NextResponse.json(
          { ok: false, error: "moneyDropRate không hợp lệ" },
          { status: 400 }
        );
      }
      updates.moneyDropRate = Math.floor(n);
    }
    if (!Object.keys(updates).length) {
      return NextResponse.json(
        { ok: false, error: "Thiếu itemDropRate hoặc moneyDropRate" },
        { status: 400 }
      );
    }

    const result = await writeGroupRates(group, updates);
    await writeAudit({
      adminUser: session.user.name || session.user.id,
      action: "gs.groups.rates",
      target: group,
      detail: { updates, ...result },
    });

    const snapshot = await loadGroupRatesSnapshot();
    const pendingOnly = result.written.some((w) =>
      String(w.label).startsWith("pending:")
    );
    const gsWritten = result.written.filter(
      (w) => !String(w.label).startsWith("pending:")
    ).length;
    let message: string;
    if (pendingOnly && gsWritten === 0) {
      message = `Đã lưu tỷ lệ nhóm ${group === "vip" ? "VIP" : "Thường"} (chờ tạo folder Sub). Guide đã cập nhật.`;
    } else if (gsWritten > 0) {
      message = `Đã đồng bộ ${gsWritten} GS nhóm ${group === "vip" ? "VIP" : "Thường"}. Restart GameServer để áp dụng.`;
    } else {
      message = "Không ghi được GS nào (folder chưa có hoặc không đổi).";
    }

    return NextResponse.json({
      ok: true,
      group,
      updates,
      written: result.written,
      skipped: result.skipped,
      rates: {
        normal: snapshot.normal,
        vip: snapshot.vip,
      },
      available: snapshot.available,
      missing: snapshot.missing,
      message,
    });
  } catch (e) {
    if (e instanceof Error && e.message === "UNAUTHORIZED_ADMIN") {
      return NextResponse.json(
        { ok: false, error: "Bạn không có quyền admin" },
        { status: 401 }
      );
    }
    return NextResponse.json(
      { ok: false, error: clientSafeError(e, "Lưu tỷ lệ nhóm thất bại") },
      { status: 500 }
    );
  }
}
