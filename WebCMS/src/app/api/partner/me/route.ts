import { NextResponse } from "next/server";
import { requireUser } from "@/lib/auth";
import {
  getActiveLiveSession,
  listPartnerClaims,
  listPartnerSessions,
} from "@/lib/partner/gifts";
import { getPartner, getPartnerDashboard } from "@/lib/partner/partners";
import { PARTNER_PACKAGES } from "@/lib/partner/catalog";

export const dynamic = "force-dynamic";

export async function GET() {
  try {
    const session = await requireUser();
    const account = session.user.id;
    const partner = await getPartner(account);
    if (!partner || !partner.isActive) {
      return NextResponse.json(
        { ok: false, error: "Bạn không phải đối tác đang hoạt động", isPartner: false },
        { status: 403 }
      );
    }

    const dash = await getPartnerDashboard(account);
    const [sessions, claims, activeLive] = await Promise.all([
      listPartnerSessions(account, 15),
      listPartnerClaims(account, 30),
      getActiveLiveSession(account),
    ]);

    return NextResponse.json({
      ok: true,
      isPartner: true,
      partner: dash.partner,
      yearMonth: dash.yearMonth,
      budget: dash.budget,
      packages: PARTNER_PACKAGES,
      activeLive,
      sessions,
      claims,
    });
  } catch (e) {
    if (e instanceof Error && e.message === "UNAUTHORIZED_USER") {
      return NextResponse.json(
        { ok: false, error: "Bạn cần đăng nhập tài khoản game" },
        { status: 401 }
      );
    }
    const message = e instanceof Error ? e.message : "Không tải được panel đối tác";
    return NextResponse.json({ ok: false, error: message }, { status: 500 });
  }
}
