//ME
#include <modules/vfs/VFS.h>

//REQUIREMENTS
#include <modules/data_structs/LinkedList.h>

//CORE
#include <core/boot/Kernel.h>
#include <core/debug/Common.h>
#include <core/debug/Debug.h>

bool VFS_init()
{
    FS_mounts = LinkedList_create();
    return true;
}

char VFS_nextMountId()
{
    char curr = 'C';
    for(uint32_t mp = 0 ; mp < FS_mounts->items ; mp++)
    {
        if(((VFS_device_t *)(LinkedList_getNodeByIndex(FS_mounts, mp))->data)->mount_id != curr)
            return curr;
        else
            curr += 1;

        if(curr > 'Z')
            return NULL;
    }
    return curr;
}

bool VFS_mount(VFS_device_t *device)
{
    char id = VFS_nextMountId();

    if(id == NULL)
        return false;
    else
        device->mount_id = id;

    for(uint32_t mp = 0 ; mp < FS_mounts->items ; mp++)
    {
        if(device->rootNode == ((VFS_device_t *)(LinkedList_getNodeByIndex(FS_mounts, mp))->data)->rootNode)
        {
            return false;
        }
    }

    LinkedList_addNode(FS_mounts, device);

    return true;
}

bool VFS_unmount(VFS_device_t *device)
{
    for(uint32_t mp = 0 ; mp < FS_mounts->items ; mp++)
    {
        if(((VFS_device_t *)(LinkedList_getNodeByIndex(FS_mounts, mp)->data))->mount_id == device->mount_id)
        {
            LinkedList_removeNode(FS_mounts, LinkedList_getNodeByIndex(FS_mounts, mp));
            return true;
        }
    }
    return false;
}

uint32_t VFS_nameCmp(const char *a, const char *b)
{
    while(*a && (*a == *b))
    {
        a++;
        b++;
    }
    return *(const unsigned char*)a - *(const unsigned char*)b;
}

VFS_node_t *VFS_findNode(VFS_device_t *device, const char *filename)
{
    LinkedList *list = device->files;

    for(uint32_t f = 0 ; f < list->items ; f++)
    {
        if(!VFS_nameCmp(filename, ((VFS_node_t *)(LinkedList_getNodeByIndex(list, f)->data))->filename))
            return (VFS_node_t *)(LinkedList_getNodeByIndex(list, f)->data);
    }

    return NULL;
}

VFS_node_t *VFS_open(const char *filename, const char *mode)
{
    for(uint32_t d = 0 ; d < FS_mounts->items ; d++)
    {
        VFS_node_t *node = VFS_findNode((VFS_device_t *)(LinkedList_getNodeByIndex(FS_mounts, d)->data), filename);
        if(node != NULL)
            return node;
    }
    return NULL;
}

void VFS_fileInfo(VFS_node_t *node, char *dest)
{
    memcpy(dest, node, sizeof(VFS_node_t));
    return NULL;
}

uint32_t VFS_getFileSize(const char *filename)
{
    for(uint32_t d = 0 ; d < FS_mounts->items ; d++)
    {
        VFS_node_t *node = VFS_findNode((VFS_device_t *)(LinkedList_getNodeByIndex(FS_mounts, d)->data), filename);
        if(node != 0)
            return node->file_size + 1;
    }
    return 0;
}

char *VFS_getFileAtIndex(uint32_t index)
{
    uint32_t i = 0;
    for(uint32_t d = 0 ; d < FS_mounts->items ; d++)
    {
        LinkedList *list = ((VFS_device_t *)(LinkedList_getNodeByIndex(FS_mounts, d))->data)->files;
        for(uint32_t f = 0 ; f < list->items ; f++)
        {
            if(i == index)
                return ((VFS_node_t *)(LinkedList_getNodeByIndex(list, f)->data))->filename;
            i++;
        }
    }
    return NULL;
}

uint32_t VFS_read(VFS_node_t *node, char *dest, uint32_t size, uint32_t off)
{
    //kprintf("Going to copy\n");
    node->device->read(node, dest, size, off);
}
