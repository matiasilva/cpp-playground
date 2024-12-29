CFLAGS := -g -Wall
CC := clang
BIN := defn
SOURCES := wordapi.c

all: $(BIN)

%: %.c
	$(CC) $(CFLAGS) $< $(SOURCES) -lcurl -o $@

clean:
	rm -rf $(BIN) *.dSYM
