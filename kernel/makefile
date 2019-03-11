TARGET_NAME := kernel
all: build

include ../mkfiles/toolset.mk
include ../mkfiles/dirs.mk
include ../mkfiles/rules.mk

TARGET_BINARY := $(DIR_BIN)/starrios.sys

build: $(TARGET_BINARY)

$(TARGET_BINARY): $(TARGET_ELF)
	$(TOOLSET_OBJCOPY) -O binary -j .stage -j .text -j .rodata -j .data -j .bss -S -g $< $@
