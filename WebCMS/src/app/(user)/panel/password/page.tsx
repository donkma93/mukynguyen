import { redirect } from "next/navigation";
import { getServerAuthSession } from "@/lib/auth";
import PasswordForm from "@/components/panel/PasswordForm";

export const metadata = { title: "Đổi mật khẩu" };

export default async function PasswordPage() {
  const session = await getServerAuthSession();
  if (!session?.user) redirect("/login");

  return (
    <div className="space-y-6">
      <div>
        <h1 className="section-title">Đổi mật khẩu</h1>
        <p className="muted mt-1">Mật khẩu game cũng dùng để đăng nhập WebCMS.</p>
      </div>
      <div className="card-glow max-w-xl p-5 md:p-6">
        <PasswordForm />
      </div>
    </div>
  );
}
