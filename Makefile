CC = gcc
CFLAGS = -Wall 
LIBS = -lcrypto -lssl

SRC = src/main.c
EXE = main

all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(EXE) $(LIBS)

clean:
	rm -f $(EXE)

.PHONY: all clean

