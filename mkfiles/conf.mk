CONFIG ?= debug

CFLAGS += -ffreestanding -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 \
	-std=c99 -Wall -Wextra -Werror -Wno-unused-parameter -fpack-struct
ASFLAGS +=
LDFLAGS += -ffreestanding -nostdlib
OBJDUMP_FLAGS +=
NM_FLAGS += --line-numbers --print-size --print-armap --numeric-sort

ifeq ($(CONFIG), debug)
CFLAGS += -DDEBUG -ggdb
ASFLAGS += -DDEBUG
else ifeq ($(CONFIG), release)
CFLAGS += -DNDEBUG -O3 -flto
ASFLAGS += -DNDEBUG
else
$(error [CONFIG] '$(CONFIG)': unknown configuration.)
endif

DIR_SRC := src
DIR_BIN := bin/$(CONFIG)
DIR_OBJ := obj/$(CONFIG)
DIRS := $(DIR_BIN) $(DIR_OBJ)

$(DIRS):
	mkdir -p $(DIR_BIN)
	mkdir -p $(DIR_OBJ)

clean_dirs:
	-rm -rf $(DIR_BIN)
	-rm -rf $(DIR_OBJ)
