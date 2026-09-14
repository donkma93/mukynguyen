import { z } from "zod";

export const accountSchema = z
  .string()
  .trim()
  .min(4, "Tài khoản tối thiểu 4 ký tự")
  .max(10, "Tài khoản tối đa 10 ký tự")
  .regex(/^[a-zA-Z0-9]+$/, "Chỉ dùng chữ và số");

export const passwordSchema = z
  .string()
  .min(4, "Mật khẩu tối thiểu 4 ký tự")
  .max(10, "Mật khẩu tối đa 10 ký tự");

export const registerSchema = z
  .object({
    account: accountSchema,
    password: passwordSchema,
    confirmPassword: z.string(),
    email: z.string().email("Email không hợp lệ").optional().or(z.literal("")),
    /** Dev math captcha answer (only when Turnstile not configured). */
    captcha: z.string().trim().optional().or(z.literal("")),
    /** Cloudflare Turnstile token (production / when configured). */
    turnstileToken: z.string().trim().optional().or(z.literal("")),
  })
  .refine((v) => v.password === v.confirmPassword, {
    message: "Mật khẩu xác nhận không khớp",
    path: ["confirmPassword"],
  });

export const loginSchema = z.object({
  account: accountSchema,
  password: passwordSchema,
});

export const giftcodeSchema = z.object({
  code: z.string().trim().min(3).max(32),
});

export const characterNameSchema = z
  .string()
  .trim()
  .min(1, "Thiếu tên nhân vật")
  .max(10, "Tên nhân vật tối đa 10 ký tự");

export const partnerClaimSchema = z.object({
  code: z.string().trim().min(3).max(32),
  characterName: characterNameSchema,
});

export const partnerHighlightSchema = z.object({
  targetAccount: accountSchema,
  characterName: characterNameSchema,
});

export const partnerUpsertSchema = z.object({
  account: accountSchema,
  tier: z.enum(["new", "stable", "top"]),
  isActive: z.boolean().optional(),
});
