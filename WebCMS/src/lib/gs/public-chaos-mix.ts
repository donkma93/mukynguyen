import {
  ACCOUNT_LEVELS,
  type AccountLevel,
  parseIniValues,
  ratesByAccountLevel,
} from "@/lib/gs/public-guide-parse";

export type AlRates = Record<AccountLevel, number | null>;

export type PublicChaosMix = {
  plusItem: { level: number; label: string; rates: AlRates }[];
  wings: { key: string; label: string; rates: AlRates }[];
  bloodCastleMix: { tier: number; rates: AlRates }[];
  devilSquareMix: { tier: number; rates: AlRates }[];
  jewels: {
    soul: AlRates;
    life: AlRates;
    harmony: AlRates;
    luck1: AlRates;
    luck2: AlRates;
  };
  itemDropRate: AlRates;
  moneyDropRate: AlRates;
};

export function emptyPublicChaosMix(): PublicChaosMix {
  const emptyAl = (): AlRates => ({ 0: null, 1: null, 2: null, 3: null });
  return {
    plusItem: [10, 11, 12, 13, 14, 15].map((level, i) => ({
      level,
      label: `+${level}`,
      rates: emptyAl(),
    })),
    wings: [
      { key: "Wing1", label: "Wing 1", rates: emptyAl() },
      { key: "Wing2", label: "Wing 2", rates: emptyAl() },
      { key: "Wing3", label: "Wing 3", rates: emptyAl() },
    ],
    bloodCastleMix: Array.from({ length: 7 }, (_, i) => ({
      tier: i + 1,
      rates: emptyAl(),
    })),
    devilSquareMix: Array.from({ length: 7 }, (_, i) => ({
      tier: i + 1,
      rates: emptyAl(),
    })),
    jewels: {
      soul: emptyAl(),
      life: emptyAl(),
      harmony: emptyAl(),
      luck1: emptyAl(),
      luck2: emptyAl(),
    },
    itemDropRate: emptyAl(),
    moneyDropRate: emptyAl(),
  };
}

export function buildPublicChaosMix(
  chaosMixText: string,
  commonText: string
): PublicChaosMix {
  const chaos = parseIniValues(chaosMixText);
  const common = parseIniValues(commonText);
  const dto = emptyPublicChaosMix();

  dto.plusItem = [1, 2, 3, 4, 5, 6].map((idx) => ({
    level: 9 + idx,
    label: `+${9 + idx}`,
    rates: ratesByAccountLevel(chaos, `PlusItemLevelMixRate${idx}`),
  }));

  dto.wings = [
    {
      key: "Wing1",
      label: "Wing 1",
      rates: ratesByAccountLevel(chaos, "Wing1MixRate"),
    },
    {
      key: "Wing2",
      label: "Wing 2",
      rates: ratesByAccountLevel(chaos, "Wing2MixRate"),
    },
    {
      key: "Wing3",
      label: "Wing 3",
      rates: ratesByAccountLevel(chaos, "Wing3MixRate"),
    },
  ];

  dto.bloodCastleMix = [1, 2, 3, 4, 5, 6, 7].map((tier) => ({
    tier,
    rates: ratesByAccountLevel(chaos, `BloodCastleMixRate${tier}`),
  }));

  dto.devilSquareMix = [1, 2, 3, 4, 5, 6, 7].map((tier) => ({
    tier,
    rates: ratesByAccountLevel(chaos, `DevilSquareMixRate${tier}`),
  }));

  dto.jewels = {
    soul: ratesByAccountLevel(common, "SoulSuccessRate"),
    life: ratesByAccountLevel(common, "LifeSuccessRate"),
    harmony: ratesByAccountLevel(common, "HarmonySuccessRate"),
    luck1: ratesByAccountLevel(common, "AddLuckSuccessRate1"),
    luck2: ratesByAccountLevel(common, "AddLuckSuccessRate2"),
  };

  dto.itemDropRate = ratesByAccountLevel(common, "ItemDropRate");
  dto.moneyDropRate = ratesByAccountLevel(common, "MoneyAmountDropRate");

  // Drop empty BC/DS tiers that have no AL0 key at all
  dto.bloodCastleMix = dto.bloodCastleMix.filter((row) =>
    ACCOUNT_LEVELS.some((al) => row.rates[al] != null)
  );
  dto.devilSquareMix = dto.devilSquareMix.filter((row) =>
    ACCOUNT_LEVELS.some((al) => row.rates[al] != null)
  );

  return dto;
}

/** True when all AL tiers share the same non-null rate. */
export function uniformRate(rates: AlRates): number | null {
  const vals = ACCOUNT_LEVELS.map((al) => rates[al]).filter(
    (v): v is number => v != null
  );
  if (!vals.length) return null;
  const first = vals[0];
  return vals.every((v) => v === first) ? first : null;
}

const DEFAULT_VIP_SHORT: Record<AccountLevel, string> = {
  0: "Normal",
  1: "VIP 1",
  2: "VIP 2",
  3: "VIP 3",
};

export function formatAlRates(
  rates: AlRates,
  options?: {
    suffix?: string;
    /** Short VIP labels keyed by account level. */
    vipShort?: Partial<Record<AccountLevel, string>> | Record<string, string>;
    /** Joiner between differing tiers (default: ", "). */
    join?: string;
  }
): string {
  const suffix = options?.suffix ?? "%";
  const join = options?.join ?? ", ";
  const vipShort = options?.vipShort;
  const labelFor = (al: AccountLevel) =>
    (vipShort?.[String(al) as `${AccountLevel}`] as string | undefined) ??
    (vipShort?.[al] as string | undefined) ??
    DEFAULT_VIP_SHORT[al];
  const u = uniformRate(rates);
  if (u != null) return `${u}${suffix}`;
  return ACCOUNT_LEVELS.map((al) => {
    const v = rates[al];
    const value = v == null ? "—" : `${v}${suffix}`;
    return `${labelFor(al)} ${value}`;
  }).join(join);
}

export function sampleRate(rates: AlRates): number | null {
  for (const al of ACCOUNT_LEVELS) {
    const v = rates[al];
    if (v != null) return v;
  }
  return null;
}

export function chaosMixHasData(dto: PublicChaosMix): boolean {
  return dto.plusItem.some((r) => sampleRate(r.rates) != null);
}
