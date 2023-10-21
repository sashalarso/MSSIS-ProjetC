CC = gcc
CFLAGS = -Wall -g 
LIBS = -lcrypto -lssl

SRC = src/main.c src/lookup.c src/load_table.c src/freetree.c src/insert.c src/search.c src/hash_sha256.c
EXE = main

all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(EXE) $(LIBS)

clean:
	rm -f $(EXE)

.PHONY: all clean

