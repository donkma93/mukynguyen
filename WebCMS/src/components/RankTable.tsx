import { className, type RankRow } from "@/lib/game";
import type { Dictionary } from "@/lib/i18n/get-dictionary";

type Props = {
  rows: RankRow[];
  type: "reset" | "master" | "level" | "zen" | "kill";
  labels: Dictionary["ranking"];
  numberLocale?: string;
};

function metric(row: RankRow, type: Props["type"], numberLocale: string) {
  switch (type) {
    case "reset":
      return row.ResetCount;
    case "master":
      return row.MasterResetCount;
    case "level":
      return row.cLevel;
    case "zen":
      return row.Money.toLocaleString(numberLocale);
    case "kill":
      return row.Kills;
  }
}

export default function RankTable({
  rows,
  type,
  labels,
  numberLocale = "en-US",
}: Props) {
  const localeMap = {
    reset: labels.tabs.reset,
    master: labels.tabs.master,
    level: labels.tabs.level,
    zen: labels.tabs.zen,
    kill: labels.tabs.kill,
  };

  return (
    <div className="table-wrap">
      <table className="data-table">
        <thead>
          <tr>
            <th>#</th>
            <th>{labels.character}</th>
            <th>{labels.class}</th>
            <th>{localeMap[type]}</th>
            <th>{labels.level}</th>
            <th>{labels.reset}</th>
          </tr>
        </thead>
        <tbody>
          {rows.length === 0 ? (
            <tr>
              <td colSpan={6} className="py-8 text-center text-mu-muted">
                {labels.empty}
              </td>
            </tr>
          ) : (
            rows.map((row, idx) => (
              <tr key={`${row.AccountID}-${row.Name}`}>
                <td className="font-semibold text-mu-gold">{idx + 1}</td>
                <td className="font-medium text-white">{row.Name}</td>
                <td>{className(row.Class)}</td>
                <td className="text-mu-lime">
                  {metric(row, type, numberLocale)}
                </td>
                <td>{row.cLevel}</td>
                <td>{row.ResetCount}</td>
              </tr>
            ))
          )}
        </tbody>
      </table>
    </div>
  );
}
