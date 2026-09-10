import { describe, expect, it } from "vitest";
import { resolveAllowedPath } from "@/lib/gs/files";
import { getGsDataRoot, getGsIniRoot, getMuServerRoot, gsIniSlugToFile } from "@/lib/gs/paths";
import { clientSafeError, publicFileName } from "@/lib/security/client-error";

describe("GS path security", () => {
  it("gsIniSlugToFile only maps allowlisted slugs", () => {
    expect(gsIniSlugToFile("common")).toContain("Common.ini");
    expect(gsIniSlugToFile("../../etc/passwd")).toBeNull();
    expect(gsIniSlugToFile("Common.ini")).toBeNull();
    expect(gsIniSlugToFile("..")).toBeNull();
    expect(gsIniSlugToFile("")).toBeNull();
    expect(gsIniSlugToFile("character")).toContain("Character.ini");
  });

  it("resolveAllowedPath rejects traversal and NUL", () => {
    expect(() => resolveAllowedPath("ini", "../secrets")).toThrow();
    expect(() => resolveAllowedPath("ini", "..\\secrets")).toThrow();
    expect(() => resolveAllowedPath("ini", "foo\0bar")).toThrow();
    expect(() => resolveAllowedPath("data", "")).toThrow();
    // Relative file under root is allowed (may not exist)
    const full = resolveAllowedPath("ini", "GameServerInfo - Common.ini");
    expect(full.replace(/\\/g, "/")).toMatch(/GameServerInfo - Common\.ini$/);
  });

  it("discovers Mu Server without a hardcoded drive path", () => {
    const root = getMuServerRoot().replace(/\\/g, "/");
    expect(root).not.toMatch(/SRC ThangCuoi\/SRC ThangCuoi/i);
    expect(root.endsWith("/Mu Server") || root.toLowerCase().endsWith("/mu server")).toBe(
      true
    );
    expect(getGsIniRoot().replace(/\\/g, "/")).toMatch(/GameServer\/Data$/);
    expect(getGsDataRoot().replace(/\\/g, "/")).toMatch(/4\.Sub-1\/Data$/);
  });

  it("strips host filesystem paths from client errors", () => {
    expect(
      clientSafeError(
        new Error(
          "ENOENT: no such file or directory, open 'E:\\\\SRC ThangCuoi\\\\Mu Server\\\\4.Sub-1\\\\GameServer\\\\Data\\\\GameServerInfo - Character.ini'"
        ),
        "Không đọc được INI"
      )
    ).toBe("Không đọc được INI");
    expect(clientSafeError(new Error("INI không hợp lệ"), "fallback")).toBe(
      "INI không hợp lệ"
    );
    expect(publicFileName("E:\\\\SRC ThangCuoi\\\\Mu Server\\\\BlackList.txt")).toBe(
      "BlackList.txt"
    );
  });
});
