#ifndef __VFS_H
#define __VFS_H

#include <core/Common.h>
#include <modules/LinkedList.h>

#define VFSMODE_OPEN 0
#define VFSMODE_CLOSED 1

#define VFSTYPE_NORMAL 1
#define VFSTYPE_DIR 2

#define FS_DEVICE_TYPE_RD 1

LinkedList *FS_mounts;

typedef struct VFS_device VFS_device_t;
typedef struct VFS_node VFS_node_t;

struct VFS_device
{
    char mount_id;
    uint8_t type;
    VFS_node_t *rootNode;
    LinkedList *files;
    uint32_t ram_addr;
    uint32_t (*read) (VFS_node_t *node, char *dest, uint32_t size, uint32_t off);
};

struct VFS_node
{
    uint32_t file_size;
    char filename[100];
    VFS_node_t *parentNode;
    uint8_t file_type;
    uint32_t idx;
    uint32_t mode;
    uint32_t permissions;
    uint32_t user_id;
    uint32_t group_id;
    VFS_device_t *device;
};

struct VFS_user_node
{
    char filename[98];
    unsigned int filesize;
    unsigned int user;
    unsigned int group;
    unsigned int permissions;

};

typedef struct FILE_NODE
{
	char filename[100];
	unsigned int fileSize;
	unsigned int node_ptr;
} FILE;

//Functions
bool VFS_init();
bool VFS_mount(VFS_device_t *device);
bool VFS_unmount(VFS_device_t *device);
VFS_node_t *VFS_open(const char *filename, const char *mode);
void VFS_fileInfo(VFS_node_t *node, char *dest);
uint32_t VFS_read(VFS_node_t *node, char *dest, uint32_t size, uint32_t off);
uint32_t VFS_getFileSize(const char *filename);
char *VFS_getFileAtIndex(uint32_t index);

#endif
