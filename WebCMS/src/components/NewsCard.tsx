import Link from "next/link";

export type NewsItem = {
  slug: string;
  title: string;
  excerpt?: string | null;
  publishedAt?: string | Date | null;
  createdAt?: string | Date | null;
};

function formatDate(value?: string | Date | null) {
  if (!value) return "";
  const d = typeof value === "string" ? new Date(value) : value;
  if (Number.isNaN(d.getTime())) return "";
  return d.toLocaleDateString("vi-VN");
}

export default function NewsCard({ post }: { post: NewsItem }) {
  const date = formatDate(post.publishedAt ?? post.createdAt);
  return (
    <Link href={`/news/${post.slug}`} className="card-glow group block p-5 transition hover:border-mu-gold/50">
      <div className="mb-2 flex items-center justify-between gap-3">
        <span className="badge-gold">Tin tức</span>
        {date ? <time className="muted">{date}</time> : null}
      </div>
      <h3 className="font-display text-lg font-semibold text-gray-100 group-hover:text-mu-gold">
        {post.title}
      </h3>
      {post.excerpt ? <p className="mt-2 line-clamp-3 text-sm text-mu-muted">{post.excerpt}</p> : null}
      <span className="mt-4 inline-block text-sm font-medium text-mu-lime">Đọc tiếp →</span>
    </Link>
  );
}
