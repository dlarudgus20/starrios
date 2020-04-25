TARGET_NAME := boot
all: build

ASFLAGS += $(shell echo -DSTAGE_SECTORS=$$(((`stat --printf="%s" ../kernel/bin/$(CONFIG)/kernel.sys` + 511) / 512 )))
OBJDUMP_FLAGS += -Mi8086

include ../mkfiles/toolset.mk
include ../mkfiles/conf.mk
include ../mkfiles/rules.mk

BOOT := $(DIR_BIN)/boot.bin

build: $(BOOT) $(STAGE)

$(BOOT): $(TARGET_ELF)
	$(TOOLSET_OBJCOPY) -O binary -j .boot -S -g $< $@
