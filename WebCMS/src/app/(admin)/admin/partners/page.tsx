import PartnersPanel from "@/components/admin/PartnersPanel";

export const metadata = { title: "Đối tác Admin" };

export default function AdminPartnersPage() {
  return (
    <div className="space-y-6">
      <div>
        <h1 className="section-title">Quản lý đối tác</h1>
        <p className="muted mt-1">
          Gán hạng partner, bật/tắt quyền phát quà và theo dõi ngân sách tháng.
        </p>
      </div>
      <PartnersPanel />
    </div>
  );
}
