"""Create stylized Season 5.2 class emblems + hero collage for WebCMS homepage."""

from __future__ import annotations

import math
from pathlib import Path

from PIL import Image, ImageDraw, ImageFilter, ImageFont

OUT = Path(r"E:\SRC ThangCuoi\SRC ThangCuoi\WebCMS\public\assets\classes")
OUT.mkdir(parents=True, exist_ok=True)

CLASSES = [
    {
        "slug": "dark-wizard",
        "short": "DW",
        "name": "Dark Wizard",
        "accent": (168, 96, 255),
        "accent2": (90, 40, 180),
        "shape": "staff",
    },
    {
        "slug": "dark-knight",
        "short": "DK",
        "name": "Dark Knight",
        "accent": (220, 72, 72),
        "accent2": (120, 20, 30),
        "shape": "sword",
    },
    {
        "slug": "fairy-elf",
        "short": "FE",
        "name": "Fairy Elf",
        "accent": (80, 220, 140),
        "accent2": (20, 120, 80),
        "shape": "bow",
    },
    {
        "slug": "magic-gladiator",
        "short": "MG",
        "name": "Magic Gladiator",
        "accent": (70, 190, 255),
        "accent2": (20, 90, 170),
        "shape": "hybrid",
    },
    {
        "slug": "dark-lord",
        "short": "DL",
        "name": "Dark Lord",
        "accent": (255, 190, 70),
        "accent2": (160, 100, 20),
        "shape": "crown",
    },
    {
        "slug": "summoner",
        "short": "SU",
        "name": "Summoner",
        "accent": (190, 120, 255),
        "accent2": (100, 50, 170),
        "shape": "orb",
    },
    {
        "slug": "rage-fighter",
        "short": "RF",
        "name": "Rage Fighter",
        "accent": (255, 120, 70),
        "accent2": (160, 40, 20),
        "shape": "fist",
    },
]


def lerp(a: float, b: float, t: float) -> float:
    return a + (b - a) * t


def gradient_bg(size: tuple[int, int], c1: tuple[int, int, int], c2: tuple[int, int, int]) -> Image.Image:
    w, h = size
    img = Image.new("RGBA", size)
    px = img.load()
    for y in range(h):
        t = y / max(1, h - 1)
        r = int(lerp(c1[0], c2[0], t))
        g = int(lerp(c1[1], c2[1], t))
        b = int(lerp(c1[2], c2[2], t))
        for x in range(w):
            # slight vignette
            dx = (x / w - 0.5) * 2
            dy = (y / h - 0.5) * 2
            vig = 1 - min(1, (dx * dx + dy * dy) * 0.55)
            px[x, y] = (
                int(r * (0.35 + 0.65 * vig)),
                int(g * (0.35 + 0.65 * vig)),
                int(b * (0.35 + 0.65 * vig)),
                255,
            )
    return img


def draw_glow_circle(draw: ImageDraw.ImageDraw, cx: int, cy: int, radius: int, color: tuple[int, int, int], layers: int = 6):
    for i in range(layers, 0, -1):
        alpha = int(18 + (layers - i) * 10)
        r = radius + i * 10
        draw.ellipse((cx - r, cy - r, cx + r, cy + r), outline=(*color, alpha), width=3)


def draw_staff(draw: ImageDraw.ImageDraw, cx: int, cy: int, accent: tuple[int, int, int]):
    draw.line((cx, cy - 90, cx, cy + 110), fill=(*accent, 255), width=10)
    draw.ellipse((cx - 28, cy - 130, cx + 28, cy - 74), outline=(*accent, 255), width=6)
    draw.ellipse((cx - 10, cy - 112, cx + 10, cy - 92), fill=(255, 255, 255, 220))
    for ang in range(0, 360, 45):
        rad = math.radians(ang)
        x2 = cx + int(math.cos(rad) * 48)
        y2 = cy - 102 + int(math.sin(rad) * 48)
        draw.line((cx, cy - 102, x2, y2), fill=(*accent, 160), width=2)


def draw_sword(draw: ImageDraw.ImageDraw, cx: int, cy: int, accent: tuple[int, int, int]):
    draw.polygon(
        [(cx, cy - 120), (cx + 16, cy - 40), (cx + 8, cy + 70), (cx - 8, cy + 70), (cx - 16, cy - 40)],
        fill=(230, 230, 240, 230),
        outline=(*accent, 255),
    )
    draw.rectangle((cx - 40, cy + 70, cx + 40, cy + 86), fill=(*accent, 255))
    draw.rectangle((cx - 8, cy + 86, cx + 8, cy + 120), fill=(180, 160, 100, 255))


def draw_bow(draw: ImageDraw.ImageDraw, cx: int, cy: int, accent: tuple[int, int, int]):
    draw.arc((cx - 70, cy - 110, cx + 30, cy + 110), 250, 110, fill=(*accent, 255), width=8)
    draw.line((cx - 20, cy - 100, cx - 20, cy + 100), fill=(220, 220, 200, 200), width=2)
    draw.line((cx - 20, cy, cx + 90, cy), fill=(*accent, 230), width=4)
    draw.polygon([(cx + 90, cy), (cx + 70, cy - 8), (cx + 70, cy + 8)], fill=(255, 255, 255, 230))


def draw_hybrid(draw: ImageDraw.ImageDraw, cx: int, cy: int, accent: tuple[int, int, int]):
    draw_sword(draw, cx - 30, cy + 10, accent)
    draw.ellipse((cx + 20, cy - 50, cx + 90, cy + 20), outline=(*accent, 255), width=5)
    draw.ellipse((cx + 40, cy - 30, cx + 70, cy), fill=(255, 255, 255, 200))


def draw_crown(draw: ImageDraw.ImageDraw, cx: int, cy: int, accent: tuple[int, int, int]):
    pts = [
        (cx - 70, cy + 40),
        (cx - 70, cy - 20),
        (cx - 35, cy + 10),
        (cx, cy - 70),
        (cx + 35, cy + 10),
        (cx + 70, cy - 20),
        (cx + 70, cy + 40),
    ]
    draw.polygon(pts, fill=(*accent, 230), outline=(255, 230, 150, 255))
    draw.ellipse((cx - 10, cy - 10, cx + 10, cy + 10), fill=(255, 255, 255, 220))


def draw_orb(draw: ImageDraw.ImageDraw, cx: int, cy: int, accent: tuple[int, int, int]):
    draw.ellipse((cx - 55, cy - 55, cx + 55, cy + 55), fill=(*accent, 90), outline=(*accent, 255), width=5)
    draw.ellipse((cx - 25, cy - 35, cx + 5, cy - 5), fill=(255, 255, 255, 180))
    for i, r in enumerate((80, 100, 120)):
        draw.arc((cx - r, cy - r, cx + r, cy + r), 200 + i * 20, 320 + i * 10, fill=(*accent, 140), width=2)


def draw_fist(draw: ImageDraw.ImageDraw, cx: int, cy: int, accent: tuple[int, int, int]):
    draw.rounded_rectangle((cx - 45, cy - 30, cx + 45, cy + 55), radius=18, fill=(*accent, 220), outline=(255, 220, 180, 255), width=3)
    for i in range(4):
        x0 = cx - 36 + i * 18
        draw.rounded_rectangle((x0, cy - 70, x0 + 14, cy - 20), radius=6, fill=(*accent, 240))
    draw.ellipse((cx - 55, cy + 20, cx - 25, cy + 55), fill=(*accent, 230))


SHAPES = {
    "staff": draw_staff,
    "sword": draw_sword,
    "bow": draw_bow,
    "hybrid": draw_hybrid,
    "crown": draw_crown,
    "orb": draw_orb,
    "fist": draw_fist,
}


def try_font(size: int) -> ImageFont.ImageFont:
    for name in (
        r"C:\Windows\Fonts\georgia.ttf",
        r"C:\Windows\Fonts\consola.ttf",
        r"C:\Windows\Fonts\arialbd.ttf",
        r"C:\Windows\Fonts\arial.ttf",
    ):
        try:
            return ImageFont.truetype(name, size)
        except OSError:
            continue
    return ImageFont.load_default()


def make_emblem(cls: dict) -> Image.Image:
    size = 640
    img = gradient_bg((size, size), (12, 14, 24), cls["accent2"])
    # soft aurora blobs
    overlay = Image.new("RGBA", (size, size), (0, 0, 0, 0))
    od = ImageDraw.Draw(overlay)
    od.ellipse((60, 40, 320, 300), fill=(*cls["accent"], 55))
    od.ellipse((300, 280, 580, 560), fill=(*cls["accent2"], 70))
    overlay = overlay.filter(ImageFilter.GaussianBlur(28))
    img = Image.alpha_composite(img, overlay)

    draw = ImageDraw.Draw(img)
    cx = cy = size // 2
    draw_glow_circle(draw, cx, cy - 20, 150, cls["accent"])
    draw.ellipse((cx - 150, cy - 170, cx + 150, cy + 130), outline=(255, 214, 120, 180), width=3)
    draw.ellipse((cx - 140, cy - 160, cx + 140, cy + 120), outline=(*cls["accent"], 120), width=2)

    SHAPES[cls["shape"]](draw, cx, cy - 20, cls["accent"])

    # bottom label band
    draw.rounded_rectangle((70, 500, 570, 590), radius=16, fill=(0, 0, 0, 150), outline=(*cls["accent"], 180), width=2)
    font_name = try_font(28)
    font_short = try_font(20)
    name = cls["name"]
    # center text roughly
    bbox = draw.textbbox((0, 0), name, font=font_name)
    tw = bbox[2] - bbox[0]
    draw.text(((size - tw) / 2, 520), name, font=font_name, fill=(255, 230, 170, 255))
    sb = draw.textbbox((0, 0), cls["short"] + " · Season 5.2", font=font_short)
    sw = sb[2] - sb[0]
    draw.text(((size - sw) / 2, 552), cls["short"] + " · Season 5.2", font=font_short, fill=(*cls["accent"], 230))
    return img


def make_card(cls: dict) -> Image.Image:
    """Tall card used on homepage class grid."""
    w, h = 512, 720
    img = gradient_bg((w, h), (10, 12, 20), cls["accent2"])
    overlay = Image.new("RGBA", (w, h), (0, 0, 0, 0))
    od = ImageDraw.Draw(overlay)
    od.ellipse((-40, -20, 360, 380), fill=(*cls["accent"], 60))
    od.ellipse((180, 360, 560, 760), fill=(*cls["accent2"], 80))
    overlay = overlay.filter(ImageFilter.GaussianBlur(30))
    img = Image.alpha_composite(img, overlay)

    emblem = make_emblem(cls).resize((420, 420), Image.Resampling.LANCZOS)
    img.alpha_composite(emblem, ((w - 420) // 2, 40))

    draw = ImageDraw.Draw(img)
    draw.rectangle((0, 0, w - 1, h - 1), outline=(*cls["accent"], 160), width=3)
    draw.rectangle((8, 8, w - 9, h - 9), outline=(255, 214, 120, 70), width=1)
    return img


def make_hero_collage(cards: list[Image.Image]) -> Image.Image:
    W, H = 1920, 1080
    canvas = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    # dark atmospheric base
    base = gradient_bg((W, H), (6, 8, 16), (24, 14, 40))
    canvas = Image.alpha_composite(canvas, base)

    # place 7 cards in a fan / staggered row
    n = len(cards)
    spacing = 230
    start_x = (W - spacing * (n - 1)) // 2
    for i, card in enumerate(cards):
        scale = 0.78 if i % 2 else 0.9
        cw = int(card.width * scale)
        ch = int(card.height * scale)
        c = card.resize((cw, ch), Image.Resampling.LANCZOS)
        # slight rotation feel by offsetting Y
        x = start_x + i * spacing - cw // 2
        y = 140 + (50 if i % 2 else 0) + abs(i - 3) * 12
        shadow = Image.new("RGBA", (cw + 40, ch + 40), (0, 0, 0, 0))
        sd = ImageDraw.Draw(shadow)
        sd.rounded_rectangle((10, 10, cw + 20, ch + 20), radius=24, fill=(0, 0, 0, 120))
        shadow = shadow.filter(ImageFilter.GaussianBlur(18))
        canvas.alpha_composite(shadow, (x - 10, y + 20))
        canvas.alpha_composite(c, (x, y))

    # vignette
    vig = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    vd = ImageDraw.Draw(vig)
    for i, a in enumerate((110, 80, 50, 25)):
        inset = i * 40
        vd.rectangle((inset, inset, W - 1 - inset, H - 1 - inset), outline=(0, 0, 0, a), width=40)
    vig = vig.filter(ImageFilter.GaussianBlur(20))
    canvas = Image.alpha_composite(canvas, vig)
    return canvas


def main() -> None:
    cards = []
    for cls in CLASSES:
        emblem = make_emblem(cls)
        emblem_path = OUT / f"{cls['slug']}-emblem.png"
        emblem.save(emblem_path, optimize=True)
        print("emblem", emblem_path.name)

        card = make_card(cls)
        card_path = OUT / f"{cls['slug']}.png"
        card.save(card_path, optimize=True)
        cards.append(card)
        print("card", card_path.name)

    hero = make_hero_collage(cards)
    hero_path = OUT / "hero-classes.png"
    hero.save(hero_path, optimize=True)
    print("hero", hero_path.name, hero.size)


if __name__ == "__main__":
    main()
