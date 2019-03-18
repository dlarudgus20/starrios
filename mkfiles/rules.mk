ifndef TARGET_NAME
$(error target name must be specified)
endif

C_SOURCES := $(wildcard $(DIR_SRC)/*.c)
C_OBJECTS := $(patsubst $(DIR_SRC)/%.c, $(DIR_OBJ)/%.c.o, $(C_SOURCES))

AS_SOURCES := $(wildcard $(DIR_SRC)/*.S)
AS_OBJECTS := $(patsubst $(DIR_SRC)/%.S, $(DIR_OBJ)/%.S.o, $(AS_SOURCES))

LD_SCRIPT := $(DIR_SRC)/linker.ld

TARGET_ELF := $(DIR_BIN)/$(TARGET_NAME).elf

.PHONY: all build rebuild mostlyclean clean

rebuild: clean build

mostlyclean:
	-rm -rf $(DIR_OBJ)

clean: mostlyclean
	-rm -rf $(DIR_BIN)

distclean: clean clean_dirs

$(TARGET_ELF): $(LD_SCRIPT) $(C_OBJECTS) $(AS_OBJECTS) | $(DIRS)
	$(TOOLSET_GCC) $(LDFLAGS) -T $(LD_SCRIPT) -o $@ $(C_OBJECTS) $(AS_OBJECTS) \
		-Xlinker -Map=$(DIR_OBJ)/$(TARGET_NAME).map
	$(TOOLSET_NM) $(NM_FLAGS) $@ > $(DIR_OBJ)/$(TARGET_NAME).nm
	$(TOOLSET_OBJDUMP) $(OBJDUMP_FLAGS) -D $@ > $(DIR_OBJ)/$(TARGET_NAME).disasm

	$(TOOLSET_NM) --line-numbers --numeric-sort $@ \
		| sed '/.*/ s/\(.*\) . \(.*\)/\1 \2/g' > $(DIR_OBJ)/$(TARGET_NAME).sym

$(DIR_OBJ)/%.c.o: $(DIR_SRC)/%.c | $(DIRS)
	$(TOOLSET_GCC) $(CFLAGS) -c $< -o $@
	$(TOOLSET_OBJDUMP) $(OBJDUMP_FLAGS) -D $@ > $(DIR_OBJ)/$*.c.dump

$(DIR_OBJ)/%.S.o: $(DIR_SRC)/%.S | $(DIRS)
	$(TOOLSET_GCC) $(ASFLAGS) -c $< -o $@ \
		-Xassembler -a=$(DIR_OBJ)/$*.S.lst
	$(TOOLSET_OBJDUMP) $(OBJDUMP_FLAGS) -D $@ > $(DIR_OBJ)/$*.S.dump
