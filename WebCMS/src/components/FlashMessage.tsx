"use client";

import { useEffect, useState } from "react";

type Props = {
  type?: "success" | "error" | "info";
  message?: string | null;
};

export default function FlashMessage({ type = "info", message }: Props) {
  const [visible, setVisible] = useState(Boolean(message));

  useEffect(() => {
    setVisible(Boolean(message));
    if (!message) return;
    const t = setTimeout(() => setVisible(false), 5000);
    return () => clearTimeout(t);
  }, [message]);

  if (!message || !visible) return null;

  const styles =
    type === "success"
      ? "border-mu-lime/40 bg-mu-lime/10 text-mu-lime"
      : type === "error"
        ? "border-mu-danger/50 bg-mu-danger/15 text-red-200"
        : "border-mu-gold/40 bg-mu-gold/10 text-mu-gold";

  return (
    <div className={`mb-4 rounded-lg border px-4 py-3 text-sm ${styles}`} role="alert">
      {message}
    </div>
  );
}
