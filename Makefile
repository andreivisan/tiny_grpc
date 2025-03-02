# Makefile for kouka build tool

TARGET = bin/risc_v
# Compiler and Flags
CC     = gcc 
CFLAGS = -Wall -Wextra -O2 -Iinclude
LDFLAGS = -lcurl
# Source and Object files

# Directories
BIN_DIR   = bin
OBJ_DIR   = obj
TEST_DIR  = test

SRC    = $(wildcard src/*.c)
OBJ    = $(patsubst src/%.c, obj/%.o, $(SRC))
TEST_SOURCES := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJECTS := $(patsubst $(TEST_DIR)/%.c, $(TEST_DIR)/%.o, $(TEST_SOURCES))
TEST_EXES := $(TEST_SOURCES:$(TEST_DIR)/%.c=$(TEST_DIR)/%)

# Install prefix; can be overridden on the command line, e.g.,
# make PREFIX=/my/custom/path install
PREFIX    ?= /usr/local

# Phony targets
.PHONY: all clean build install test

# Default target: build the project
all: build

# Build target: compile and link the executable risc_v
build: $(TARGET)

$(TARGET): $(OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile each C source file into an object file
$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create necessary directories if they don't exist
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# Clean up build artifacts (object files and binary)
clean:
	rm -rf $(OBJ_DIR)/*
	rm -rf $(BIN_DIR)/*

# Install target: copy risc_v to $(PREFIX)/bin
install: build
	@echo "Installing risc_v to $(PREFIX)/bin"
	mkdir -p $(PREFIX)/bin
	cp $(TARGET) $(PREFIX)/bin

# Test target: run the risc_v executable with the "test" subcommand.
# (Your C program should interpret "test" and run its test suite.)
test: $(TEST_EXES)
	@echo "Running all tests..."
	@for test in $(TEST_EXES); do \
		echo "Testing: $$test"; \
		$$test || exit 1; \
	done

# Rule to build test executables
$(TEST_DIR)/%: $(TEST_DIR)/%.o $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
