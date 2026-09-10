import { NextResponse } from "next/server";
import { requireUser } from "@/lib/auth";
import { changePassword } from "@/lib/game";
import { passwordSchema } from "@/lib/validators";
import { z } from "zod";

export const dynamic = "force-dynamic";

const schema = z
  .object({
    oldPassword: passwordSchema,
    newPassword: passwordSchema,
    confirmPassword: z.string(),
  })
  .refine((v) => v.newPassword === v.confirmPassword, {
    message: "Mật khẩu mới xác nhận không khớp",
    path: ["confirmPassword"],
  });

export async function POST(req: Request) {
  try {
    const session = await requireUser();
    const body = await req.json();
    const parsed = schema.safeParse(body);
    if (!parsed.success) {
      const msg = parsed.error.issues[0]?.message || "Dữ liệu không hợp lệ";
      return NextResponse.json({ ok: false, error: msg }, { status: 400 });
    }

    await changePassword(
      session.user.id,
      parsed.data.oldPassword,
      parsed.data.newPassword
    );

    return NextResponse.json({ ok: true, message: "Đổi mật khẩu thành công" });
  } catch (e) {
    if (e instanceof Error && e.message === "UNAUTHORIZED_USER") {
      return NextResponse.json(
        { ok: false, error: "Bạn cần đăng nhập tài khoản game" },
        { status: 401 }
      );
    }
    const message = e instanceof Error ? e.message : "Đổi mật khẩu thất bại";
    return NextResponse.json({ ok: false, error: message }, { status: 400 });
  }
}
