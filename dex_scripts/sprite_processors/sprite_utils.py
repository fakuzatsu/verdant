import json
import subprocess
import tempfile
import os
from elftools.elf.elffile import ELFFile
import struct
from PIL import Image


def find_project_root():
    """Find project root by looking for Makefile"""
    current_dir = os.getcwd()
    while current_dir != os.path.dirname(current_dir):
        if os.path.exists(os.path.join(current_dir, "Makefile")):
            return current_dir
        current_dir = os.path.dirname(current_dir)
    raise RuntimeError("Could not find project root (no Makefile found)")


def read_data_from_elf(elf_file, address, size):
    """Extract binary data from an ELF file at a specified address"""
    with open(elf_file, "rb") as f:
        elf = ELFFile(f)
        for section in elf.iter_sections():
            sec_addr = section["sh_addr"]
            sec_size = section["sh_size"]

            if sec_addr <= address < sec_addr + sec_size:
                offset = address - sec_addr
                data = section.data()
                return data[offset : offset + size]
    return None


def decompress_lz77(data):
    """Decompress LZ77 compressed data"""
    if data[0] != 0x10:
        print("Data does not appear to be LZ77 compressed")
        return data

    decompressed_size = data[1] | (data[2] << 8) | (data[3] << 16)
    result = bytearray()
    pos = 4

    while len(result) < decompressed_size and pos < len(data):
        flags = data[pos]
        pos += 1

        for bit in range(8):
            if flags & (0x80 >> bit):
                if pos + 1 >= len(data):
                    break

                byte1 = data[pos]
                byte2 = data[pos + 1]
                pos += 2

                disp = ((byte1 & 0x0F) << 8) | byte2
                length = ((byte1 >> 4) & 0x0F) + 3

                if disp >= len(result):
                    print(f"Displacement {disp} exceeds result size {len(result)}")
                    continue

                for i in range(length):
                    if len(result) > 0:
                        result.append(result[-disp - 1])
            else:
                if pos >= len(data):
                    break
                result.append(data[pos])
                pos += 1

            if len(result) >= decompressed_size:
                break

    return bytes(result)


def decompress_smol(smol_bytes, tool_path):
    """
    Decompress .smol data using the external decompresSmol binary.
    Returns decompressed bytes.
    """
    with tempfile.TemporaryDirectory() as tmpdir:
        smol_path = os.path.join(tmpdir, "sprite.smol")

        # Write compressed data to disk
        with open(smol_path, "wb") as f:
            f.write(smol_bytes)

        # Call decompresSmol
        result = subprocess.run(
            [tool_path, "-d", smol_path],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            check=False
        )

        if result.returncode != 0:
            raise RuntimeError(
                f"decompresSmol failed:\n{result.stderr.decode(errors='ignore')}"
            )

        # decompresSmol usually overwrites or creates an output file
        # Adjust this if your tool behaves differently
        output_path = smol_path.replace(".smol", "")

        if not os.path.exists(output_path):
            raise FileNotFoundError("Expected decompressed output file not found")

        with open(output_path, "rb") as f:
            return f.read()


def parse_pointer(ptr_str):
    """Parse a pointer string to an integer address"""
    if isinstance(ptr_str, str):
        ptr_str = ptr_str.strip("\"'")
        if ptr_str.startswith("0x"):
            ptr_str = ptr_str[2:]
        return int(ptr_str, 16)
    return ptr_str


def extract_palette(elf_file, palette_ptr, num_colors=16):
    """Extract and parse a palette from the ELF file with proper GBA color format"""
    print(f"Reading palette from address: 0x{palette_ptr:08X}")

    raw_data = read_data_from_elf(elf_file, palette_ptr, 256)
    if not raw_data:
        print(f"Failed to read palette data at 0x{palette_ptr:X}")
        return None

    palette_data = raw_data
    if raw_data[0] == 0x10:
        print("Palette is LZ77 compressed, decompressing...")
        try:
            palette_data = decompress_lz77(raw_data)
            print(f"Decompressed palette size: {len(palette_data)} bytes")
        except Exception as e:
            print(f"Error decompressing palette: {e}")
            return None

    palette = []
    for i in range(0, min(32, len(palette_data)), 2):
        if i + 1 < len(palette_data):
            color = struct.unpack("<H", palette_data[i : i + 2])[0]
            r = ((color >> 0) & 0x1F) * 8
            g = ((color >> 5) & 0x1F) * 8
            b = ((color >> 10) & 0x1F) * 8
            palette.append((r, g, b))

    return palette


def render_sprite(sprite_data, palette, width=64, height=64):
    """Render a GBA 4bpp sprite with proper tile ordering"""
    image = Image.new("RGBA", (width, height), (0, 0, 0, 0))
    pixels = image.load()

    expected_size = (width * height) // 2
    if len(sprite_data) < expected_size:
        print(f"Sprite data too small ({len(sprite_data)} bytes, expected {expected_size})")

    tile_width = 8
    tile_height = 8
    tiles_per_row = width // tile_width

    for tile_y in range(height // tile_height):
        for tile_x in range(width // tile_width):
            tile_index = tile_y * tiles_per_row + tile_x
            tile_offset = tile_index * (tile_width * tile_height // 2)

            if tile_offset >= len(sprite_data):
                continue

            for row in range(tile_height):
                row_offset = tile_offset + (row * tile_width // 2)

                for col_pair in range(tile_width // 2):
                    if row_offset + col_pair >= len(sprite_data):
                        continue

                    pixel_byte = sprite_data[row_offset + col_pair]
                    pixel1_idx = pixel_byte & 0x0F
                    pixel2_idx = (pixel_byte >> 4) & 0x0F

                    x1 = tile_x * tile_width + col_pair * 2
                    x2 = x1 + 1
                    y = tile_y * tile_height + row

                    if pixel1_idx > 0 and pixel1_idx < len(palette):
                        pixels[x1, y] = palette[pixel1_idx] + (255,)

                    if pixel2_idx > 0 and pixel2_idx < len(palette):
                        pixels[x2, y] = palette[pixel2_idx] + (255,)

    return image


class SpriteProcessor:
    """Base class for sprite processing"""
    
    def __init__(self, json_file, elf_file, output_dir):
        self.project_root = find_project_root()
        self.json_file = os.path.join(self.project_root, json_file)
        self.elf_file = os.path.join(self.project_root, elf_file)
        self.output_dir = os.path.join(self.project_root, output_dir)
        self.max_sprite_size = 8192
        
    def load_data(self):
        """Load sprite data from JSON file"""
        with open(self.json_file, "r") as f:
            return json.load(f)
    
    def get_sprite_data(self, elf_file, sprite_ptr):
        """Extract and decompress sprite data from ELF file"""
        sprite_data = read_data_from_elf(
            elf_file, sprite_ptr, self.max_sprite_size
        )
        if not sprite_data:
            return None

        try:
            # LZ77 (existing)
            if sprite_data[0] == 0x10:
                sprite_data = decompress_lz77(sprite_data)
                print(f"  LZ77 decompressed: {len(sprite_data)} bytes")

            # SMOL (example magic value — adjust if needed)
            else:
                sprite_data = decompress_smol(
                    sprite_data,
                    tool_path=os.path.join(
                        self.project_root,
                        "tools/decompresSmol/decompresSmol"
                    )
                )
                print(f"  SMOL decompressed: {len(sprite_data)} bytes")

        except Exception as e:
            print(f"  Error decompressing sprite: {e}")
            return None

        return sprite_data

