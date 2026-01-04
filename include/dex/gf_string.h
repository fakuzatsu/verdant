#ifndef GF_STRING
#define GF_STRING

#include "global.h"

// Converts one GF character to UTF-8. Writes to `out`, returns number of bytes.
int ConvertGfCharToUtf8(u8 gfChar, u8 *out);

// Converts a full GF-encoded string to a UTF-8 string.
void ConvertGfStringToUtf8(const u8 *gfInput, char *utf8Output);

void WriteHexByte(char *out, u8 val);

#endif // GF_STRING