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

# Clean up build artifacts
clean:
	rm -f $(TARGET)
