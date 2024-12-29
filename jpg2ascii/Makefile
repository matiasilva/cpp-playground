CFLAGS := -g -Wall
CC := clang
BIN := jpg2ascii
JPEG_TURBO := $(shell pkg-config --cflags --libs libturbojpeg)

all: $(BIN)

%: %.c
	$(CC) $(CFLAGS) $< $(JPEG_TURBO) -o $@

clean:
	rm -rf $(BIN) *.dSYM
