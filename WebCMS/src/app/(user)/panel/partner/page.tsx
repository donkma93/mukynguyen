import { redirect } from "next/navigation";
import PartnerPanel from "@/components/panel/PartnerPanel";
import { getServerAuthSession } from "@/lib/auth";
import { isActivePartner } from "@/lib/partner/partners";

export const dynamic = "force-dynamic";
export const metadata = { title: "Panel đối tác" };

export default async function PartnerPage() {
  const session = await getServerAuthSession();
  if (!session?.user?.id || session.user.role !== "user") redirect("/login");

  const ok = await isActivePartner(session.user.id);
  if (!ok) redirect("/panel");

  return (
    <div className="space-y-6">
      <div>
        <h1 className="section-title">Panel đối tác</h1>
        <p className="muted mt-1">
          Tạo mã Live/Newbie và phát Highlight cho người xem stream trong hạn mức tháng.
        </p>
      </div>
      <PartnerPanel />
    </div>
  );
}
