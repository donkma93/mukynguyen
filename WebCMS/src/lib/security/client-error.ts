const PATH_LEAK =
  /(?:[A-Za-z]:[\\/]|\\\\|\/(?:home|Users|var|opt|mnt|root|tmp)\/|ENOENT|EACCES|EPERM|EISDIR|ENOTDIR|no such file|not a directory)/i;

/** Never send Node/fs messages (they include host filesystem paths) to the browser. */
export function clientSafeError(error: unknown, fallback: string): string {
  if (!(error instanceof Error)) return fallback;
  const message = error.message?.trim();
  if (!message) return fallback;
  if (PATH_LEAK.test(message)) return fallback;
  return message;
}

export function publicFileName(filePath: string): string {
  const normalized = filePath.replace(/\\/g, "/");
  const base = normalized.split("/").pop() || "";
  return base;
}
