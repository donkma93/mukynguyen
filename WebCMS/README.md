# MU Kỷ Nguyên WebCMS

Website CMS full-stack cho máy chủ **MU Kỷ Nguyên**.

- Next.js 15 (App Router) + TypeScript + Tailwind
- SQL Server (ODBC DSN `MuThangCuoi` / `msnodesqlv8`) — đổi DSN/DB trên máy khác, không hardcode path disk
- NextAuth Credentials: tài khoản game + admin CMS
- Đăng ký có captcha chống bot tạo tài khoản hàng loạt

## Chạy local

```powershell
cd WebCMS
.\start-web.ps1
```

Hoặc:

```powershell
npm install
npm run dev
```

Mở trình duyệt:

- http://127.0.0.2:3000
- http://localhost:3000

## Tài khoản

| Vai trò | URL | Mặc định |
|---|---|---|
| Người chơi | `/register`, `/login` | Đăng ký mới (pass game plain text, ≤10 ký tự) |
| Admin CMS | `/login?admin=1` | `admin` / `admin123` (đổi trong `.env.local`) |

Admin bootstrap lấy từ:

```
ADMIN_BOOTSTRAP_USER=admin
ADMIN_BOOTSTRAP_PASS=admin123
```

## Tính năng

**Public**

- Trang chủ, tin tức / sự kiện
- Bảng xếp hạng Reset / Master / Level / Zen / Kill
- Download + IP kết nối `127.0.0.2:44405`

**Panel người chơi** (`/panel`)

- Xem WCoin / VIP / nhân vật
- Đổi mật khẩu game
- Nhập giftcode

**Admin** (`/admin`)

- Quản lý account (khóa, VIP, cộng coin)
- Quản lý nhân vật + **Give Item** (set đồ vào túi, chỉ khi Offline)

- **Vận hành GS** — status process + Restart GameServer / full stack (`/admin/ops`)
- **GS INI editor** — Common / ChaosMix / Character / Event… (`/admin/gs/ini`)
- CRUD tin tức
- CRUD giftcode

Lưu INI sẽ tạo backup `.bak.<timestamp>` rồi ghi file. Áp dụng bằng **Restart GS** (Phase 0).

## Cấu hình `.env.local`

Xem mẫu đầy đủ: [`.env.example`](./.env.example).

```env
MSSQL_CONNECTION_STRING=DSN=MuThangCuoi;
NEXTAUTH_SECRET=change-me-to-a-long-random-string-32+
NEXTAUTH_URL=http://127.0.0.2:3000
ADMIN_BOOTSTRAP_USER=admin
ADMIN_BOOTSTRAP_PASS=change-this-password
TURNSTILE_SITE_KEY=
TURNSTILE_SECRET_KEY=
ALLOW_DEV_CAPTCHA=1
NEXT_PUBLIC_SERVER_IP=127.0.0.2
NEXT_PUBLIC_SERVER_PORT=44405
# Optional — auto-detects sibling "Mu Server" if unset
# MU_SERVER_ROOT=
GS_OPS_ENABLED=false
```

### Cloudflare Turnstile (chống bot đăng ký)

1. Tạo widget tại [Cloudflare Turnstile](https://dash.cloudflare.com/?to=/:account/turnstile).
2. Điền `TURNSTILE_SITE_KEY` + `TURNSTILE_SECRET_KEY`.
3. Production **bắt buộc** Turnstile; local có thể dùng math captcha khi `ALLOW_DEV_CAPTCHA=1`.

## Bảo mật & test

```powershell
npm run test:security
# Khi web đang chạy + DB:
$env:RUN_INTEGRATION=1; $env:BASE_URL="http://localhost:3000"; npm run test:integration
```

Checklist trước khi public: [`scripts/security-checklist.md`](./scripts/security-checklist.md).

## Launcher

`Client/Data/Launcher/index.html` đã trỏ:

- Trang Chủ → `http://127.0.0.2:3000/`
- Đăng Ký → `http://127.0.0.2:3000/register`

## Lưu ý

- Password game hiện **plain text** (`MD5Encryption=0`). CMS khớp cấu hình này.
- Cần ODBC User DSN tên `MuThangCuoi` trỏ tới SQL Server + database `MuThangCuoi`. `Mu Server\start-all.ps1` tự tạo DSN `.\SQLEXPRESS`. Máy khác chỉ cần restore DB và sửa DSN nếu SQL không phải SQLEXPRESS.
- `127.0.0.2` là loopback (chạy offline trên mọi máy Windows), không phải IP máy này.
- Trước khi public: đổi admin mặc định, bật Turnstile, tắt `GS_OPS_ENABLED` nếu không cần, dùng HTTPS.

Credit: **DONPV / MU Kỷ Nguyên**

