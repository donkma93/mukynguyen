# Security checklist — trước khi public WebCMS / Server

Tick từng mục trước khi mở domain ra ngoài.

## A. WebCMS env

- [ ] `NEXTAUTH_SECRET` ≥ 32 ký tự ngẫu nhiên (không dùng giá trị mẫu)
- [ ] `NEXTAUTH_URL` = `https://your-domain` (HTTPS)
- [ ] Đổi `ADMIN_BOOTSTRAP_USER` / `ADMIN_BOOTSTRAP_PASS` — **cấm** để `admin` / `admin123`
- [ ] `TURNSTILE_SITE_KEY` + `TURNSTILE_SECRET_KEY` từ Cloudflare Turnstile
- [ ] `NEXT_PUBLIC_TURNSTILE_SITE_KEY` (nếu dùng) trùng site key
- [ ] `ALLOW_DEV_CAPTCHA` **không** bật trên production
- [ ] `.env.local` không commit lên git
- [ ] `NODE_ENV=production` khi `next start`
- [ ] `SECURITY_FAIL_CLOSED=1` trên máy public (từ chối boot nếu thiếu Turnstile / secret yếu / admin mặc định)

## B. Chống bot / abuse

- [ ] Đăng ký bắt buộc Turnstile (mode `turnstile` từ `/api/bot-mode`)
- [ ] Rate-limit register / login lockout / VIP / giftcode đang bật (code mặc định)
- [ ] `npm run test:security` pass trên máy build

## C. Ops / GameServer

- [ ] `GS_OPS_ENABLED=false` trên VPS public (hoặc chỉ VPN/IP admin)
- [ ] Không expose SQL Server (1433), RDP, Share ra Internet
- [ ] Firewall: chỉ 80/443 (web) + port game cần thiết (ví dụ ConnectServer)
- [ ] Admin CMS chỉ truy cập qua HTTPS; cân nhắc IP allowlist / VPN
- [ ] Không chạy `next dev -H 0.0.0.0` trên máy public — dùng `next build` + `next start` sau reverse proxy

## D. HTTPS / proxy

- [ ] Cloudflare / Caddy / Nginx terminate TLS
- [ ] Redirect HTTP → HTTPS
- [ ] Header bảo mật đã có từ Next (`X-Frame-Options`, `nosniff`, `Referrer-Policy`, HSTS khi prod)

## E. Dữ liệu

- [ ] Backup DB trước public
- [ ] Đã chạy migrate VIP một loại (`AccountLevel > 1 → 1`) nếu cần
- [ ] Giftcode / coin admin chỉ dùng tài khoản admin đã đổi mật khẩu

## F. Follow-up (chưa bắt buộc phase-1)

- [ ] Rate-limit Redis nếu chạy nhiều instance
- [ ] CSP nonce đầy đủ
- [ ] Hash mật khẩu game (phá tương thích client — thiết kế riêng)
- [ ] Turnstile trên login admin nếu bị brute-force
