import { NextResponse } from "next/server";
import { requireAdmin } from "@/lib/auth";
import { findAccount } from "@/lib/game";
import { getBudgetSnapshot } from "@/lib/partner/budget";
import { listPartners, upsertPartner } from "@/lib/partner/partners";
import { vietnamYearMonth } from "@/lib/partner/catalog";
import { partnerUpsertSchema } from "@/lib/validators";

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

export async function GET() {
  try {
    await requireAdmin();
    const yearMonth = vietnamYearMonth();
    const partners = await listPartners();
    const items = await Promise.all(
      partners.map(async (p) => {
        const budget = await getBudgetSnapshot(p.account, p.tier, yearMonth);
        return {
          account: p.account,
          tier: p.tier,
          isActive: p.isActive,
          createdBy: p.createdBy,
          createdAt: p.createdAt,
          updatedAt: p.updatedAt,
          yearMonth,
          budget: budget.remaining,
          used: budget.used,
          caps: budget.caps,
        };
      })
    );
    return NextResponse.json({ ok: true, items });
  } catch (e) {
    const unauthorized = authError(e);
    if (unauthorized) return unauthorized;
    const message = e instanceof Error ? e.message : "Không tải được danh sách đối tác";
    return NextResponse.json({ ok: false, error: message }, { status: 500 });
  }
}

export async function POST(req: Request) {
  try {
    const session = await requireAdmin();
    const body = await req.json();
    const parsed = partnerUpsertSchema.safeParse({
      account: body.account,
      tier: body.tier,
      isActive:
        body.isActive !== undefined
          ? Boolean(body.isActive)
          : body.is_active !== undefined
            ? Boolean(body.is_active)
            : undefined,
    });
    if (!parsed.success) {
      const msg = parsed.error.issues[0]?.message || "Dữ liệu không hợp lệ";
      return NextResponse.json({ ok: false, error: msg }, { status: 400 });
    }

    const existing = await findAccount(parsed.data.account);
    if (!existing) {
      return NextResponse.json(
        { ok: false, error: "Tài khoản game không tồn tại" },
        { status: 404 }
      );
    }

    const partner = await upsertPartner({
      account: parsed.data.account,
      tier: parsed.data.tier,
      isActive: parsed.data.isActive,
      createdBy: session.user.name || session.user.id,
    });

    return NextResponse.json({
      ok: true,
      item: partner,
      message: "Đã cập nhật đối tác",
    });
  } catch (e) {
    const unauthorized = authError(e);
    if (unauthorized) return unauthorized;
    const message = e instanceof Error ? e.message : "Cập nhật đối tác thất bại";
    return NextResponse.json({ ok: false, error: message }, { status: 500 });
  }
}
