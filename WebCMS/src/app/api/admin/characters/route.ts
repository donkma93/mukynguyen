import { NextResponse } from "next/server";
import { requireAdmin } from "@/lib/auth";
import {
  className,
  getCharacterByName,
  searchCharacters,
  updateCharacterStats,
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

function mapChar(c: NonNullable<Awaited<ReturnType<typeof getCharacterByName>>>) {
  return {
    account: c.AccountID,
    name: c.Name,
    classId: c.Class,
    className: className(c.Class),
    cLevel: c.cLevel,
    levelUpPoint: c.LevelUpPoint,
    strength: c.Strength,
    dexterity: c.Dexterity,
    vitality: c.Vitality,
    energy: c.Energy,
    leadership: c.Leadership,
    resetCount: c.ResetCount,
    masterResetCount: c.MasterResetCount,
    money: Number(c.Money),
    kills: c.Kills,
    deads: c.Deads,
    mapNumber: c.MapNumber,
    mapPosX: c.MapPosX,
    mapPosY: c.MapPosY,
    ctlCode: c.CtlCode,
    rDanhHieu: c.rDanhHieu,
    rQuanHam: c.rQuanHam,
    rTuLuyen: c.rTuLuyen,
    rHonHoan: c.rHonHoan,
    rNewVip: c.rNewVip,
    rHuyChuong: c.rHuyChuong,
  };
}

export async function GET(req: Request) {
  try {
    await requireAdmin();
    const { searchParams } = new URL(req.url);
    const name = (searchParams.get("name") || "").trim();
    const q = (searchParams.get("q") || "").trim();

    if (name) {
      const row = await getCharacterByName(name);
      if (!row) {
        return NextResponse.json(
          { ok: false, error: "Nhân vật không tồn tại" },
          { status: 404 }
        );
      }
      return NextResponse.json({ ok: true, item: mapChar(row) });
    }

    if (!q) return NextResponse.json({ ok: true, items: [] });
    const items = await searchCharacters(q);
    return NextResponse.json({ ok: true, items: items.map(mapChar) });
  } catch (e) {
    const unauthorized = authError(e);
    if (unauthorized) return unauthorized;
    const message = e instanceof Error ? e.message : "Không tải được nhân vật";
    return NextResponse.json({ ok: false, error: message }, { status: 500 });
  }
}

export async function POST(req: Request) {
  try {
    await requireAdmin();
    const body = await req.json();
    const name = String(body.name || "").trim();
    if (!name) {
      return NextResponse.json({ ok: false, error: "Thiếu tên nhân vật" }, { status: 400 });
    }

    const num = (v: unknown) =>
      v === undefined || v === null || v === "" ? undefined : Number(v);

    const updated = await updateCharacterStats(name, {
      Class: num(body.classId ?? body.Class),
      cLevel: num(body.cLevel),
      LevelUpPoint: num(body.levelUpPoint ?? body.LevelUpPoint),
      Strength: num(body.strength ?? body.Strength),
      Dexterity: num(body.dexterity ?? body.Dexterity),
      Vitality: num(body.vitality ?? body.Vitality),
      Energy: num(body.energy ?? body.Energy),
      Leadership: num(body.leadership ?? body.Leadership),
      ResetCount: num(body.resetCount ?? body.ResetCount),
      MasterResetCount: num(body.masterResetCount ?? body.MasterResetCount),
      Money: num(body.money ?? body.Money),
      MapNumber: num(body.mapNumber ?? body.MapNumber),
      MapPosX: num(body.mapPosX ?? body.MapPosX),
      MapPosY: num(body.mapPosY ?? body.MapPosY),
      CtlCode: num(body.ctlCode ?? body.CtlCode),
      rDanhHieu: num(body.rDanhHieu),
      rQuanHam: num(body.rQuanHam),
      rTuLuyen: num(body.rTuLuyen),
      rHonHoan: num(body.rHonHoan),
      rNewVip: num(body.rNewVip),
      rHuyChuong: num(body.rHuyChuong),
      ResetPointBonusApplied: body.resetPointBonusApplied === true,
    });

    if (!updated) {
      return NextResponse.json({ ok: false, error: "Cập nhật thất bại" }, { status: 500 });
    }

    return NextResponse.json({
      ok: true,
      message: "Đã cập nhật nhân vật",
      item: mapChar(updated),
    });
  } catch (e) {
    const unauthorized = authError(e);
    if (unauthorized) return unauthorized;
    const message = e instanceof Error ? e.message : "Cập nhật nhân vật thất bại";
    return NextResponse.json({ ok: false, error: message }, { status: 400 });
  }
}
