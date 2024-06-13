CC=gcc
CFLAGS=-Iinclude
LIB_SOURCES=src/pread.c
LIB_OBJECTS=$(LIB_SOURCES:.c=.o)
CLI_SOURCE=cli/main.c
CLI_BINARY=bin/cli

# Detect OS for setting file extension and command syntax
ifeq ($(OS),Windows_NT)
    TARGET_LIB_STATIC=lib/libpread.lib
    TARGET_LIB_SHARED=lib/libpread.dll
    CLI_BINARY=bin/cli.exe
    DLLFLAG=-Wl,--out-implib,lib/libpread.dll.a
else
    TARGET_LIB_STATIC=lib/libpread.a
    TARGET_LIB_SHARED=lib/libpread.so
    CLI_BINARY=bin/cli
    DLLFLAG=
endif

.PHONY: all clean libraries cli check

all: libraries cli

libraries: $(TARGET_LIB_STATIC) $(TARGET_LIB_SHARED)

$(TARGET_LIB_STATIC): $(LIB_OBJECTS)
	ar rcs $@ $^

$(TARGET_LIB_SHARED): $(LIB_OBJECTS)
	$(CC) -shared -o $@ $^ $(CFLAGS) $(DLLFLAG)

cli: $(CLI_SOURCE) $(TARGET_LIB_STATIC)
	$(CC) -o $(CLI_BINARY) $(CLI_SOURCE) -Llib -lpread $(CFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

check: all
	$(MAKE) -C src/tests check

clean:
	rm -f src/*.o cli/*.o $(TARGET_LIB_STATIC) $(TARGET_LIB_SHARED) $(CLI_BINARY)
	$(MAKE) -C src/tests clean
