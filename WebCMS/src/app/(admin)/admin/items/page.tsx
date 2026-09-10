import ItemDeliveryPanel from "@/components/admin/ItemDeliveryPanel";

export const dynamic = "force-dynamic";
export const metadata = { title: "GS - Đẩy đồ" };

export default function AdminItemsPage() {
  return (
    <div className="space-y-6">
      <div>
        <h1 className="section-title">GS — Đẩy đồ nhân vật</h1>
        <p className="muted mt-1">Tạo item đúng chuẩn qua GameServer, theo dõi lệnh chờ và hủy trước khi giao.</p>
      </div>
      <ItemDeliveryPanel />
    </div>
  );
}
