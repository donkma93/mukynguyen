import HackPanel from "@/components/admin/HackPanel";

export const metadata = { title: "Hack Detect" };

export default function AdminHackPage() {
  return (
    <div className="space-y-6">
      <div>
        <h1 className="section-title">Kiểm tra hack</h1>
        <p className="muted mt-1">
          Đọc nhật ký GameServer HACK_LOG và AntiHack, tìm theo tài khoản / IP /
          HWID, xem blacklist.
        </p>
      </div>
      <HackPanel />
    </div>
  );
}
