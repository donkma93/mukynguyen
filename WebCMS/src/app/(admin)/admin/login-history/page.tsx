import Link from "next/link";
import { listAdminLoginActivity } from "@/lib/login-activity";
import { publicIp } from "@/lib/security/client-ip";

export const dynamic = "force-dynamic";
export const metadata = { title: "Lịch sử đăng nhập" };

type Props = {
  searchParams?: Promise<{ q?: string }>;
};

function checkHostUrl(ip: string) {
  return `https://check-host.net/ip-info?host=${encodeURIComponent(ip)}`;
}

export default async function AdminLoginHistoryPage({ searchParams }: Props) {
  const params = (await searchParams) ?? {};
  const q = (params.q || "").trim().slice(0, 10);
  let entries: Awaited<ReturnType<typeof listAdminLoginActivity>> = [];
  let unavailable = false;

  try {
    entries = await listAdminLoginActivity(q);
  } catch {
    unavailable = true;
  }

  return (
    <div className="space-y-6">
      <div>
        <h1 className="section-title">Lịch sử đăng nhập tài khoản</h1>
        <p className="muted mt-1">
          Theo dõi IP public, thiết bị và lần xác thực gần nhất trong Game / Website (30 ngày).
        </p>
      </div>

      <form action="/admin/login-history" className="card flex flex-wrap gap-3 p-4">
        <input
          name="q"
          defaultValue={q}
          className="input max-w-sm"
          placeholder="Tìm tài khoản..."
          maxLength={10}
        />
        <button type="submit" className="btn-gold">Tìm kiếm</button>
        {q && (
          <Link href="/admin/login-history" className="btn-ghost">
            Xóa lọc
          </Link>
        )}
      </form>

      <div className="overflow-hidden rounded-sm border border-white/5 bg-[#07021c] shadow-[0_12px_36px_rgba(0,0,0,0.28)]">
        {unavailable ? (
          <p className="p-5 text-sm text-red-300">Không thể tải lịch sử đăng nhập.</p>
        ) : (
          <div className="overflow-x-auto">
            <table className="w-full min-w-[960px] text-left text-sm">
              <thead className="bg-[#24203f] text-white">
                <tr className="border-b border-[#544d78]">
                  <th className="px-4 py-2.5 font-bold uppercase">Tài khoản</th>
                  <th className="px-4 py-2.5 font-bold uppercase">IP</th>
                  <th className="px-4 py-2.5 font-bold">
                    <span className="block uppercase">Game</span>
                    <span className="mt-1 block text-xs font-semibold normal-case text-white/95">Xác thực gần nhất</span>
                  </th>
                  <th className="px-4 py-2.5 font-bold">
                    <span className="block uppercase">Website</span>
                    <span className="mt-1 block text-xs font-semibold normal-case text-white/95">Xác thực gần nhất</span>
                  </th>
                </tr>
              </thead>
              <tbody>
                {entries.length === 0 ? (
                  <tr>
                    <td colSpan={4} className="px-4 py-8 text-center text-mu-muted">
                      Không có lịch sử đăng nhập trong 30 ngày gần đây.
                    </td>
                  </tr>
                ) : (
                  entries.map((entry) => {
                    const websiteIp = publicIp(entry.ipAddress);
                    const gameIp = publicIp(entry.gameIpAddress);
                    const ip = websiteIp || gameIp;
                    return (
                      <tr key={entry.id} className="align-top border-b border-white/5 text-white/95 last:border-0">
                        <td className="px-4 py-3 font-mono font-semibold text-mu-gold">{entry.account}</td>
                        <td className="px-4 py-3 font-mono font-semibold">
                          {ip ? (
                            <a
                              href={checkHostUrl(ip)}
                              target="_blank"
                              rel="noopener noreferrer"
                              className="text-mu-lime underline decoration-mu-lime/50 underline-offset-4 hover:text-white"
                            >
                              {ip} <span className="font-sans text-xs">Kiểm tra vị trí</span>
                            </a>
                          ) : (
                            <span className="font-sans text-mu-muted">Chưa có IP công khai</span>
                          )}
                        </td>
                        <td className="px-4 py-3">
                          <p className="font-mono font-semibold">
                            {entry.gameConnectedAt
                              ? new Date(entry.gameConnectedAt).toLocaleString("vi-VN")
                              : "Chưa có dữ liệu"}
                          </p>
                          <p className="mt-1 text-xs font-semibold text-white/95">
                            {entry.gameServerName || "Chưa có đăng nhập game"}
                          </p>
                        </td>
                        <td className="px-4 py-3">
                          <p className="font-mono font-semibold">
                            {new Date(entry.loggedInAt).toLocaleString("vi-VN")}
                          </p>
                          <p className="mt-1 text-xs font-semibold text-white/95">{entry.device}</p>
                        </td>
                      </tr>
                    );
                  })
                )}
              </tbody>
            </table>
          </div>
        )}
      </div>
    </div>
  );
}
