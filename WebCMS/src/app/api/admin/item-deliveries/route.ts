import { NextResponse } from "next/server";
import { requireAdmin } from "@/lib/auth";
import {
  cancelItemDelivery,
  listAdminItemDeliveries,
  queueItemDelivery,
} from "@/lib/admin-deliveries";
import { getGameItemByIndex } from "@/lib/item-catalog";

export const dynamic = "force-dynamic";

function unauthorized(error: unknown) {
  if (error instanceof Error && error.message === "UNAUTHORIZED_ADMIN") {
    return NextResponse.json({ ok: false, error: "Bạn không có quyền admin" }, { status: 401 });
  }
  return null;
}

const number = (value: unknown, fallback = 0) => {
  const parsed = Number(value ?? fallback);
  if (!Number.isFinite(parsed)) throw new Error("Dữ liệu số không hợp lệ");
  return parsed;
};

export async function GET(req: Request) {
  try {
    await requireAdmin();
    const { searchParams } = new URL(req.url);
    const items = await listAdminItemDeliveries(
      searchParams.get("character") || undefined,
      number(searchParams.get("limit"), 100)
    );
    return NextResponse.json({ ok: true, items });
  } catch (error) {
    const auth = unauthorized(error);
    if (auth) return auth;
    return NextResponse.json(
      { ok: false, error: error instanceof Error ? error.message : "Không tải được lịch sử đẩy đồ" },
      { status: 500 }
    );
  }
}

export async function POST(req: Request) {
  try {
    const session = await requireAdmin();
    const body = await req.json();
    const characterName = String(body.characterName || "").trim();
    if (!characterName) {
      return NextResponse.json({ ok: false, error: "Chưa chọn nhân vật" }, { status: 400 });
    }

    const itemIndex = number(body.itemIndex);
    if (itemIndex < 0 || itemIndex > 8191) {
      return NextResponse.json({ ok: false, error: "Item Index phải nằm trong khoảng 0–8191" }, { status: 400 });
    }
    const socketCount = number(body.socketCount);
    const gameItem = getGameItemByIndex(itemIndex);
    if (!gameItem) {
      return NextResponse.json({ ok: false, error: "Không tìm thấy vật phẩm trong Item.txt của GameServer" }, { status: 400 });
    }
    if (socketCount > gameItem.maxSocket) {
      return NextResponse.json(
        { ok: false, error: gameItem.maxSocket > 0 ? `${gameItem.name} chỉ hỗ trợ tối đa ${gameItem.maxSocket} socket` : `${gameItem.name} không hỗ trợ socket` },
        { status: 400 }
      );
    }

    const item = await queueItemDelivery({
      characterName,
      itemIndex,
      itemLevel: number(body.itemLevel),
      durability: number(body.durability, 255),
      skill: Boolean(body.skill),
      luck: Boolean(body.luck),
      option: number(body.option),
      excellent: number(body.excellent),
      ancient: number(body.ancient),
      socketCount,
      durationSeconds: number(body.durationSeconds),
      quantity: number(body.quantity, 1),
      note: String(body.note || ""),
      createdBy: session.user.name || session.user.id,
    });
    return NextResponse.json({
      ok: true,
      item,
      message: `Đã xếp ${item.quantity} vật phẩm cho ${item.characterName}. GameServer sẽ giao khi nhân vật vào game.`,
    });
  } catch (error) {
    const auth = unauthorized(error);
    if (auth) return auth;
    return NextResponse.json(
      { ok: false, error: error instanceof Error ? error.message : "Không thể tạo lệnh đẩy đồ" },
      { status: 400 }
    );
  }
}

export async function DELETE(req: Request) {
  try {
    await requireAdmin();
    const { searchParams } = new URL(req.url);
    const id = number(searchParams.get("id"));
    if (!Number.isInteger(id) || id <= 0) {
      return NextResponse.json({ ok: false, error: "ID lệnh không hợp lệ" }, { status: 400 });
    }
    const item = await cancelItemDelivery(id);
    return NextResponse.json({ ok: true, item, message: "Đã hủy lệnh đẩy đồ" });
  } catch (error) {
    const auth = unauthorized(error);
    if (auth) return auth;
    return NextResponse.json(
      { ok: false, error: error instanceof Error ? error.message : "Không thể hủy lệnh" },
      { status: 400 }
    );
  }
}
