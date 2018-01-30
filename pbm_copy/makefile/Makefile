CC=gcc
SOURCE_DIR=./src
INC +=-I ../common
BIN_DIR=./bin
OUTPUT=$(BIN_DIR)/pbm_writer.o

SOURCE= $(SOURCE_DIR)/*.c

CFLAGS=-std=c11

MKDIR= mkdir -p

all: $(OUTPUT)

$(OUTPUT):  $(SOURCE)
	    $(MKDIR) $(BIN_DIR)
	    $(CC) $(INC) $(SOURCE) $(CFLAGS) -o $(OUTPUT) $(LIBS)

.PHONY: clean
clean:
	rm -f $(OUTPUT)
