TARGET_NAME := kernel
all: build

include ../mkfiles/toolset.mk
include ../mkfiles/conf.mk
include ../mkfiles/rules.mk

TARGET_BINARY := $(DIR_BIN)/kernel.sys

build: $(TARGET_BINARY)

$(TARGET_BINARY): $(TARGET_ELF)
	$(TOOLSET_OBJCOPY) -O binary -j .startup -j .text -j .rodata -j .data -j .bss -S -g $< $@
