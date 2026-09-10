import EnhancementRatesPanel from "@/components/admin/EnhancementRatesPanel";

export const dynamic = "force-dynamic";
export const metadata = { title: "Tỉ lệ đập đồ" };

export default function EnhancementRatesPage() {
  return (
    <div className="space-y-6">
      <div>
        <h1 className="section-title">Tỉ lệ đập đồ</h1>
        <p className="muted mt-1">
          Chỉnh toàn bộ Chaos Mix, tỉ lệ ngọc và Luck theo từng mức tài khoản.
        </p>
      </div>
      <EnhancementRatesPanel />
    </div>
  );
}
