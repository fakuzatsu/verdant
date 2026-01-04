#include "dex/gf_string.h"
#include "global.h"

#define GBA_END 0xFF
#define PRINT_UNMAPPED_BYTES FALSE

int ConvertGfCharToUtf8(u8 gfChar, u8 *out)
{
    switch (gfChar)
    {
    case 0x00:
        memcpy(out, (u8[]){0x20}, 1);
        return 1;
    case 0x01:
        memcpy(out, (u8[]){0xC3, 0x80}, 2);
        return 2;
    case 0x02:
        memcpy(out, (u8[]){0xC3, 0x81}, 2);
        return 2;
    case 0x03:
        memcpy(out, (u8[]){0xC3, 0x82}, 2);
        return 2;
    case 0x04:
        memcpy(out, (u8[]){0xC3, 0x87}, 2);
        return 2;
    case 0x05:
        memcpy(out, (u8[]){0xC3, 0x88}, 2);
        return 2;
    case 0x06:
        memcpy(out, (u8[]){0xC3, 0x89}, 2);
        return 2;
    case 0x07:
        memcpy(out, (u8[]){0xC3, 0x8A}, 2);
        return 2;
    case 0x08:
        memcpy(out, (u8[]){0xC3, 0x8B}, 2);
        return 2;
    case 0x09:
        memcpy(out, (u8[]){0xC3, 0x8C}, 2);
        return 2;
    case 0x0B:
        memcpy(out, (u8[]){0xC3, 0x8E}, 2);
        return 2;
    case 0x0C:
        memcpy(out, (u8[]){0xC3, 0x8F}, 2);
        return 2;
    case 0x0D:
        memcpy(out, (u8[]){0xC3, 0x92}, 2);
        return 2;
    case 0x0E:
        memcpy(out, (u8[]){0xC3, 0x93}, 2);
        return 2;
    case 0x0F:
        memcpy(out, (u8[]){0xC3, 0x94}, 2);
        return 2;
    case 0x10:
        memcpy(out, (u8[]){0xC5, 0x92}, 2);
        return 2;
    case 0x11:
        memcpy(out, (u8[]){0xC3, 0x99}, 2);
        return 2;
    case 0x12:
        memcpy(out, (u8[]){0xC3, 0x9A}, 2);
        return 2;
    case 0x13:
        memcpy(out, (u8[]){0xC3, 0x9B}, 2);
        return 2;
    case 0x14:
        memcpy(out, (u8[]){0xC3, 0x91}, 2);
        return 2;
    case 0x15:
        memcpy(out, (u8[]){0xC3, 0x9F}, 2);
        return 2;
    case 0x16:
        memcpy(out, (u8[]){0xC3, 0xA0}, 2);
        return 2;
    case 0x17:
        memcpy(out, (u8[]){0xC3, 0xA1}, 2);
        return 2;
    case 0x19:
        memcpy(out, (u8[]){0xC3, 0xA7}, 2);
        return 2;
    case 0x1A:
        memcpy(out, (u8[]){0xC3, 0xA8}, 2);
        return 2;
    case 0x1B:
        memcpy(out, (u8[]){0xC3, 0xA9}, 2);
        return 2;
    case 0x1C:
        memcpy(out, (u8[]){0xC3, 0xAA}, 2);
        return 2;
    case 0x1D:
        memcpy(out, (u8[]){0xC3, 0xAB}, 2);
        return 2;
    case 0x1E:
        memcpy(out, (u8[]){0xC3, 0xAC}, 2);
        return 2;
    case 0x20:
        memcpy(out, (u8[]){0xC3, 0xAE}, 2);
        return 2;
    case 0x21:
        memcpy(out, (u8[]){0xC3, 0xAF}, 2);
        return 2;
    case 0x22:
        memcpy(out, (u8[]){0xC3, 0xB2}, 2);
        return 2;
    case 0x23:
        memcpy(out, (u8[]){0xC3, 0xB3}, 2);
        return 2;
    case 0x24:
        memcpy(out, (u8[]){0xC3, 0xB4}, 2);
        return 2;
    case 0x25:
        memcpy(out, (u8[]){0xC5, 0x93}, 2);
        return 2;
    case 0x26:
        memcpy(out, (u8[]){0xC3, 0xB9}, 2);
        return 2;
    case 0x27:
        memcpy(out, (u8[]){0xC3, 0xBA}, 2);
        return 2;
    case 0x28:
        memcpy(out, (u8[]){0xC3, 0xBB}, 2);
        return 2;
    case 0x29:
        memcpy(out, (u8[]){0xC3, 0xB1}, 2);
        return 2;
    case 0x2A:
        memcpy(out, (u8[]){0xC2, 0xBA}, 2);
        return 2;
    case 0x2B:
        memcpy(out, (u8[]){0xC2, 0xAA}, 2);
        return 2;
    case 0x2C:
        memcpy(out, (u8[]){0xE1, 0xB5, 0x89, 0xCA, 0xB3}, 5);
        return 5;
    case 0x2D:
        memcpy(out, (u8[]){0x26}, 1);
        return 1;
    case 0x2E:
        memcpy(out, (u8[]){0x2B}, 1);
        return 1;
    case 0x50:
        memcpy(out, (u8[]){0xE2, 0x96, 0xAF}, 3);
        return 3;
    case 0x51:
        memcpy(out, (u8[]){0xC2, 0xBF}, 2);
        return 2;
    case 0x52:
        memcpy(out, (u8[]){0xC2, 0xA1}, 2);
        return 2;
    case 0x53:
        memcpy(out, (u8[]){0x50, 0x4B}, 2);
        return 2;
    case 0x54:
        memcpy(out, (u8[]){0x4D, 0x4E}, 2);
        return 2;
    case 0x5C:
        memcpy(out, (u8[]){0xC3, 0x8D}, 2);
        return 2;
    case 0x5D:
        memcpy(out, (u8[]){0x25}, 1);
        return 1;
    case 0x5E:
        memcpy(out, (u8[]){0x28}, 1);
        return 1;
    case 0x5F:
        memcpy(out, (u8[]){0x29}, 1);
        return 1;
    case 0x7C:
        memcpy(out, (u8[]){0xE2, 0x86, 0x91}, 3);
        return 3;
    case 0x7D:
        memcpy(out, (u8[]){0xE2, 0x86, 0x93}, 3);
        return 3;
    case 0x7E:
        memcpy(out, (u8[]){0xE2, 0x86, 0x90}, 3);
        return 3;
    case 0x7F:
        memcpy(out, (u8[]){0xE2, 0x86, 0x92}, 3);
        return 3;
    case 0x80:
        memcpy(out, (u8[]){0x2A}, 1);
        return 1;
    case 0x81:
        memcpy(out, (u8[]){0x2A}, 1);
        return 1;
    case 0x82:
        memcpy(out, (u8[]){0x2A}, 1);
        return 1;
        memcpy(out, (u8[]){0x2A}, 1);
        return 1;
    case 0x83:
        memcpy(out, (u8[]){0x2A}, 1);
        return 1;
    case 0x84:
        memcpy(out, (u8[]){0xE1, 0xB5, 0x89}, 3);
        return 3;
    case 0x85:
        memcpy(out, (u8[]){0x3C}, 1);
        return 1;
    case 0x86:
        memcpy(out, (u8[]){0x3E}, 1);
        return 1;
    case 0xA0:
        memcpy(out, (u8[]){0xCA, 0xB3, 0xE1, 0xB5, 0x89}, 5);
        return 5;
    case 0xA1:
        memcpy(out, (u8[]){0x30}, 1);
        return 1;
    case 0xA2:
        memcpy(out, (u8[]){0x31}, 1);
        return 1;
    case 0xA3:
        memcpy(out, (u8[]){0x32}, 1);
        return 1;
    case 0xA4:
        memcpy(out, (u8[]){0x33}, 1);
        return 1;
    case 0xA5:
        memcpy(out, (u8[]){0x34}, 1);
        return 1;
    case 0xA6:
        memcpy(out, (u8[]){0x35}, 1);
        return 1;
    case 0xA7:
        memcpy(out, (u8[]){0x36}, 1);
        return 1;
    case 0xA8:
        memcpy(out, (u8[]){0x37}, 1);
        return 1;
    case 0xA9:
        memcpy(out, (u8[]){0x38}, 1);
        return 1;
    case 0xAA:
        memcpy(out, (u8[]){0x39}, 1);
        return 1;
    case 0xAB:
        memcpy(out, (u8[]){0x21}, 1);
        return 1;
    case 0xAC:
        memcpy(out, (u8[]){0x3F}, 1);
        return 1;
    case 0xAD:
        memcpy(out, (u8[]){0x2E}, 1);
        return 1;
    case 0xAE:
        memcpy(out, (u8[]){0x2D}, 1);
        return 1;
    case 0xAF:
        memcpy(out, (u8[]){0xEF, 0xBD, 0xA5}, 3);
        return 3;
    case 0xB0:
        memcpy(out, (u8[]){0xE2, 0x80, 0xA5}, 3);
        return 3;
    case 0xB1:
        memcpy(out, (u8[]){0xE2, 0x80, 0x9C}, 3);
        return 3;
    case 0xB2:
        memcpy(out, (u8[]){0xE2, 0x80, 0x9D}, 3);
        return 3;
    case 0xB3:
        memcpy(out, (u8[]){0xE2, 0x80, 0x98}, 3);
        return 3;
    case 0xB4:
        memcpy(out, (u8[]){0x27}, 1);
        return 1;
    case 0xB5:
        memcpy(out, (u8[]){0xE2, 0x99, 0x82}, 3);
        return 3;
    case 0xB6:
        memcpy(out, (u8[]){0xE2, 0x99, 0x80}, 3);
        return 3;
    case 0xB7:
        memcpy(out, (u8[]){0x24}, 1);
        return 1;
    case 0xB8:
        memcpy(out, (u8[]){0x2C}, 1);
        return 1;
    case 0xB9:
        memcpy(out, (u8[]){0xC3, 0x97}, 2);
        return 2;
    case 0xBA:
        memcpy(out, (u8[]){0x2F}, 1);
        return 1;
    case 0xBB:
        memcpy(out, (u8[]){0x41}, 1);
        return 1;
    case 0xBC:
        memcpy(out, (u8[]){0x42}, 1);
        return 1;
    case 0xBD:
        memcpy(out, (u8[]){0x43}, 1);
        return 1;
    case 0xBE:
        memcpy(out, (u8[]){0x44}, 1);
        return 1;
    case 0xBF:
        memcpy(out, (u8[]){0x45}, 1);
        return 1;
    case 0xC0:
        memcpy(out, (u8[]){0x46}, 1);
        return 1;
    case 0xC1:
        memcpy(out, (u8[]){0x47}, 1);
        return 1;
    case 0xC2:
        memcpy(out, (u8[]){0x48}, 1);
        return 1;
    case 0xC3:
        memcpy(out, (u8[]){0x49}, 1);
        return 1;
    case 0xC4:
        memcpy(out, (u8[]){0x4A}, 1);
        return 1;
    case 0xC5:
        memcpy(out, (u8[]){0x4B}, 1);
        return 1;
    case 0xC6:
        memcpy(out, (u8[]){0x4C}, 1);
        return 1;
    case 0xC7:
        memcpy(out, (u8[]){0x4D}, 1);
        return 1;
    case 0xC8:
        memcpy(out, (u8[]){0x4E}, 1);
        return 1;
    case 0xC9:
        memcpy(out, (u8[]){0x4F}, 1);
        return 1;
    case 0xCA:
        memcpy(out, (u8[]){0x50}, 1);
        return 1;
    case 0xCB:
        memcpy(out, (u8[]){0x51}, 1);
        return 1;
    case 0xCC:
        memcpy(out, (u8[]){0x52}, 1);
        return 1;
    case 0xCD:
        memcpy(out, (u8[]){0x53}, 1);
        return 1;
    case 0xCE:
        memcpy(out, (u8[]){0x54}, 1);
        return 1;
    case 0xCF:
        memcpy(out, (u8[]){0x55}, 1);
        return 1;
    case 0xD0:
        memcpy(out, (u8[]){0x56}, 1);
        return 1;
    case 0xD1:
        memcpy(out, (u8[]){0x57}, 1);
        return 1;
    case 0xD2:
        memcpy(out, (u8[]){0x58}, 1);
        return 1;
    case 0xD3:
        memcpy(out, (u8[]){0x59}, 1);
        return 1;
    case 0xD4:
        memcpy(out, (u8[]){0x5A}, 1);
        return 1;
    case 0xD5:
        memcpy(out, (u8[]){0x61}, 1);
        return 1;
    case 0xD6:
        memcpy(out, (u8[]){0x62}, 1);
        return 1;
    case 0xD7:
        memcpy(out, (u8[]){0x63}, 1);
        return 1;
    case 0xD8:
        memcpy(out, (u8[]){0x64}, 1);
        return 1;
    case 0xD9:
        memcpy(out, (u8[]){0x65}, 1);
        return 1;
    case 0xDA:
        memcpy(out, (u8[]){0x66}, 1);
        return 1;
    case 0xDB:
        memcpy(out, (u8[]){0x67}, 1);
        return 1;
    case 0xDC:
        memcpy(out, (u8[]){0x68}, 1);
        return 1;
    case 0xDD:
        memcpy(out, (u8[]){0x69}, 1);
        return 1;
    case 0xDE:
        memcpy(out, (u8[]){0x6A}, 1);
        return 1;
    case 0xDF:
        memcpy(out, (u8[]){0x6B}, 1);
        return 1;
    case 0xE0:
        memcpy(out, (u8[]){0x6C}, 1);
        return 1;
    case 0xE1:
        memcpy(out, (u8[]){0x6D}, 1);
        return 1;
    case 0xE2:
        memcpy(out, (u8[]){0x6E}, 1);
        return 1;
    case 0xE3:
        memcpy(out, (u8[]){0x6F}, 1);
        return 1;
    case 0xE4:
        memcpy(out, (u8[]){0x70}, 1);
        return 1;
    case 0xE5:
        memcpy(out, (u8[]){0x71}, 1);
        return 1;
    case 0xE6:
        memcpy(out, (u8[]){0x72}, 1);
        return 1;
    case 0xE7:
        memcpy(out, (u8[]){0x73}, 1);
        return 1;
    case 0xE8:
        memcpy(out, (u8[]){0x74}, 1);
        return 1;
    case 0xE9:
        memcpy(out, (u8[]){0x75}, 1);
        return 1;
    case 0xEA:
        memcpy(out, (u8[]){0x76}, 1);
        return 1;
    case 0xEB:
        memcpy(out, (u8[]){0x77}, 1);
        return 1;
    case 0xEC:
        memcpy(out, (u8[]){0x78}, 1);
        return 1;
    case 0xED:
        memcpy(out, (u8[]){0x79}, 1);
        return 1;
    case 0xEE:
        memcpy(out, (u8[]){0x7A}, 1);
        return 1;
    case 0xEF:
        memcpy(out, (u8[]){0xE2, 0x96, 0xBA}, 3);
        return 3;
    case 0xF0:
        memcpy(out, (u8[]){0x3A}, 1);
        return 1;
    case 0xF1:
        memcpy(out, (u8[]){0xC3, 0x84}, 2);
        return 2;
    case 0xF2:
        memcpy(out, (u8[]){0xC3, 0x96}, 2);
        return 2;
    case 0xF3:
        memcpy(out, (u8[]){0xC3, 0x9C}, 2);
        return 2;
    case 0xF4:
        memcpy(out, (u8[]){0xC3, 0xA4}, 2);
        return 2;
    case 0xF5:
        memcpy(out, (u8[]){0xC3, 0xB6}, 2);
        return 2;
    case 0xF6:
        memcpy(out, (u8[]){0xC3, 0xBC}, 2);
        return 2;
    case 0xF7:
        memcpy(out, (u8[]){0x43, 0x6F, 0x6E, 0x74, 0x72, 0x6F, 0x6C, 0x20, 0x63, 0x68, 0x61, 0x72, 0x61, 0x63, 0x74, 0x65, 0x72, 0x73}, 18);
        return 18;
    case 0xFE:
        out[0] = ' ';
        return 1;
    default:
        out[0] = '?';
        return 1;
    }
}

static const char *const GBA_CHAR_MAP[16][16] = {
    {" ", "À", "Á", "Â", "Ç", "È", "É", "Ê", "Ë", "Ì", "", "Î", "Ï", "Ò", "Ó", "Ô"},
    {"Œ", "Ù", "Ú", "Û", "Ñ", "ß", "à", "á", "", "ç", "è", "é", "ê", "ë", "ì", ""},
    {"î", "ï", "ò", "ó", "ô", "œ", "ù", "ú", "û", "ñ", "º", "ª", "ᵉʳ", "&", "+", ""},
    {"", "", "", "", "Lv", "=", ";", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"▯", "¿", "¡", "ᴾₖ", "ᴹₙ", "[0x55]", "[0x56]", "[0x57]", "[0x58]", "[0x59]", "Í", "%", "(", ")", "", ""},
    {"", "", "", "", "", "", "", "", "â", "", "", "", "", "", "", "í"},
    {"", "", "", "", "", "", "", "", "", "↑", "↓", "←", "→", "*", "*", "*"},
    {"*", "*", "*", "*", "ᵉ", "<", ">", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"ʳᵉ", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "!", "?", ".", "-", "･"},
    {"‥", "“", "”", "‘", "'", "♂", "♀", "$", ",", "×", "/", "A", "B", "C", "D", "E"},
    {"F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U"},
    {"V", "W", "X", "Y", "Z", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k"},
    {"l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "►"},
    {":", "Ä", "Ö", "Ü", "ä", "ö", "ü", "\x00", "\x00", "\x00", "\x00", "\x00", "\x00", "\x00", " ", "\x00"}};


void WriteHexByte(char *out, u8 val)
{
    static const char hex[] = "0123456789ABCDEF";
    *out++ = '[';
    *out++ = '0';
    *out++ = 'x';
    *out++ = hex[(val >> 4) & 0xF];
    *out++ = hex[val & 0xF];
    *out++ = ']';
    *out = '\0';
}


void ConvertGfStringToUtf8(const u8 *gbaInput, char *utf8Output)
{
    while (*gbaInput != GBA_END)
    {
        // Check for Pokeblock sequence (0x55, 0x56, 0x57, 0x58)
        if (gbaInput[0] == 0x55 && 
            gbaInput[1] == 0x56 && 
            gbaInput[2] == 0x57 && 
            gbaInput[3] == 0x58 && 
            gbaInput[4] == 0x59)
        {
            const char *pokeblock = "Pokéblock";
            size_t len = strlen(pokeblock);
            memcpy(utf8Output, pokeblock, len);
            utf8Output += len;
            gbaInput += 5;
            continue;
        }
        uint8_t val = *gbaInput++;
        const char *utf8 = GBA_CHAR_MAP[val >> 4][val & 0xF];
        if (utf8 && *utf8)
        {
            size_t len = strlen(utf8);
            memcpy(utf8Output, utf8, len);
            utf8Output += len;
        }
#if PRINT_UNMAPPED_BYTES
        else
        {
            WriteHexByte(utf8Output, val);
            utf8Output += 6;
        }
#endif
    }
    *utf8Output = '\0';
}
