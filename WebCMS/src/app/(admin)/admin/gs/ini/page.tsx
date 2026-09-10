import IniEditorPanel from "@/components/admin/IniEditorPanel";

export const metadata = { title: "GS INI" };

type Props = {
  searchParams?: Promise<{ slug?: string }>;
};

export default async function AdminGsIniPage({ searchParams }: Props) {
  const params = (await searchParams) ?? {};
  const initialSlug = (params.slug || "chaosmix").toLowerCase();

  return (
    <div className="space-y-6">
      <div>
        <h1 className="section-title">Cấu hình GS (INI)</h1>
        <p className="muted mt-1">
          Sửa GameServerInfo (ChaosMix, Common, Event…). Lưu sẽ backup file; cần
          Restart GS để áp dụng.
        </p>
      </div>
      <IniEditorPanel initialSlug={initialSlug} />
    </div>
  );
}
