CC=gcc
SOURCE_DIR=./src
COMMON_DIR=../pbm_copy/src/
INC +=-I ../common
BIN_DIR=./bin
OUTPUT=$(BIN_DIR)/pbm_bitflip.o

SOURCE= $(SOURCE_DIR)/main.c $(COMMON_DIR)/pbm_copy_common.c

CFLAGS=-std=c11

MKDIR= mkdir -p

all: $(OUTPUT)

$(OUTPUT):  $(SOURCE)
	    $(MKDIR) $(BIN_DIR)
	    $(CC) $(INC) $(SOURCE) $(CFLAGS) -o $(OUTPUT) $(LIBS)

.PHONY: clean
clean:
	rm -f $(OUTPUT)
