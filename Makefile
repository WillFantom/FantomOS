#############################
##  MAKEFILE FOR FANTOMOS  ##
#############################
##  Author - Will Fantom

##  NOTE: not for user space or Ramdisk (Use bash file)

## COMPILERS
ASM_COMPILER = nasm
C_COMPILER = i686-elf-gcc

##  Directories
BUILD_DIR = ./build
IMG_DIR = ./bin

##  Names
OS_NAME = FantomOS
IMG_NAME = $(OS_NAME).bin
ISO_NAME = $(OS_NAME).iso

## Build Flags
ASM_FLAGS = -f elf32
C_FLAGS = -fno-stack-protector -w -std=c99 -fno-builtin -m32 -I packages/

##  Link Flags
LINK_FLAGS = -m elf_i386 -Map $(IMG_DIR)/memory.map

##  Core Files
C_CORE_SRC = $(wildcard packages/core/**/*.c)
ASM_CORE_SRC = $(wildcard packages/core/**/*.s)

##  Module Files
C_MODULES_SRC = $(wildcard packages/modules/**/*.c)
ASM_MODULES_SRC = $(wildcard packages/modules/**/*.s)

##  Core Objects
C_CORE_OBJ = $(C_CORE_SRC:.c=.o)
ASM_CORE_OBJ = $(ASM_CORE_SRC:.s=.o)

##  Module Obkects
C_MODULES_OBJ = $(C_MODULES_SRC:.c=.o)
ASM_MODULES_OBJ = $(ASM_MODULES_SRC:.s=.o)

##  Objects
OBJECTS = $($(ASM_CORE_OBJ) $(C_MODULES_OBJ) $(C_CORE_OBJ))

##  RD
RD_SCRIPT = makeRD.sh

##  Make
all: createDirs buidlDisk link iso run

##  Create Directories
createDirs:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(IMG_DIR)
	@mkdir -p $(BUILD_DIR)/iso
	@mkdir -p $(BUILD_DIR)/iso/boot
	@mkdir -p $(BUILD_DIR)/iso/boot/grub

##  Build the RamDisk
buidlDisk: 
	cd ramdisk
	$(./RD_SCRIPT)
	cd ..

##  Compile & Link
link: $(ASM_CORE_OBJ) $(C_CORE_OBJ) $(ASM_MODULES_OBJ) $(C_MODULES_OBJ) 
	@echo "Linking"
	@ld -T linker.ld $(LINK_FLAGS) $(foreach file,$(^F),$(BUILD_DIR)/$(file)) -o $(IMG_DIR)/$(IMG_NAME)

##  Convert to Objects
.c.o:
	@$(C_COMPILER) $(C_FLAGS) -c $< -o $(BUILD_DIR)/$(@F)

.s.o:
	@$(ASM_COMPILER) $(ASM_FLAGS) $< -o $(BUILD_DIR)/$(@F)

##  Create ISO
iso:
	@cp $(IMG_DIR)/$(IMG_NAME) $(BUILD_DIR)/iso/boot/$(IMG_NAME)
	@cp grub.cfg $(BUILD_DIR)/iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO_NAME) $(BUILD_DIR)/iso 

##  Run on QEMU
run:
	@echo "Booting FantomOS"
	qemu-system-i386 -cdrom $(ISO_NAME) -serial file:COM1.log -m 4G -monitor stdio 

##  Clean
clean:
	@rm -R -f $(BUILD_DIR)
	@rm -R -f $(IMG_DIR)
	@rm com1.log
	@rm FantomOS.iso
