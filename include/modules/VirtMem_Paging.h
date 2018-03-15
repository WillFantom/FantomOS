#ifndef __VIRTMEMPAGING_H
#define __VIRTMEMPAGING_H

#include <core/Common.h>
#include <core/Memory.h>

//Inital Page Dir in FantomOS
bool PageSizeExtension_enabled;

//Standard x86 Paging Structures
typedef struct PTE
{
    uint32_t present : 1;
    uint32_t read_write_flag : 1;
    uint32_t mode_flag : 1;
    uint32_t res : 2;
    uint32_t access_flag : 1;
    uint32_t dirty_flag : 1;
    uint32_t res2 : 2;
    uint32_t avaliable : 3;
    uint32_t page_address : 20;
} __attribute__((packed)) page_table_entry_t;

typedef struct page_table
{
    page_table_entry_t entry[1024];
} page_table_t;

typedef struct PDE
{
    uint32_t present : 1;
    uint32_t read_write_flag : 1;
    uint32_t mode_flag : 1;
    uint32_t write_through_flag : 1;
    uint32_t cached : 1;
    uint32_t access_flag : 1;
    uint32_t res : 1;
    uint32_t page_size : 1;
    uint32_t global : 1;
    uint32_t avaliable : 3;
    uint32_t page_address : 20;
} __attribute__((packed)) page_dir_entry_t;

typedef struct page_dir
{
    page_dir_entry_t entry[1024];
} page_dir_t;

//For mapping to current
#define TEMP_VADDR 0xC0000000

//Required Page Dir (Kernel)
page_dir_t *kdir;

//Functions
void VirtMem_disablePSE();
void VirtMem_invalidateTLBE(uint32_t vaddress);
uint32_t *VirtMem_mapToCurrent(uint32_t paddr, uint8_t page);
void VirtMem_initPDE(page_dir_entry_t *dir_e, page_table_t *table, uint8_t readWrite, uint8_t priv);
void VirtMem_initPTE(page_table_entry_t *table_e, uint32_t address, uint8_t readWrite, uint8_t priv);
uint32_t VirtMem_virt2Phys(page_dir_t *dir, uint32_t vaddr);
page_dir_t *VirtMem_createDir(uint32_t paddr);
void VirtMem_freeDir(page_dir_t * dir);
uint32_t VirtMem_mapPage(page_dir_t *dir, uint32_t vaddr, uint32_t paddr, uint8_t readWrite, uint8_t priv);
void VirtMem_unmapPage(page_dir_t *dir, uint32_t vaddr, bool free_phys);
void VirtMem_cpyDir(page_dir_t *srcDir, page_dir_t *dstDir, bool user);
page_dir_t *VirtMem_getKDIR();
bool VirtMem_Paging_init();

#endif
