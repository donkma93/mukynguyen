import { NextResponse } from "next/server";
import {
  BALANCE_CLASS_CODES,
  readBalancingFromIniText,
  type BalanceClassCode,
  type BalancingMatrix,
} from "@/lib/gs/balancing";
import { readTextFile } from "@/lib/gs/files";

export const dynamic = "force-dynamic";

type PublicBalancing = {
  matrix: BalancingMatrix;
  pvp: Record<BalanceClassCode, number | null>;
  pvm: Record<BalanceClassCode, number | null>;
};

function toPublicDto(rawText: string): PublicBalancing {
  const parsed = readBalancingFromIniText(rawText);
  const matrix = {} as BalancingMatrix;
  const pvp = {} as Record<BalanceClassCode, number | null>;
  const pvm = {} as Record<BalanceClassCode, number | null>;

  for (const atk of BALANCE_CLASS_CODES) {
    matrix[atk] = {} as Record<BalanceClassCode, number | null>;
    pvp[atk] = parsed.pvp[atk];
    pvm[atk] = parsed.pvm[atk];
    for (const def of BALANCE_CLASS_CODES) {
      matrix[atk][def] = parsed.matrix[atk][def];
    }
  }

  return { matrix, pvp, pvm };
}

export async function GET() {
  try {
    const rawText = await readTextFile("ini", "GameServerInfo - Character.ini");
    const data = toPublicDto(rawText);
    return NextResponse.json({ ok: true, data });
  } catch {
    return NextResponse.json(
      { ok: false, error: "unavailable" },
      { status: 503 }
    );
  }
}
