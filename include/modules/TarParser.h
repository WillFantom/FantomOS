#ifndef __TARPARSER_H
#define __TARPARSER_H

#include <core/Common.h>

#define TAR_BLOCKSIZE 512

//Structre referenced in doc
typedef struct Tar_header
{
    char filename[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char checksum[8];
    char typeflag[1];
    char linkname[100];
    char magic[6];
    char version[2];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
} Tar_header_t;

//Functions
uint32_t Tar_getInt(const char *str, uint32_t len, uint32_t format);
Tar_header_t *Tar_nextHeader(const Tar_header_t *currentHeader);
uint32_t Tar_countHeaders(Tar_header_t *header);
Tar_header_t * Tar_getHeaderAtIndex(const Tar_header_t *first, uint32_t index);

#endif
