"use client";

import { FormEvent, useEffect, useState } from "react";
import FlashMessage from "@/components/FlashMessage";

type PostRow = {
  id: number;
  slug: string;
  title: string;
  excerpt?: string | null;
  content?: string;
  isPublished?: boolean;
};

export default function PostsPanel() {
  const [posts, setPosts] = useState<PostRow[]>([]);
  const [slug, setSlug] = useState("");
  const [title, setTitle] = useState("");
  const [excerpt, setExcerpt] = useState("");
  const [content, setContent] = useState("");
  const [isPublished, setIsPublished] = useState(true);
  const [editingId, setEditingId] = useState<number | null>(null);
  const [error, setError] = useState<string | null>(null);
  const [message, setMessage] = useState<string | null>(null);
  const [loading, setLoading] = useState(false);

  async function load() {
    try {
      const res = await fetch("/api/admin/posts");
      const data = await res.json().catch(() => ({}));
      const list = (data.items || []) as PostRow[];
      setPosts(Array.isArray(list) ? list : []);
    } catch {
      setPosts([]);
    }
  }

  useEffect(() => {
    void load();
  }, []);

  function resetForm() {
    setEditingId(null);
    setSlug("");
    setTitle("");
    setExcerpt("");
    setContent("");
    setIsPublished(true);
  }

  function edit(post: PostRow) {
    setEditingId(post.id);
    setSlug(post.slug);
    setTitle(post.title);
    setExcerpt(post.excerpt || "");
    setContent(post.content || "");
    setIsPublished(Boolean(post.isPublished));
  }

  async function onSubmit(e: FormEvent) {
    e.preventDefault();
    setLoading(true);
    setError(null);
    setMessage(null);
    try {
      const res = await fetch("/api/admin/posts", {
        method: editingId ? "PUT" : "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({
          id: editingId,
          slug,
          title,
          excerpt,
          content,
          isPublished,
        }),
      });
      const data = await res.json().catch(() => ({}));
      if (!res.ok) {
        setError(data.error || data.message || "Lưu bài viết thất bại");
        return;
      }
      setMessage(editingId ? "Đã cập nhật bài viết." : "Đã tạo bài viết.");
      resetForm();
      await load();
    } catch {
      setError("Không thể kết nối máy chủ.");
    } finally {
      setLoading(false);
    }
  }

  return (
    <div className="space-y-6">
      <FlashMessage type="error" message={error} />
      <FlashMessage type="success" message={message} />

      <form onSubmit={onSubmit} className="card space-y-3 p-5">
        <h2 className="panel-title">{editingId ? "Sửa bài viết" : "Tạo bài viết"}</h2>
        <div className="grid gap-3 md:grid-cols-2">
          <div>
            <label className="label">Tiêu đề</label>
            <input className="input" value={title} onChange={(e) => setTitle(e.target.value)} required />
          </div>
          <div>
            <label className="label">Slug</label>
            <input className="input" value={slug} onChange={(e) => setSlug(e.target.value)} />
          </div>
        </div>
        <div>
          <label className="label">Tóm tắt</label>
          <input className="input" value={excerpt} onChange={(e) => setExcerpt(e.target.value)} />
        </div>
        <div>
          <label className="label">Nội dung</label>
          <textarea
            className="input min-h-[160px]"
            value={content}
            onChange={(e) => setContent(e.target.value)}
            required
          />
        </div>
        <label className="flex items-center gap-2 text-sm text-gray-300">
          <input
            type="checkbox"
            checked={isPublished}
            onChange={(e) => setIsPublished(e.target.checked)}
          />
          Xuất bản ngay
        </label>
        <div className="flex gap-2">
          <button type="submit" className="btn-gold" disabled={loading}>
            {loading ? "Đang lưu..." : editingId ? "Cập nhật" : "Tạo mới"}
          </button>
          {editingId ? (
            <button type="button" className="btn-ghost" onClick={resetForm}>
              Huỷ
            </button>
          ) : null}
        </div>
      </form>

      <div className="card p-5">
        <h2 className="panel-title mb-4">Danh sách bài viết</h2>
        <div className="table-wrap">
          <table className="data-table">
            <thead>
              <tr>
                <th>Tiêu đề</th>
                <th>Slug</th>
                <th>Trạng thái</th>
                <th></th>
              </tr>
            </thead>
            <tbody>
              {posts.length === 0 ? (
                <tr>
                  <td colSpan={4} className="py-6 text-center text-mu-muted">
                    Chưa có bài viết.
                  </td>
                </tr>
              ) : (
                posts.map((post) => (
                  <tr key={post.id}>
                    <td className="font-medium text-white">{post.title}</td>
                    <td>{post.slug}</td>
                    <td>
                      {post.isPublished ? (
                        <span className="badge-lime">Published</span>
                      ) : (
                        <span className="badge-gold">Draft</span>
                      )}
                    </td>
                    <td>
                      <button type="button" className="btn-ghost px-2 py-1 text-xs" onClick={() => edit(post)}>
                        Sửa
                      </button>
                    </td>
                  </tr>
                ))
              )}
            </tbody>
          </table>
        </div>
      </div>
    </div>
  );
}
