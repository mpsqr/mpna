# Compiler and flags
CC = gcc
CFLAGS = -Iinclude -std=c11

# Directories
SRC_DIR = src
INC_DIR = include
BIN_DIR = bin

# Target executable
TARGET = $(BIN_DIR)/main_pi

# Source files and object files
SRCS = $(shell find $(SRC_DIR) -name '*.c')
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRCS))

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(OBJS) -lm -o $(TARGET)

# Compilation
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create bin directory if it doesn't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean build artifacts
clean:
	rm -rf $(BIN_DIR)

# Phony targets
.PHONY: all clean
