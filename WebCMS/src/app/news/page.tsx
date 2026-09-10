import NewsCard from "@/components/NewsCard";
import { listPublishedPosts } from "@/lib/cms";

export const dynamic = "force-dynamic";
export const metadata = { title: "Tin tức" };

export default async function NewsPage() {
  let posts: Awaited<ReturnType<typeof listPublishedPosts>> = [];
  try {
    posts = await listPublishedPosts(50);
  } catch {
    posts = [];
  }

  return (
    <div className="mx-auto max-w-6xl px-4 py-10">
      <h1 className="section-title">Tin tức</h1>
      <p className="muted mt-2 mb-8">Thông báo chính thức từ Ban Quản Trị MU Kỷ Nguyên.</p>

      {posts.length === 0 ? (
        <div className="card p-8 text-center text-mu-muted">Chưa có tin tức.</div>
      ) : (
        <div className="grid gap-4 md:grid-cols-2 lg:grid-cols-3">
          {posts.map((post) => (
            <NewsCard key={post.slug} post={post} />
          ))}
        </div>
      )}
    </div>
  );
}
