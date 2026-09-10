import BalancingEditorPanel from "@/components/admin/BalancingEditorPanel";

export const metadata = { title: "Class Balancing" };

export default function AdminGsBalancingPage() {
  return (
    <div className="space-y-6">
      <div>
        <h1 className="section-title">Cân bằng class</h1>
        <p className="muted mt-1">
          Sửa ma trận dame class × class, PvP và PvM. Lưu sẽ backup file và tự
          Reload Character trên GameServer đang chạy để số trong game khớp ngay
          với web.
        </p>
      </div>
      <BalancingEditorPanel />
    </div>
  );
}
