CC=gcc
SOURCE_DIR=./src
BIN_DIR=./bin
OUTPUT=$(BIN_DIR)/pbm_writer.o

SOURCE= $(SOURCE_DIR)/main.c

CFLAGS=-std=c11

MKDIR= mkdir -p

all: $(OUTPUT)

$(OUTPUT):  $(SOURCE)
	    $(MKDIR) $(BIN_DIR)
	    $(CC) $(INC) $< $(CFLAGS) -o $(OUTPUT) $(LIBS)

.PHONY: clean
clean:
	rm -f $(OUTPUT)
