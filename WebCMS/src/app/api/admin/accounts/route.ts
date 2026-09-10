import { NextResponse } from "next/server";
import { requireAdmin } from "@/lib/auth";
import {
  addCoins,
  findAccount,
  searchAccounts,
  setAccountBlock,
  setVip,
} from "@/lib/game";

export const dynamic = "force-dynamic";

function authError(e: unknown) {
  if (e instanceof Error && e.message === "UNAUTHORIZED_ADMIN") {
    return NextResponse.json(
      { ok: false, error: "Bạn không có quyền admin" },
      { status: 401 }
    );
  }
  return null;
}

export async function GET(req: Request) {
  try {
    await requireAdmin();
    const { searchParams } = new URL(req.url);
    const q = (searchParams.get("q") || "").trim();
    if (!q) {
      return NextResponse.json({ ok: true, items: [] });
    }
    const items = await searchAccounts(q);
    return NextResponse.json({
      ok: true,
      items: items.map((a) => ({
        account: a.memb___id,
        email: a.mail_addr,
        blocked: String(a.bloc_code || "0").trim() !== "0" || Number(a.Lock || 0) !== 0,
        vip: a.AccountLevel ?? 0,
        expireDate: a.AccountExpireDate,
      })),
    });
  } catch (e) {
    const unauthorized = authError(e);
    if (unauthorized) return unauthorized;
    const message = e instanceof Error ? e.message : "Không tìm được tài khoản";
    return NextResponse.json({ ok: false, error: message }, { status: 500 });
  }
}

export async function POST(req: Request) {
  try {
    await requireAdmin();
    const body = await req.json();
    const account = String(body.account || "").trim().toLowerCase();
    const action = String(body.action || "").toLowerCase();

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

    switch (action) {
      case "block":
        await setAccountBlock(account, true);
        return NextResponse.json({ ok: true, account, blocked: true });
      case "unblock":
        await setAccountBlock(account, false);
        return NextResponse.json({ ok: true, account, blocked: false });
      case "vip": {
        const level = Number(body.level ?? body.vip ?? 0);
        const days = Number(body.days ?? body.expireDays ?? 0);
        if (!Number.isFinite(level) || level < 0) {
          return NextResponse.json(
            { ok: false, error: "Cấp VIP không hợp lệ (0 = Thường, 1 = VIP)" },
            { status: 400 }
          );
        }
        // Single VIP type: clamp to 0 or 1
        const vipLevel = level > 0 ? 1 : 0;
        await setVip(account, vipLevel, Math.floor(days || 0));
        return NextResponse.json({ ok: true, account, vip: vipLevel, days });
      }
      case "coins": {
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
      }
      default:
        return NextResponse.json(
          { ok: false, error: "Hành động không hợp lệ" },
          { status: 400 }
        );
    }
  } catch (e) {
    const unauthorized = authError(e);
    if (unauthorized) return unauthorized;
    const message = e instanceof Error ? e.message : "Thao tác tài khoản thất bại";
    return NextResponse.json({ ok: false, error: message }, { status: 500 });
  }
}
