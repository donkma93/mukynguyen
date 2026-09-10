import path from "path";
import { defineConfig } from "vitest/config";

export default defineConfig({
  test: {
    environment: "node",
    setupFiles: ["./src/test/setup.ts"],
    include: ["src/test/**/*.test.ts"],
    exclude: [
      "node_modules",
      // Integration only when RUN_INTEGRATION=1
      ...(process.env.RUN_INTEGRATION === "1" ? [] : ["src/test/integration/**"]),
    ],
    testTimeout: 15000,
  },
  resolve: {
    alias: {
      "@": path.resolve(__dirname, "./src"),
    },
  },
});
