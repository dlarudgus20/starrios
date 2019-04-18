TARGET_NAME := bootloader
all: build

OBJDUMP_FLAGS += -Mi8086

include ../mkfiles/toolset.mk
include ../mkfiles/conf.mk
include ../mkfiles/rules.mk

BOOT := $(DIR_BIN)/boot.bin

build: $(BOOT) $(STAGE)

$(BOOT): $(TARGET_ELF)
	$(TOOLSET_OBJCOPY) -O binary -j .boot -S -g $< $@
