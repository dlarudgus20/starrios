DIR_SRC := src
DIR_BIN := bin
DIR_OBJ := obj
DIRS := $(DIR_BIN) $(DIR_OBJ)

$(DIRS):
	mkdir -p $(DIR_BIN)
	mkdir -p $(DIR_OBJ)

clean_dirs:
	-rm -rf $(DIR_BIN)
	-rm -rf $(DIR_OBJ)
