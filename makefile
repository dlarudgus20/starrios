all: build

include mkfiles/toolset.mk
include mkfiles/dirs.mk

TOOLS_EDIMG := tools/edimg/edimg

BOOTLOADER_BOOT := bootloader/$(DIR_BIN)/boot.bin
BOOTLOADER_STAGE := bootloader/$(DIR_BIN)/stage.bin
KERNEL_BINARY := kernel/$(DIR_BIN)/starrios.sys

RAW_IMAGE := img/raw_floppy.img
TARGET_IMAGE := $(DIR_BIN)/floppy.img

QEMU_FLAGS := -L . -m 64 -fda $(TARGET_IMAGE) -boot a -localtime -M pc
BOCHSRC := bochsrc.bxrc

.PHONY: all build re rebuild run rerun dbg debug mostlyclean clean distclean

build:
	make -C tools
	make build -C bootloader
	make build -C kernel
	make $(TARGET_IMAGE)

re: rebuild
rebuild: clean build

run: build
	$(TOOLSET_QEMU) $(QEMU_FLAGS)

dbg: debug
debug: build
	$(TOOLSET_BOCHS) -qf $(BOCHSRC) "debug_symbols: file=\"bootloader/$(DIR_OBJ)/bootloader.sym\""

mostlyclean:
	make mostlyclean -C bootloader
	make mostlyclean -C kernel

clean:
	make clean -C bootloader
	make clean -C kernel
	-rm -rf $(DIR_BIN)/*

distclean:
	make distclean -C bootloader
	make distclean -C kernel
	make distclean -C tools
	-rm -rf $(DIR_BIN)

$(TARGET_IMAGE): $(BOOTLOADER_BOOT) $(BOOTLOADER_STAGE) $(KERNEL_BINARY)
	mkdir -p $(DIR_BIN)
	$(TOOLS_EDIMG) imgin:$(RAW_IMAGE) \
		wbinimg src:$(BOOTLOADER_BOOT) len:512 from:0 to:0 \
		copy from:$(BOOTLOADER_STAGE) to:@: \
		copy from:$(KERNEL_BINARY) to:@: \
		imgout:$(TARGET_IMAGE)
