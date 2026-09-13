import MossMerchantPanel from "@/components/admin/MossMerchantPanel";

export const metadata = { title: "Sự kiện Game" };

export default function AdminEventsPage() {
  return (
    <div className="space-y-6">
      <div>
        <h1 className="section-title">Sự kiện Game</h1>
        <p className="muted mt-1">Bật, hẹn giờ và chạy thử sự kiện trực tiếp từ web quản trị.</p>
      </div>
      <MossMerchantPanel />
    </div>
  );
}
