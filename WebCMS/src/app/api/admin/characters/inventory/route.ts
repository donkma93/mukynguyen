import { NextResponse } from "next/server";
import { requireAdmin } from "@/lib/auth";
import { writeAudit } from "@/lib/gs/audit";
import { getCharacterInventory, giveItemToCharacter } from "@/lib/game";

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
    const name = (searchParams.get("name") || "").trim();
    if (!name) {
      return NextResponse.json(
        { ok: false, error: "Thiếu tên nhân vật" },
        { status: 400 }
      );
    }
    const data = await getCharacterInventory(name);
    if (!data) {
      return NextResponse.json(
        { ok: false, error: "Nhân vật không tồn tại" },
        { status: 404 }
      );
    }
    return NextResponse.json({
      ok: true,
      account: data.account,
      name: data.name,
      extInventory: data.extInventory,
      online: data.online,
      bagItems: data.bagItems,
      bagCount: data.bagItems.length,
    });
  } catch (e) {
    const unauthorized = authError(e);
    if (unauthorized) return unauthorized;
    const message = e instanceof Error ? e.message : "Không đọc được inventory";
    return NextResponse.json({ ok: false, error: message }, { status: 500 });
  }
}

export async function POST(req: Request) {
  try {
    const session = await requireAdmin();
    const body = await req.json().catch(() => ({}));
    const name = String(body.name || "").trim();
    if (!name) {
      return NextResponse.json(
        { ok: false, error: "Thiếu tên nhân vật" },
        { status: 400 }
      );
    }

    const num = (v: unknown, fallback?: number) => {
      if (v === undefined || v === null || v === "") return fallback;
      const n = Number(v);
      return Number.isFinite(n) ? n : fallback;
    };

    const result = await giveItemToCharacter(name, {
      section: num(body.section),
      type: num(body.type),
      index: num(body.index),
      level: num(body.level, 0),
      skill: num(body.skill, 0),
      luck: num(body.luck, 0),
      option: num(body.option, 0),
      excellent: num(body.excellent, 0),
      setOption: num(body.setOption, 0),
      durability: num(body.durability),
      joh: num(body.joh, 0),
      optionEx: num(body.optionEx, 0),
      socketCount: num(body.socketCount, 0),
    });

    await writeAudit({
      adminUser: session.user.name || session.user.id,
      action: "character.give_item",
      target: name,
      detail: {
        slot: result.slot,
        item: result.item,
      },
    });

    return NextResponse.json({
      ok: true,
      message: `Đã đưa ${result.item.name} vào slot ${result.slot}`,
      ...result,
    });
  } catch (e) {
    const unauthorized = authError(e);
    if (unauthorized) return unauthorized;
    const message = e instanceof Error ? e.message : "Give item thất bại";
    const status = message.includes("Online") ? 409 : 400;
    return NextResponse.json({ ok: false, error: message }, { status });
  }
}
