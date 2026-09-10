import { redirect } from "next/navigation";
import { getServerAuthSession } from "@/lib/auth";
import GiftcodeForm from "@/components/panel/GiftcodeForm";

export const metadata = { title: "Giftcode" };

export default async function GiftcodePage() {
  const session = await getServerAuthSession();
  if (!session?.user) redirect("/login");

  return (
    <div className="space-y-6">
      <div>
        <h1 className="section-title">Nhập Giftcode</h1>
        <p className="muted mt-1">Nhận phần thưởng sự kiện bằng mã giftcode.</p>
      </div>
      <div className="card-glow max-w-xl p-5 md:p-6">
        <GiftcodeForm />
      </div>
    </div>
  );
}
