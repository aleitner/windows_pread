CC=gcc
CFLAGS=-I../../include
LDFLAGS=-L../../lib -lpread

TEST_SOURCES=$(wildcard *.c)
TEST_BINARIES=$(TEST_SOURCES:.c=)

# Detect OS for setting file extension and command syntax
ifeq ($(OS),Windows_NT)
    EXEC_SUFFIX=.exe
    RM=del /Q
    ERRIGNORE=2>NUL || exit 0
else
    EXEC_SUFFIX=
    RM=rm -f
    ERRIGNORE= || exit 0
endif

.PHONY: all check clean

all: $(TEST_BINARIES)

check: all
	@for test in $(TEST_BINARIES); do \
		echo Running $$test...; \
		./$$test$(EXEC_SUFFIX); \
		if [ $$? -eq 0 ]; then \
			echo $$test passed; \
		else \
			echo $$test failed; \
			exit 1; \
		fi; \
	done

%: %.c
	$(CC) $(CFLAGS) $< -o $@$(EXEC_SUFFIX) $(LDFLAGS)

clean:
	$(RM) $(addsuffix $(EXEC_SUFFIX),$(TEST_BINARIES))
