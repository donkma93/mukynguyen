import IniEditorPanel from "@/components/admin/IniEditorPanel";

export const metadata = { title: "Cấu hình GameServer" };

type Props = {
  searchParams?: Promise<{ slug?: string }>;
};

export default async function AdminGsIniPage({ searchParams }: Props) {
  const params = (await searchParams) ?? {};
  const initialSlug = (params.slug || "chaosmix").toLowerCase();

  return (
    <div className="space-y-6">
      <div>
        <h1 className="section-title">Cấu hình GameServer</h1>
        <p className="muted mt-1">
          Khu vực nâng cao để chỉnh sự kiện, kỹ năng và quy tắc máy chủ. Nếu cần đổi
          tỉ lệ ép đồ, hãy dùng mục Tỉ lệ đập đồ để thao tác đơn giản hơn.
        </p>
      </div>
      <IniEditorPanel initialSlug={initialSlug} />
    </div>
  );
}
