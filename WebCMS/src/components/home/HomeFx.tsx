"use client";

import { useMemo } from "react";

type Spark = {
  id: number;
  left: string;
  top: string;
  size: number;
  delay: string;
  duration: string;
  tone: "gold" | "lime" | "purple";
};

const toneClass = {
  gold: "bg-mu-gold/80 shadow-[0_0_12px_rgba(212,175,55,0.8)]",
  lime: "bg-mu-lime/80 shadow-[0_0_12px_rgba(120,255,20,0.7)]",
  purple: "bg-purple-300/80 shadow-[0_0_12px_rgba(168,85,247,0.75)]",
};

export default function HomeFx({ count = 28 }: { count?: number }) {
  const sparks = useMemo<Spark[]>(() => {
    return Array.from({ length: count }, (_, i) => {
      const tone =
        i % 3 === 0 ? "gold" : i % 3 === 1 ? "lime" : ("purple" as const);
      return {
        id: i,
        left: `${(i * 37) % 100}%`,
        top: `${(i * 53) % 100}%`,
        size: 2 + (i % 4),
        delay: `${(i % 8) * 0.35}s`,
        duration: `${2.8 + (i % 5) * 0.45}s`,
        tone,
      };
    });
  }, [count]);

  return (
    <div
      aria-hidden
      className="pointer-events-none absolute inset-0 overflow-hidden"
    >
      <div className="mu-aurora absolute inset-0 opacity-90" />
      <div className="mu-grid-fade absolute inset-0 opacity-40" />
      {sparks.map((s) => (
        <span
          key={s.id}
          className={`mu-spark absolute rounded-full ${toneClass[s.tone]}`}
          style={{
            left: s.left,
            top: s.top,
            width: s.size,
            height: s.size,
            animationDelay: s.delay,
            animationDuration: s.duration,
          }}
        />
      ))}
      <div className="absolute -left-24 top-1/4 h-72 w-72 rounded-full bg-mu-gold/10 blur-3xl" />
      <div className="absolute -right-16 top-10 h-80 w-80 rounded-full bg-mu-purple/25 blur-3xl" />
      <div className="absolute bottom-0 left-1/3 h-64 w-64 rounded-full bg-mu-lime/10 blur-3xl" />
    </div>
  );
}
