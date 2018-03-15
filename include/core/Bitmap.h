#ifndef __BITMAP_H
#define __BITMAP_H

#include <core/Common.h>

typedef struct Bitmap
{
    void *base;
    uint32_t bytes;
} bitmap_t;

bool Bitmap_checkBit(bitmap_t *bitmap, uint32_t bit);
void Bitmap_setBit(bitmap_t *bitmap, uint32_t bit);
void Bitmap_clearBit(bitmap_t *bitmap, uint32_t bit);
void Bitmap_clear(bitmap_t *bitmap);
void Bitmap_setAll(bitmap_t *bitmap);

#endif
