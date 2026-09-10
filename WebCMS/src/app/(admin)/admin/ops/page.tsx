import OpsPanel from "@/components/admin/OpsPanel";

export const metadata = { title: "Vận hành GS" };

export default function AdminOpsPage() {
  return (
    <div className="space-y-6">
      <div>
        <h1 className="section-title">Vận hành GameServer</h1>
        <p className="muted mt-1">
          Xem tiến trình server và restart sau khi sửa cấu hình.
        </p>
      </div>
      <OpsPanel />
    </div>
  );
}
