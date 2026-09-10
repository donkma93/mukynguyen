import CharactersPanel from "@/components/admin/CharactersPanel";

export const dynamic = "force-dynamic";
export const metadata = { title: "Nhân vật" };

export default function AdminCharactersPage() {
  return (
    <div className="space-y-6">
      <div>
        <h1 className="section-title">Quản lý nhân vật</h1>
        <p className="muted mt-1">
          Cập nhật Class, danh hiệu/VIP/quan hàm, Level, Reset, Zen, Map và bộ chỉ số.
        </p>
      </div>
      <CharactersPanel />
    </div>
  );
}
