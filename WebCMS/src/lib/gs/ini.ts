export type IniLine =
  | { kind: "blank"; raw: string }
  | { kind: "comment"; raw: string; text: string }
  | { kind: "section"; raw: string; name: string }
  | { kind: "key"; raw: string; key: string; value: string; spacing: string };

export type IniDocument = {
  lines: IniLine[];
  /** First occurrence wins for lookup helpers. */
  values: Record<string, string>;
};

export function parseIni(text: string): IniDocument {
  const lines: IniLine[] = [];
  const values: Record<string, string> = {};
  const rawLines = text.replace(/^\uFEFF/, "").split(/\r?\n/);

  for (const raw of rawLines) {
    const trimmed = raw.trim();
    if (!trimmed) {
      lines.push({ kind: "blank", raw });
      continue;
    }
    if (trimmed.startsWith(";") || trimmed.startsWith("#")) {
      lines.push({ kind: "comment", raw, text: trimmed.slice(1).trim() });
      continue;
    }
    const sectionMatch = trimmed.match(/^\[(.+)]$/);
    if (sectionMatch) {
      lines.push({ kind: "section", raw, name: sectionMatch[1].trim() });
      continue;
    }
    const eq = raw.indexOf("=");
    if (eq === -1) {
      lines.push({ kind: "comment", raw, text: trimmed });
      continue;
    }
    const key = raw.slice(0, eq).trim();
    const afterEq = raw.slice(eq + 1);
    const value = afterEq.trim();
    const spacingMatch = raw.slice(0, eq).match(/(\s*)$/);
    const spacing = spacingMatch?.[1] ?? " ";
    lines.push({ kind: "key", raw, key, value, spacing: spacing || " " });
    if (!(key in values)) values[key] = value;
  }

  return { lines, values };
}

export function serializeIni(doc: IniDocument): string {
  return doc.lines.map((l) => l.raw).join("\r\n") + (doc.lines.length ? "\r\n" : "");
}

/** Split value into core + trailing `//` or `;` comment (with leading whitespace). */
function splitValueAndComment(value: string): { core: string; suffix: string } {
  const match = value.match(/^(.*?)(\s*(?:\/\/|;).*)$/);
  if (match) {
    return { core: match[1].trimEnd(), suffix: match[2] };
  }
  return { core: value.trim(), suffix: "" };
}

/**
 * Apply key→value updates while preserving comments/sections/order.
 * When replacing a value that had a trailing `//` or `;` comment, the comment is kept
 * and only the numeric/text core before the comment is replaced.
 * Unknown keys are appended under the last section (or end of file).
 */
export function applyIniUpdates(
  text: string,
  updates: Record<string, string>
): { text: string; changed: string[]; missingCreated: string[] } {
  const doc = parseIni(text);
  const pending = { ...updates };
  const changed: string[] = [];
  const missingCreated: string[] = [];

  for (const line of doc.lines) {
    if (line.kind !== "key") continue;
    if (!(line.key in pending)) continue;
    const nextCore = String(pending[line.key]).trim();
    const { core: prevCore, suffix } = splitValueAndComment(line.value);
    if (prevCore !== nextCore) {
      const nextValue = suffix ? `${nextCore}${suffix}` : nextCore;
      line.value = nextValue;
      line.raw = `${line.key}${line.spacing}= ${nextValue}`;
      changed.push(line.key);
    }
    delete pending[line.key];
  }

  const remaining = Object.keys(pending);
  if (remaining.length) {
    let lastSectionIdx = -1;
    for (let i = 0; i < doc.lines.length; i++) {
      if (doc.lines[i].kind === "section") lastSectionIdx = i;
    }
    const insertAt = lastSectionIdx >= 0 ? lastSectionIdx + 1 : doc.lines.length;
    const extra: IniLine[] = [];
    for (const key of remaining) {
      const value = String(pending[key]);
      extra.push({
        kind: "key",
        raw: `${key} = ${value}`,
        key,
        value,
        spacing: " ",
      });
      missingCreated.push(key);
      changed.push(key);
    }
    doc.lines.splice(insertAt, 0, ...extra);
  }

  // rebuild values map
  doc.values = {};
  for (const line of doc.lines) {
    if (line.kind === "key" && !(line.key in doc.values)) {
      doc.values[line.key] = line.value;
    }
  }

  return { text: serializeIni(doc), changed, missingCreated };
}

export function listIniEntries(text: string): { key: string; value: string; section: string }[] {
  const doc = parseIni(text);
  let section = "";
  const out: { key: string; value: string; section: string }[] = [];
  for (const line of doc.lines) {
    if (line.kind === "section") {
      section = line.name;
      continue;
    }
    if (line.kind === "key") {
      out.push({ key: line.key, value: line.value, section });
    }
  }
  return out;
}
