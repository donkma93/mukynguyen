import { NextResponse } from "next/server";
import { requireAdmin } from "@/lib/auth";
import {
  createPost,
  deletePost,
  listAllPosts,
  updatePost,
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
    const items = await listAllPosts();
    return NextResponse.json({ ok: true, items });
  } catch (e) {
    const unauthorized = authError(e);
    if (unauthorized) return unauthorized;
    const message = e instanceof Error ? e.message : "Không tải được bài viết";
    return NextResponse.json({ ok: false, error: message }, { status: 500 });
  }
}

export async function POST(req: Request) {
  try {
    const session = await requireAdmin();
    const body = await req.json();
    const title = String(body.title || "").trim();
    const content = String(body.content || "");
    if (!title || !content) {
      return NextResponse.json(
        { ok: false, error: "Thiếu tiêu đề hoặc nội dung" },
        { status: 400 }
      );
    }

    const item = await createPost({
      title,
      slug: body.slug ? String(body.slug) : undefined,
      excerpt: body.excerpt ? String(body.excerpt) : undefined,
      content,
      postType: body.postType || body.post_type || "news",
      coverUrl: body.coverUrl || body.cover_url || undefined,
      isPublished: Boolean(body.isPublished ?? body.is_published ?? false),
      author: body.author ? String(body.author) : session.user.name || session.user.id,
    });

    return NextResponse.json({ ok: true, item });
  } catch (e) {
    const unauthorized = authError(e);
    if (unauthorized) return unauthorized;
    const message = e instanceof Error ? e.message : "Tạo bài viết thất bại";
    const status = message.toLowerCase().includes("unique") || message.includes("slug")
      ? 400
      : 500;
    return NextResponse.json({ ok: false, error: message }, { status });
  }
}

export async function PUT(req: Request) {
  try {
    await requireAdmin();
    const body = await req.json();
    const id = Number(body.id);
    if (!Number.isInteger(id) || id <= 0) {
      return NextResponse.json(
        { ok: false, error: "ID bài viết không hợp lệ" },
        { status: 400 }
      );
    }

    const item = await updatePost(id, {
      title: body.title !== undefined ? String(body.title) : undefined,
      slug: body.slug !== undefined ? String(body.slug) : undefined,
      excerpt:
        body.excerpt !== undefined
          ? body.excerpt === null
            ? null
            : String(body.excerpt)
          : undefined,
      content: body.content !== undefined ? String(body.content) : undefined,
      postType:
        body.postType !== undefined
          ? String(body.postType)
          : body.post_type !== undefined
            ? String(body.post_type)
            : undefined,
      coverUrl:
        body.coverUrl !== undefined
          ? body.coverUrl
          : body.cover_url !== undefined
            ? body.cover_url
            : undefined,
      isPublished:
        body.isPublished !== undefined
          ? Boolean(body.isPublished)
          : body.is_published !== undefined
            ? Boolean(body.is_published)
            : undefined,
      author:
        body.author !== undefined
          ? body.author === null
            ? null
            : String(body.author)
          : undefined,
    });

    return NextResponse.json({ ok: true, item });
  } catch (e) {
    const unauthorized = authError(e);
    if (unauthorized) return unauthorized;
    const message = e instanceof Error ? e.message : "Cập nhật bài viết thất bại";
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
        { ok: false, error: "ID bài viết không hợp lệ" },
        { status: 400 }
      );
    }

    await deletePost(id);
    return NextResponse.json({ ok: true });
  } catch (e) {
    const unauthorized = authError(e);
    if (unauthorized) return unauthorized;
    const message = e instanceof Error ? e.message : "Xóa bài viết thất bại";
    const status = message.includes("không tồn tại") ? 404 : 500;
    return NextResponse.json({ ok: false, error: message }, { status });
  }
}
