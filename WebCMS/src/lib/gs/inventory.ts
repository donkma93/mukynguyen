import {
  calcItemDurability,
  getItemInfo,
  type ItemCatalogEntry,
} from "@/lib/gs/item-catalog";
import { getMaxSocketForItem } from "@/lib/gs/socket-item-type";

export const ITEM_SIZE = 16;
export const INVENTORY_SIZE = 236;
export const INVENTORY_BYTES = INVENTORY_SIZE * ITEM_SIZE; // 3776
export const INVENTORY_WEAR_SIZE = 12;
export const INVENTORY_MAIN_SIZE = 76;
export const INVENTORY_EXT1_SIZE = 108;
export const INVENTORY_EXT2_SIZE = 140;
export const INVENTORY_EXT3_SIZE = 172;
export const INVENTORY_EXT4_SIZE = 204;

export type GiveItemInput = {
  section?: number;
  type?: number;
  /** Absolute item index = section*512 + type */
  index?: number;
  level?: number;
  skill?: number;
  luck?: number;
  option?: number;
  excellent?: number;
  setOption?: number;
  durability?: number;
  joh?: number;
  optionEx?: number;
  socketCount?: number;
};

export type DecodedItem = {
  slot: number;
  index: number;
  level: number;
  skill: number;
  luck: number;
  option: number;
  excellent: number;
  setOption: number;
  durability: number;
  serial: number;
  name?: string;
};

function clampByte(n: number): number {
  return Math.max(0, Math.min(255, Math.floor(n)));
}

export function getInventoryMaxValue(extInventory: number): number {
  switch (Math.max(0, Math.min(4, extInventory | 0))) {
    case 0:
      return INVENTORY_MAIN_SIZE;
    case 1:
      return INVENTORY_EXT1_SIZE;
    case 2:
      return INVENTORY_EXT2_SIZE;
    case 3:
      return INVENTORY_EXT3_SIZE;
    default:
      return INVENTORY_EXT4_SIZE;
  }
}

export function ensureInventoryBuffer(raw: Buffer | null | undefined): Buffer {
  const buf = Buffer.alloc(INVENTORY_BYTES, 0xff);
  if (raw && raw.length) {
    raw.copy(buf, 0, 0, Math.min(raw.length, INVENTORY_BYTES));
  }
  return buf;
}

export function isEmptySlotBytes(slot: Buffer): boolean {
  if (slot.length < ITEM_SIZE) return true;
  // GS ConvertItemByte empty check + all-FF
  if (slot.every((b) => b === 0xff)) return true;
  return slot[0] === 0xff && (slot[7] & 0x80) === 0x80 && (slot[9] & 0xf0) === 0xf0;
}

export function decodeItemIndex(slot: Buffer): number {
  return slot[0] | ((slot[9] & 0xf0) * 32) | ((slot[7] & 0x80) * 2);
}

export function decodeSlot(slotIndex: number, slot: Buffer): DecodedItem | null {
  if (isEmptySlotBytes(slot)) return null;
  const index = decodeItemIndex(slot);
  const level = (slot[1] >> 3) & 0x0f;
  const skill = (slot[1] >> 7) & 1;
  const luck = (slot[1] >> 2) & 1;
  const option = (slot[1] & 3) + ((slot[7] & 64) / 16);
  const excellent = slot[7] & 0x3f;
  const setOption = slot[8] & 0x0f;
  const durability = slot[2];
  const serial =
    ((slot[3] << 24) >>> 0) + (slot[4] << 16) + (slot[5] << 8) + slot[6];
  const info = getItemInfo(index);
  return {
    slot: slotIndex,
    index,
    level,
    skill,
    luck,
    option,
    excellent,
    setOption,
    durability,
    serial,
    name: info?.name,
  };
}

/** Encode DB 16-byte item (DBItemByteConvert). */
export function encodeDbItem(input: {
  index: number;
  level: number;
  skill: number;
  luck: number;
  option: number;
  durability: number;
  serial: number;
  excellent?: number;
  setOption?: number;
  joh?: number;
  optionEx?: number;
  sockets?: number[];
}): Buffer {
  const index = input.index | 0;
  const level = clampByte(input.level) & 0x0f;
  const skill = input.skill ? 1 : 0;
  const luck = input.luck ? 1 : 0;
  const option = Math.max(0, Math.min(7, input.option | 0));
  const excellent = (input.excellent ?? 0) & 0x3f;
  const setOption = (input.setOption ?? 0) & 0x0f;
  const joh = clampByte(input.joh ?? 0);
  const optionEx = (input.optionEx ?? 0) & 0x80;
  const serial = input.serial >>> 0;
  const buf = Buffer.alloc(ITEM_SIZE, 0xff);

  buf[0] = index & 0xff;
  buf[1] = (level * 8) | (skill * 128) | (luck * 4) | (option & 3);
  buf[2] = clampByte(input.durability);
  buf[3] = (serial >>> 24) & 0xff;
  buf[4] = (serial >>> 16) & 0xff;
  buf[5] = (serial >>> 8) & 0xff;
  buf[6] = serial & 0xff;
  buf[7] = ((index & 256) >> 1) | (option > 3 ? 0x40 : 0) | excellent;
  buf[8] = setOption;
  buf[9] = ((index & 0x1e00) >> 5) | (optionEx >> 4);
  buf[10] = joh;

  const sockets = input.sockets ?? [0xff, 0xff, 0xff, 0xff, 0xff];
  for (let i = 0; i < 5; i++) {
    buf[11 + i] = clampByte(sockets[i] ?? 0xff);
  }
  return buf;
}

function slotOffset(slot: number): number {
  return slot * ITEM_SIZE;
}

function readSlot(inv: Buffer, slot: number): Buffer {
  const off = slotOffset(slot);
  return inv.subarray(off, off + ITEM_SIZE);
}

function writeSlot(inv: Buffer, slot: number, bytes: Buffer): void {
  bytes.copy(inv, slotOffset(slot), 0, ITEM_SIZE);
}

/** Build occupancy map for bag cells (0 = free, 1 = used), length = MaxY*8. */
export function buildBagOccupancy(
  inv: Buffer,
  extInventory: number
): { map: Uint8Array; maxSlots: number; maxY: number } {
  const maxSlots = getInventoryMaxValue(extInventory);
  const maxY = Math.floor((maxSlots - INVENTORY_WEAR_SIZE) / 8);
  const map = new Uint8Array(maxY * 8);
  map.fill(0);

  for (let slot = INVENTORY_WEAR_SIZE; slot < maxSlots; slot++) {
    const bytes = readSlot(inv, slot);
    if (isEmptySlotBytes(bytes)) continue;
    const index = decodeItemIndex(bytes);
    const info = getItemInfo(index);
    const w = info?.width ?? 1;
    const h = info?.height ?? 1;
    const bagIndex = slot - INVENTORY_WEAR_SIZE;
    const ox = bagIndex % 8;
    const oy = Math.floor(bagIndex / 8);
    for (let sy = 0; sy < h; sy++) {
      for (let sx = 0; sx < w; sx++) {
        const cx = ox + sx;
        const cy = oy + sy;
        if (cx >= 8 || cy >= maxY) continue;
        map[cy * 8 + cx] = 1;
      }
    }
  }

  return { map, maxSlots, maxY };
}

function rectFits(
  map: Uint8Array,
  maxY: number,
  x: number,
  y: number,
  width: number,
  height: number
): boolean {
  if (x + width > 8 || y + height > maxY) return false;
  for (let sy = 0; sy < height; sy++) {
    for (let sx = 0; sx < width; sx++) {
      if (map[(y + sy) * 8 + (x + sx)] !== 0) return false;
    }
  }
  return true;
}

export function findFreeBagSlot(
  inv: Buffer,
  extInventory: number,
  width: number,
  height: number
): number | null {
  const { map, maxY } = buildBagOccupancy(inv, extInventory);
  for (let y = 0; y < maxY; y++) {
    for (let x = 0; x < 8; x++) {
      if (map[y * 8 + x] !== 0) continue;
      if (rectFits(map, maxY, x, y, width, height)) {
        return INVENTORY_WEAR_SIZE + y * 8 + x;
      }
    }
  }
  return null;
}

export function resolveItemSpec(input: GiveItemInput): {
  info: ItemCatalogEntry;
  index: number;
} {
  let index = input.index;
  if (index === undefined || index === null) {
    if (input.section === undefined || input.type === undefined) {
      throw new Error("Thiếu section/type hoặc index item");
    }
    index = (input.section | 0) * 512 + (input.type | 0);
  }
  const info = getItemInfo(index);
  if (!info) throw new Error(`Không tìm thấy item index ${index} trong Item.txt`);
  return { info, index };
}

export function buildGiveItemBytes(
  input: GiveItemInput,
  serial: number
): { bytes: Buffer; info: ItemCatalogEntry; meta: Record<string, number> } {
  const { info, index } = resolveItemSpec(input);
  const level = Math.max(0, Math.min(15, input.level ?? 0));
  const skill = input.skill ? 1 : 0;
  const luck = input.luck ? 1 : 0;
  const option = Math.max(0, Math.min(7, input.option ?? 0));
  const excellent = Math.max(0, Math.min(63, input.excellent ?? 0));
  const setOption = Math.max(0, Math.min(15, input.setOption ?? 0));
  const joh = input.joh ?? 0;
  const optionEx = input.optionEx ?? 0;
  const maxSocket = getMaxSocketForItem(index);
  let socketCount = Math.max(0, Math.min(5, input.socketCount ?? 0));
  if (socketCount > 0 && maxSocket <= 0) {
    throw new Error(
      `Item ${info.name} (#${index}) không nằm trong SocketItemType.txt — không mở lỗ socket được`
    );
  }
  if (socketCount > maxSocket) {
    socketCount = maxSocket;
  }
  // 0xFE = empty socket hole (same as /make); 0xFF = no socket
  const sockets = Array.from({ length: 5 }, (_, i) =>
    i < socketCount ? 0xfe : 0xff
  );
  const durability =
    input.durability !== undefined && input.durability !== null
      ? clampByte(Number(input.durability))
      : calcItemDurability(info, level, excellent, setOption);

  const bytes = encodeDbItem({
    index,
    level,
    skill,
    luck,
    option,
    durability,
    serial,
    excellent,
    setOption,
    joh,
    optionEx,
    sockets,
  });

  return {
    bytes,
    info,
    meta: {
      index,
      level,
      skill,
      luck,
      option,
      excellent,
      setOption,
      durability,
      serial,
      socketCount,
    },
  };
}

export function insertItemIntoInventory(
  invRaw: Buffer,
  extInventory: number,
  itemBytes: Buffer,
  width: number,
  height: number
): { inventory: Buffer; slot: number } {
  const inv = ensureInventoryBuffer(invRaw);
  const slot = findFreeBagSlot(inv, extInventory, width, height);
  if (slot === null) {
    throw new Error("Túi đồ không còn chỗ trống (theo kích thước item)");
  }
  writeSlot(inv, slot, itemBytes);
  return { inventory: inv, slot };
}

export function listBagItems(
  invRaw: Buffer,
  extInventory: number
): DecodedItem[] {
  const inv = ensureInventoryBuffer(invRaw);
  const maxSlots = getInventoryMaxValue(extInventory);
  const out: DecodedItem[] = [];
  for (let slot = INVENTORY_WEAR_SIZE; slot < maxSlots; slot++) {
    const decoded = decodeSlot(slot, readSlot(inv, slot));
    if (decoded) out.push(decoded);
  }
  return out;
}
