TARGET_NAME := bootloader
all: build

OBJDUMP_FLAGS := -Mintel,i8086

include ../mkfiles/toolset.mk
include ../mkfiles/dirs.mk
include ../mkfiles/rules.mk

BOOT := $(DIR_BIN)/boot.bin
STAGE := $(DIR_BIN)/stage.bin

build: $(BOOT) $(STAGE)

$(BOOT): $(TARGET_ELF)
	$(TOOLSET_OBJCOPY) -O binary -j .boot -S -g $< $@

$(STAGE): $(TARGET_ELF)
	$(TOOLSET_OBJCOPY) -O binary -j .stage -S -g $< $@
