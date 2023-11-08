# Declare variables
CC = gcc
CFLAGS = -Wall
READER = Reader
WRITER = Writer

# Add phony
.PHONY: all

# Compile Reader.c and Writer.c to individual binaries.
$(READER): $(READER).c
	$(CC) $(CFLAGS) -o $(READER) $(READER).c

$(WRITER): $(WRITER).c
	$(CC) $(CFLAGS) -o $(WRITER) $(WRITER).c

# Clean up
clean:
	rm -f $(READER) $(WRITER)

# Default target
all: $(READER) $(WRITER)

# End of Makefile
