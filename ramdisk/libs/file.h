#ifndef __FILE_H
#define __FILE_H

#include <LinkedList.h>

typedef struct VFS_device VFS_device_t;
typedef struct VFS_node FILE;

struct VFS_device
{
    char mount_id;
    unsigned char type;
    FILE *rootNode;
    LinkedList *files;
    unsigned int ram_addr;
    unsigned int (*read) (FILE *node, char *dest, unsigned int size, unsigned int off);
};

struct VFS_node
{
	unsigned int file_size;
    char filename[100];
    FILE *parentNode;
    unsigned char file_type;
    unsigned int idx;
    unsigned int mode;
    unsigned int permissions;
    unsigned int user_id;
    unsigned int group_id;
    VFS_device_t *device;
};

#endif
