type Props = {
  label: string;
  value: string | number;
  tone?: "gold" | "lime" | "purple" | "danger";
};

const toneClass = {
  gold: "border-mu-gold/35 text-mu-gold",
  lime: "border-mu-lime/35 text-mu-lime",
  purple: "border-mu-purple/45 text-purple-200",
  danger: "border-mu-danger/45 text-red-300",
};

export default function StatBadge({ label, value, tone = "gold" }: Props) {
  return (
    <div className={`card flex flex-col gap-1 px-4 py-3 ${toneClass[tone]}`}>
      <span className="text-[11px] uppercase tracking-wider text-mu-muted">{label}</span>
      <span className="font-display text-2xl font-bold">{value}</span>
    </div>
  );
}
