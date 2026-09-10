import PostsPanel from "@/components/admin/PostsPanel";

export const dynamic = "force-dynamic";
export const metadata = { title: "Quản lý tin tức" };

export default function AdminPostsPage() {
  return (
    <div className="space-y-6">
      <div>
        <h1 className="section-title">Quản lý tin tức</h1>
        <p className="muted mt-1">Tạo và chỉnh sửa bài viết CMS.</p>
      </div>
      <PostsPanel />
    </div>
  );
}
