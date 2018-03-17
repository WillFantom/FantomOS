
#include <modules/ramdisk/TarParser.h>
#include <core/debug/Common.h>
#include <core/debug/Debug.h>

uint32_t Tar_getInt(const char *str, uint32_t len, uint32_t format)
{
    const char* ip;
    uint32_t num = 0;
    int v = len;

    for (ip = str; (*ip >= '0' && *ip <= '9') || (format > 10 && (*ip >= 'a' && *ip <= 'f')); ip++) {

        uint32_t offset = (*ip > '9') ? *ip - 'a' + 10 : *ip - '0';
        num = num * format + offset;
        len--;
        if(len == 0)
            return num;
    }

    return num;
}

bool Tar_validHeader(const Tar_header_t *currentHeader)
{
    uint8_t checksum = Tar_getInt(currentHeader->checksum, 8, 8);
    uint32_t* address = (uint32_t*) currentHeader;

    for (uint32_t i = 0; i < TAR_BLOCKSIZE; i++)
        checksum -= (i >= 148 && i < 156) ? 32 : address[i];

    return !checksum;
}

Tar_header_t *Tar_nextHeader(const Tar_header_t *currentHeader)
{
    //Get length of current
    uint32_t length = Tar_getInt(currentHeader->size, 12, 8);

    //Get end address of current (ROUND TO BLOCK)
    uint32_t addr = (uint32_t)currentHeader + (((length / TAR_BLOCKSIZE) + 1) * TAR_BLOCKSIZE);

    if((uint32_t)length % TAR_BLOCKSIZE)
        addr += TAR_BLOCKSIZE;

    currentHeader = (Tar_header_t *)addr;

    return currentHeader;
}

uint32_t Tar_countHeaders(Tar_header_t *header)
{
    uint32_t count = 0;
    while(1)
    {
        if(header->filename[0] == '\0')
            return count;

        header = Tar_nextHeader(header);
        count++;
        if(header == NULL)
            return count;
    }
    return NULL;
}

Tar_header_t *Tar_getHeaderAtIndex(const Tar_header_t *first, uint32_t index)
{
    KASSERT(index < Tar_countHeaders(first));

    for(uint32_t i = 0 ; i < index ; i++)
    {
        first = Tar_nextHeader(first);
    }
    return first;
}
