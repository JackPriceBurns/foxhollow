#!/usr/bin/env python3
import struct, zlib, math, sys

WIDTH = 512
HEIGHT = 256


def shade(x, y):
    u = x / (WIDTH - 1)
    v = y / (HEIGHT - 1)
    checker = ((x // 32) + (y // 32)) % 2 == 0
    wave = 0.5 + 0.5 * math.sin((u * 6.0) + (v * 3.0))
    if checker:
        return (int(40 + 60 * v), int(120 + 110 * wave), int(200 - 80 * u), 255)
    return (int(230 - 90 * v), int(60 + 60 * u), int(120 + 100 * wave), 255)


def write_png(path, width, height, rgba):
    raw = b"".join(b"\x00" + rgba[y * width * 4:(y + 1) * width * 4] for y in range(height))

    def chunk(tag, payload):
        body = tag + payload
        return struct.pack(">I", len(payload)) + body + struct.pack(">I", zlib.crc32(body) & 0xFFFFFFFF)

    png = b"\x89PNG\r\n\x1a\n"
    png += chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, 8, 6, 0, 0, 0))
    png += chunk(b"IDAT", zlib.compress(raw, 9))
    png += chunk(b"IEND", b"")
    with open(path, "wb") as handle:
        handle.write(png)


def main():
    out = sys.argv[1] if len(sys.argv) > 1 else "mods/example-texture-pack/textures/tex1_512x256_454f43ec55d77742_14.png"
    pixels = bytearray()
    for y in range(HEIGHT):
        for x in range(WIDTH):
            pixels += bytes(shade(x, y))
    write_png(out, WIDTH, HEIGHT, bytes(pixels))
    print(f"wrote {out}")


if __name__ == "__main__":
    main()
