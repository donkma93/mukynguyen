import AccountsPanel from "@/components/admin/AccountsPanel";

export const metadata = { title: "Quản lý tài khoản" };

export default function AdminAccountsPage() {
  return (
    <div className="space-y-6">
      <div>
        <h1 className="section-title">Quản lý tài khoản</h1>
        <p className="muted mt-1">Tìm kiếm, khóa/mở, set VIP và cộng coin.</p>
      </div>
      <AccountsPanel />
    </div>
  );
}
