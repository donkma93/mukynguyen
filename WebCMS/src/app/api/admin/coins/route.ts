import { NextResponse } from "next/server";
import { requireAdmin } from "@/lib/auth";
import { addCoins, findAccount } from "@/lib/game";

export const dynamic = "force-dynamic";

export async function POST(req: Request) {
  try {
    await requireAdmin();
    const body = await req.json();
    const account = String(body.account || "").trim().toLowerCase();
    if (!account) {
      return NextResponse.json(
        { ok: false, error: "Thiếu tài khoản" },
        { status: 400 }
      );
    }

    const existing = await findAccount(account);
    if (!existing) {
      return NextResponse.json(
        { ok: false, error: "Tài khoản không tồn tại" },
        { status: 404 }
      );
    }

    const wc = Number(body.wc ?? 0);
    const wp = Number(body.wp ?? 0);
    const wg = Number(body.wg ?? 0);
    const rd = Number(body.rd ?? 0);
    const at = Number(body.at ?? 0);

    if (![wc, wp, wg, rd, at].every((n) => Number.isFinite(n))) {
      return NextResponse.json(
        { ok: false, error: "Số coin không hợp lệ" },
        { status: 400 }
      );
    }

    if (wc === 0 && wp === 0 && wg === 0 && rd === 0 && at === 0) {
      return NextResponse.json(
        { ok: false, error: "Cần ít nhất một loại coin khác 0" },
        { status: 400 }
      );
    }

    await addCoins(account, {
      wc: Math.trunc(wc),
      wp: Math.trunc(wp),
      wg: Math.trunc(wg),
      rd: Math.trunc(rd),
      at: Math.trunc(at),
    });

    return NextResponse.json({
      ok: true,
      account,
      coins: { wc, wp, wg, rd, at },
    });
  } catch (e) {
    if (e instanceof Error && e.message === "UNAUTHORIZED_ADMIN") {
      return NextResponse.json(
        { ok: false, error: "Bạn không có quyền admin" },
        { status: 401 }
      );
    }
    const message = e instanceof Error ? e.message : "Cộng coin thất bại";
    return NextResponse.json({ ok: false, error: message }, { status: 500 });
  }
}
