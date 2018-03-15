
//ME
#include <modules/RamDisk.h>

//REQUIREMENTS
#include <modules/TarParser.h>
#include <modules/LinkedList.h>
#include <modules/HeapMem_SBRK.h>
#include <modules/VFS.h>

//CORE
#include <core/Common.h>
#include <core/Kernel.h>
#include <core/Memory.h>
#include <core/Debug.h>

/* Tar Based Ramdisk */

uint32_t RamDisk_getDepth(const char *path)
{
    uint32_t d = 0;

    for(d = 0 ; path[d] ; path[d] == '/' ? d++ : (uint32_t)path++);

    return d;
}

char *RamDisk_onlyFileName(char* restrict dest, const char* restrict src) {
    const uint8_t* s = (uint8_t*) src;
    uint8_t* d = (uint8_t*) dest;

    /*uint32_t depth = RamDisk_getDepth(src);
    Debug_kprintf("Depth %d\n", depth);
    while(depth > 0)
        if(*src++ == '/')
            depth--;*/

    while ((*d++ = *s++));
    return dest;
}

int RamDisk_filenameCmp(const char* sL, const char* sR)
{
    while(*sL && (*sL == *sR))
    {
        sL++;
        sR++;
    }
    return *(const unsigned char*)sL - *(const unsigned char*)sR;
}

void RamDisk_scanTar(Tar_header_t *header, VFS_device_t *device)
{
    LinkedList *list = device->files;

    LinkedList_addNode(list, device->rootNode);

    uint32_t scans = 0;

    while(1)
    {
        //Is End / Valid
        if(header->filename[0] == '\0')
            break;

        //List
        VFS_node_t *node = calloc(1, sizeof(VFS_node_t));
        LinkedList_addNode(list, node);

        RamDisk_onlyFileName(node->filename, header->filename);
        node->file_size = Tar_getInt(header->size, 12, 8);
        node->device = device;
        node->idx = scans++;

        header = Tar_nextHeader(header);

        if(header == NULL)
            break;
    }
}

uint32_t RamDisk_readFile(VFS_node_t *node, char *dest, uint32_t size, uint32_t off)
{
    //kprintf("Soon to be Copied\n\n");
    Tar_header_t *header = Tar_getHeaderAtIndex((Tar_header_t *)node->device->ram_addr, node->idx);
    uint32_t addr = (uint32_t)header + off + TAR_BLOCKSIZE;


    memcpy(dest, addr, size);
    dest[size] = '\0';
    //kprintf("Copied\n\n");

    return size;
}

bool RamDisk_init()
{
    uint32_t ramdiskEntryAddress = 0;

    if(mb_mods == NULL)
        return;
    else
        ramdiskEntryAddress = (uint32_t)mb_mods[0] + KBASE_VADDRESS;

    //Parse Tar Archive
    uint32_t fileCount = Tar_countHeaders((Tar_header_t *)ramdiskEntryAddress);


    VFS_device_t *device = calloc(1, sizeof(VFS_device_t));
    VFS_node_t *rootNode = calloc(1, sizeof(VFS_node_t));
    LinkedList *files = LinkedList_create();

    //Device
    device->type = FS_DEVICE_TYPE_RD;
    device->rootNode = rootNode;
    device->files = files;
    device->ram_addr = ramdiskEntryAddress;
    device->read = RamDisk_readFile;

    VFS_mount(device);

    //Root
    rootNode->filename[0] = device->mount_id;
    rootNode->filename[1] = '/';
    rootNode->filename[2] = '\0';
    rootNode->parentNode = NULL;
    rootNode->file_size = 0;
    rootNode->mode = VFSMODE_CLOSED;
    rootNode->permissions = 0;
    rootNode->user_id = 0;
    rootNode->group_id = 0;
    rootNode->device = device;

    //Scan Archive
    RamDisk_scanTar((Tar_header_t *)ramdiskEntryAddress, device);

    kprintf("RamDisk %s : Tar Archive : %d Files Found\n", rootNode->filename, fileCount);

    return true;
}
