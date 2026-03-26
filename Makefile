# Makefile for toller

CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -pedantic -O2
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin

# Target executable
TARGET := $(BIN_DIR)/toller

# Source files - recursively find all .c files in SRC_DIR and subdirectories
SOURCES := $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*/*.c $(SRC_DIR)/*/*/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

# Include directories
INCLUDES := -I$(SRC_DIR) -I$(SRC_DIR)/file_systems -I$(SRC_DIR)/utils/log

# Default target
.PHONY: all
all: $(TARGET)

# Create necessary directories
$(BUILD_DIR) $(BIN_DIR):
	@mkdir -p $@

# Link object files to create executable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^
	@echo "Build complete: $@"

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

# Clean build artifacts
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "Clean complete"

# Run the program
.PHONY: run
run: all
	./$(TARGET)

# Run with arguments
.PHONY: run-help
run-help: all
	./$(TARGET) --help

.PHONY: run-version
run-version: all
	./$(TARGET) --version

# Display help
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all          - Build the project (default)"
	@echo "  clean        - Remove build artifacts"
	@echo "  run          - Build and run the program"
	@echo "  run-help     - Run with --help"
	@echo "  run-version  - Run with --version"
	@echo "  help         - Show this message"
