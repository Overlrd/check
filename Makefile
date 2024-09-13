# Clean up build artifacts
.PHONY: clean

# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra
TARGET := main

# Default target
all: $(TARGET)

# Build target
$(TARGET): main.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGET)
