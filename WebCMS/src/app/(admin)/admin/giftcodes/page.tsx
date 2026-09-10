import GiftcodesPanel from "@/components/admin/GiftcodesPanel";

export const metadata = { title: "Giftcode Admin" };

export default function AdminGiftcodesPage() {
  return (
    <div className="space-y-6">
      <div>
        <h1 className="section-title">Quản lý Giftcode</h1>
        <p className="muted mt-1">Tạo mã sự kiện và theo dõi lượt dùng.</p>
      </div>
      <GiftcodesPanel />
    </div>
  );
}
