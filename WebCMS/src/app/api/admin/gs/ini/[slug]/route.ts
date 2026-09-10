import { NextResponse } from "next/server";
import { requireAdmin } from "@/lib/auth";
import { writeAudit } from "@/lib/gs/audit";
import { readTextFile, writeTextFile } from "@/lib/gs/files";
import { applyIniUpdates, listIniEntries, parseIni } from "@/lib/gs/ini";
import { reloadCharacterIni } from "@/lib/gs/ops";
import {
  gsIniFileToSlug,
  gsIniSlugToFile,
  STARTUP_ONLY_KEYS,
} from "@/lib/gs/paths";
import { clientSafeError, publicFileName } from "@/lib/security/client-error";

export const dynamic = "force-dynamic";

type Ctx = { params: Promise<{ slug: string }> };

export async function GET(_req: Request, ctx: Ctx) {
  try {
    await requireAdmin();
    const { slug } = await ctx.params;
    const fileName = gsIniSlugToFile(slug);
    if (!fileName) {
      return NextResponse.json(
        { ok: false, error: "INI không hợp lệ" },
        { status: 404 }
      );
    }
    const raw = await readTextFile("ini", fileName);
    const entries = listIniEntries(raw);
    const startupKeys = entries
      .filter((e) => STARTUP_ONLY_KEYS.has(e.key))
      .map((e) => e.key);

    return NextResponse.json({
      ok: true,
      slug: gsIniFileToSlug(fileName),
      fileName,
      raw,
      entries,
      startupKeys,
    });
  } catch (e) {
    if (e instanceof Error && e.message === "UNAUTHORIZED_ADMIN") {
      return NextResponse.json(
        { ok: false, error: "Bạn không có quyền admin" },
        { status: 401 }
      );
    }
    return NextResponse.json(
      { ok: false, error: clientSafeError(e, "Không đọc được INI") },
      { status: 500 }
    );
  }
}

export async function PUT(req: Request, ctx: Ctx) {
  try {
    const session = await requireAdmin();
    const { slug } = await ctx.params;
    const fileName = gsIniSlugToFile(slug);
    if (!fileName) {
      return NextResponse.json(
        { ok: false, error: "INI không hợp lệ" },
        { status: 404 }
      );
    }

    const body = await req.json().catch(() => ({}));
    const mode = String(body.mode || "keys").toLowerCase();
    const current = await readTextFile("ini", fileName);

    let nextText = current;
    let changed: string[] = [];
    let missingCreated: string[] = [];

    if (mode === "raw") {
      if (typeof body.raw !== "string") {
        return NextResponse.json(
          { ok: false, error: "Thiếu nội dung raw" },
          { status: 400 }
        );
      }
      nextText = body.raw.replace(/\r?\n/g, "\r\n");
      if (!nextText.endsWith("\r\n")) nextText += "\r\n";
      // validate parseable
      parseIni(nextText);
      changed = ["<raw>"];
    } else {
      const updates = (body.updates || {}) as Record<string, string>;
      if (!updates || typeof updates !== "object" || Array.isArray(updates)) {
        return NextResponse.json(
          { ok: false, error: "updates phải là object key→value" },
          { status: 400 }
        );
      }
      const normalized: Record<string, string> = {};
      for (const [k, v] of Object.entries(updates)) {
        if (!k || typeof k !== "string") continue;
        normalized[k] = String(v ?? "").trim();
      }
      if (!Object.keys(normalized).length) {
        return NextResponse.json(
          { ok: false, error: "Không có key nào để cập nhật" },
          { status: 400 }
        );
      }
      const applied = applyIniUpdates(current, normalized);
      nextText = applied.text;
      changed = applied.changed;
      missingCreated = applied.missingCreated;
    }

    if (nextText === current) {
      return NextResponse.json({
        ok: true,
        fileName,
        changed: [],
        backup: null,
        message: "Không có thay đổi",
      });
    }

    const { backupPath } = await writeTextFile("ini", fileName, nextText);
    const touchedStartup = changed.filter((k) => STARTUP_ONLY_KEYS.has(k));
    const fileSlug = gsIniFileToSlug(fileName);

    let reload: { ok: boolean; message: string } | null = null;
    if (fileSlug === "character" && touchedStartup.length === 0) {
      reload = await reloadCharacterIni();
    }

    await writeAudit({
      adminUser: session.user.name || session.user.id,
      action: "gs.ini.save",
      target: fileName,
      detail: {
        mode,
        changed,
        missingCreated,
        backupPath,
        touchedStartup,
        reload,
      },
    });

    const needsFullStackRestart = touchedStartup.length > 0;
    let message = "Đã lưu.";
    if (needsFullStackRestart) {
      message =
        "Đã lưu. Có key startup — cần Restart full stack để áp dụng.";
    } else if (reload?.ok) {
      message =
        "Đã lưu và Reload Character trên GameServer — số trong game đã khớp web.";
    } else if (reload) {
      message = `Đã lưu file. ${reload.message}`;
    } else {
      message = "Đã lưu. Restart GameServer để áp dụng.";
    }

    return NextResponse.json({
      ok: true,
      fileName,
      slug: fileSlug,
      changed,
      missingCreated,
      backup: backupPath ? publicFileName(backupPath) : null,
      touchedStartup,
      reload,
      needsRestart: !reload?.ok,
      needsFullStackRestart,
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
      { ok: false, error: clientSafeError(e, "Lưu INI thất bại") },
      { status: 500 }
    );
  }
}
