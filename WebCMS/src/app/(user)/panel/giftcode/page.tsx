import { redirect } from "next/navigation";
import GiftcodeForm from "@/components/panel/GiftcodeForm";
import PartnerClaimForm from "@/components/panel/PartnerClaimForm";
import { getServerAuthSession } from "@/lib/auth";
import { getCharacters } from "@/lib/game";

export const dynamic = "force-dynamic";
export const metadata = { title: "Giftcode" };

export default async function GiftcodePage() {
  const session = await getServerAuthSession();
  if (!session?.user?.id) redirect("/login");

  let characters: Awaited<ReturnType<typeof getCharacters>> = [];
  try {
    characters = await getCharacters(session.user.id);
  } catch {
    characters = [];
  }

  return (
    <div className="space-y-6">
      <div>
        <h1 className="section-title">Nhập Giftcode</h1>
        <p className="muted mt-1">Nhận phần thưởng sự kiện hoặc quà đối tác.</p>
      </div>
      <div className="grid gap-4 lg:grid-cols-2">
        <div className="card-glow p-5 md:p-6">
          <h2 className="panel-title mb-3">Giftcode sự kiện</h2>
          <GiftcodeForm />
        </div>
        <div className="card p-5 md:p-6">
          <h2 className="panel-title mb-1">Quà đối tác</h2>
          <p className="muted mb-4 text-sm">
            Nhập mã LIVE/NEWBIE từ streamer. Coin cộng ngay; ngọc/bùa vào nhân vật khi vào game.
          </p>
          <PartnerClaimForm characters={characters} />
        </div>
      </div>
    </div>
  );
}
