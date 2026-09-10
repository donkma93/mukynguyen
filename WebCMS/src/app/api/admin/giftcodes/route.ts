import { NextResponse } from "next/server";
import { requireAdmin } from "@/lib/auth";
import {
  createGiftcode,
  deleteGiftcode,
  listGiftcodes,
  updateGiftcode,
} from "@/lib/cms";

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
    const items = await listGiftcodes();
    return NextResponse.json({ ok: true, items });
  } catch (e) {
    const unauthorized = authError(e);
    if (unauthorized) return unauthorized;
    const message = e instanceof Error ? e.message : "Không tải được giftcode";
    return NextResponse.json({ ok: false, error: message }, { status: 500 });
  }
}

export async function POST(req: Request) {
  try {
    await requireAdmin();
    const body = await req.json();
    const code = String(body.code || "").trim();
    if (!code) {
      return NextResponse.json(
        { ok: false, error: "Thiếu mã giftcode" },
        { status: 400 }
      );
    }

    const item = await createGiftcode({
      code,
      description: body.description ? String(body.description) : undefined,
      rewardWc: Number(body.rewardWc ?? body.reward_wc ?? 0),
      rewardWp: Number(body.rewardWp ?? body.reward_wp ?? 0),
      rewardWg: Number(body.rewardWg ?? body.reward_wg ?? 0),
      maxUses: Number(body.maxUses ?? body.max_uses ?? 1),
      expiresAt:
        body.expiresAt !== undefined
          ? body.expiresAt
          : body.expires_at !== undefined
            ? body.expires_at
            : null,
      isActive:
        body.isActive !== undefined
          ? Boolean(body.isActive)
          : body.is_active !== undefined
            ? Boolean(body.is_active)
            : true,
    });

    return NextResponse.json({ ok: true, item });
  } catch (e) {
    const unauthorized = authError(e);
    if (unauthorized) return unauthorized;
    const message = e instanceof Error ? e.message : "Tạo giftcode thất bại";
    return NextResponse.json({ ok: false, error: message }, { status: 500 });
  }
}

export async function PUT(req: Request) {
  try {
    await requireAdmin();
    const body = await req.json();
    const id = Number(body.id);
    if (!Number.isInteger(id) || id <= 0) {
      return NextResponse.json(
        { ok: false, error: "ID giftcode không hợp lệ" },
        { status: 400 }
      );
    }

    const item = await updateGiftcode(id, {
      code: body.code !== undefined ? String(body.code) : undefined,
      description:
        body.description !== undefined
          ? body.description === null
            ? null
            : String(body.description)
          : undefined,
      rewardWc:
        body.rewardWc !== undefined
          ? Number(body.rewardWc)
          : body.reward_wc !== undefined
            ? Number(body.reward_wc)
            : undefined,
      rewardWp:
        body.rewardWp !== undefined
          ? Number(body.rewardWp)
          : body.reward_wp !== undefined
            ? Number(body.reward_wp)
            : undefined,
      rewardWg:
        body.rewardWg !== undefined
          ? Number(body.rewardWg)
          : body.reward_wg !== undefined
            ? Number(body.reward_wg)
            : undefined,
      maxUses:
        body.maxUses !== undefined
          ? Number(body.maxUses)
          : body.max_uses !== undefined
            ? Number(body.max_uses)
            : undefined,
      expiresAt:
        body.expiresAt !== undefined
          ? body.expiresAt
          : body.expires_at !== undefined
            ? body.expires_at
            : undefined,
      isActive:
        body.isActive !== undefined
          ? Boolean(body.isActive)
          : body.is_active !== undefined
            ? Boolean(body.is_active)
            : undefined,
    });

    return NextResponse.json({ ok: true, item });
  } catch (e) {
    const unauthorized = authError(e);
    if (unauthorized) return unauthorized;
    const message = e instanceof Error ? e.message : "Cập nhật giftcode thất bại";
    const status = message.includes("không tồn tại") ? 404 : 500;
    return NextResponse.json({ ok: false, error: message }, { status });
  }
}

export async function DELETE(req: Request) {
  try {
    await requireAdmin();
    const body = await req.json().catch(() => ({}));
    const { searchParams } = new URL(req.url);
    const id = Number(body.id ?? searchParams.get("id"));
    if (!Number.isInteger(id) || id <= 0) {
      return NextResponse.json(
        { ok: false, error: "ID giftcode không hợp lệ" },
        { status: 400 }
      );
    }

    await deleteGiftcode(id);
    return NextResponse.json({ ok: true });
  } catch (e) {
    const unauthorized = authError(e);
    if (unauthorized) return unauthorized;
    const message = e instanceof Error ? e.message : "Xóa giftcode thất bại";
    const status = message.includes("không tồn tại") ? 404 : 500;
    return NextResponse.json({ ok: false, error: message }, { status });
  }
}
