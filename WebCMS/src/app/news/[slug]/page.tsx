import Link from "next/link";
import { notFound } from "next/navigation";
import { getPostBySlug } from "@/lib/cms";

export const dynamic = "force-dynamic";

type Props = { params: Promise<{ slug: string }> };

export async function generateMetadata({ params }: Props) {
  const { slug } = await params;
  try {
    const post = await getPostBySlug(slug);
    return { title: post?.title ?? "Tin tức" };
  } catch {
    return { title: "Tin tức" };
  }
}

export default async function NewsDetailPage({ params }: Props) {
  const { slug } = await params;
  let post: Awaited<ReturnType<typeof getPostBySlug>> | null = null;
  try {
    post = await getPostBySlug(slug);
  } catch {
    post = null;
  }

  if (!post || !post.isPublished) notFound();

  const dateValue = post.publishedAt ?? post.createdAt;
  const date = dateValue
    ? new Date(dateValue).toLocaleString("vi-VN")
    : null;

  return (
    <article className="mx-auto max-w-3xl px-4 py-10">
      <Link href="/news" className="text-sm text-mu-gold hover:underline">
        ← Quay lại tin tức
      </Link>
      <header className="mt-4 card-glow p-6">
        <span className="badge-gold">Tin tức</span>
        <h1 className="mt-3 font-display text-3xl font-bold text-white">{post.title}</h1>
        {date ? <p className="muted mt-2">{date}</p> : null}
      </header>
      <div className="card mt-6 whitespace-pre-wrap p-6 leading-relaxed text-gray-200">
        {post.content}
      </div>
    </article>
  );
}
