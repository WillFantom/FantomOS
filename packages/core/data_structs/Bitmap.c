/*  - Bitmap.c
    AUTHOR: Will Fantom
    DESC:   A data storage type, sutibale for binary options.
*/

#include <core/data_structs/Bitmap.h>
#include <core/debug/Common.h>

bool Bitmap_checkBit(bitmap_t *bitmap, uint32_t bit)
{
    uint8_t *byte = bitmap->base + (bit / 8);
    return *byte & (0x1 << (bit % 8));
}

void Bitmap_setBit(bitmap_t *bitmap, uint32_t bit)
{
    uint8_t *byte = bitmap->base + (bit / 8);
    *byte |= (0x1 << (bit % 8));
}

void Bitmap_clearBit(bitmap_t *bitmap, uint32_t bit)
{
    uint8_t *byte = bitmap->base + (bit / 8);
    *byte &= ~(0x1 << (bit % 8));
}

void Bitmap_clearAll(bitmap_t *bitmap)
{
    memset(bitmap->base, 0, bitmap->bytes);
}

void Bitmap_setAll(bitmap_t *bitmap)
{
    memset((uint32_t)bitmap->base, 0xFFFFFFFF, bitmap->bytes);
}
