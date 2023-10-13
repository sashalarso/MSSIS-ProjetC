CC = gcc
CFLAGS = -Wall 
LIBS = -lcrypto -lssl

SRC = test.c
EXE = test

all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(EXE) $(LIBS)

clean:
	rm -f $(EXE)

.PHONY: all clean

