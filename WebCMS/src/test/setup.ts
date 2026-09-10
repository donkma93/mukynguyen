import { beforeEach } from "vitest";
import { resetRateLimitStore } from "@/lib/security/rate-limit";

process.env.NEXTAUTH_SECRET =
  process.env.NEXTAUTH_SECRET || "test-nextauth-secret-32chars-min!!";
process.env.NODE_ENV = process.env.NODE_ENV || "test";

beforeEach(() => {
  resetRateLimitStore();
});
