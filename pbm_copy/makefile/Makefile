CC=gcc
SOURCE=./src
BIN=./bin
OUTPUT=$(BIN)/pbm_writer.o

CFLAGS=-std=c11

MKDIR= mkdir -p

all: $(OUTPUT)

$(OUTPUT):  $(SOURCE)/main.c
	    $(MKDIR) $(BIN)
	    $(CC) $(INC) $< $(CFLAGS) -o $(OUTPUT) $(LIBS)
