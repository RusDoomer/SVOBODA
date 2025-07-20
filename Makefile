# Compiler
CC := gcc

# Directories
SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build

# File patterns
SOURCES := $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

# Executable name and location (in the base directory)
EXECUTABLE := svoboda

# Compiler flags
CFLAGS := -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/stats -Wall
LDFLAGS := -lmicrohttpd -ljson-c -lpthread -flto=auto
OPT_FLAGS := -O3 -march=native -flto=auto -ffast-math
DEBUG_FLAGS := -g -fsanitize=address

# Detect the operating system
UNAME_S := $(shell uname -s)

# Default target to build the optimized version
.PHONY: all
all: directories $(EXECUTABLE)

# Create all necessary directories
DIRECTORIES := $(sort $(dir $(OBJECTS))) $(BUILD_DIR)

.PHONY: directories
directories: $(DIRECTORIES)

$(DIRECTORIES):
	@mkdir -p $@

# Link object files into the executable (placed directly in base directory)
$(EXECUTABLE): $(OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $(EXECUTABLE) $(OPT_FLAGS)

# Pattern rule for compiling source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(OPT_FLAGS) -c $< -o $@

# Target for debugging version with AddressSanitizer
.PHONY: debug
debug:
	$(MAKE) all CFLAGS="$(CFLAGS) $(DEBUG_FLAGS)" LDFLAGS="$(LDFLAGS) $(DEBUG_FLAGS)"

# Clean up build files and directories
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE)
