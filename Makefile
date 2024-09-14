.PHONY: clean

# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra
TARGET := check

# Default target
all: $(TARGET)

# Build target
$(TARGET): check.c
	$(CC) $(CFLAGS) $< -o $@

# Clean up build artifacts
clean:
	rm -f $(TARGET)
