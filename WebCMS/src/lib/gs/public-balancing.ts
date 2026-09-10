import {
  BALANCE_CLASS_CODES,
  type BalanceClassCode,
  type BalancingMatrix,
} from "@/lib/gs/balancing";

/** Public-facing balancing numbers only — no INI paths or raw config text. */
export type PublicBalancing = {
  matrix: BalancingMatrix;
  pvp: Record<BalanceClassCode, number | null>;
  pvm: Record<BalanceClassCode, number | null>;
};

export function emptyPublicBalancing(): PublicBalancing {
  const matrix = {} as BalancingMatrix;
  const pvp = {} as Record<BalanceClassCode, number | null>;
  const pvm = {} as Record<BalanceClassCode, number | null>;
  for (const atk of BALANCE_CLASS_CODES) {
    matrix[atk] = {} as Record<BalanceClassCode, number | null>;
    pvp[atk] = null;
    pvm[atk] = null;
    for (const def of BALANCE_CLASS_CODES) {
      matrix[atk][def] = null;
    }
  }
  return { matrix, pvp, pvm };
}
