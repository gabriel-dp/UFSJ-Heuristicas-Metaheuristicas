# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -O2

# Directories
SRC_DIR = src
BIN_DIR = bin

# Find all .cpp files in the src directory
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)

# Extract file names without extensions
EXEC_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%, $(SRC_FILES))

# Default target
all: $(EXEC_FILES)

# Rule to compile each .cpp file
$(BIN_DIR)/%: $(SRC_DIR)/%.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(DEFINES) -o $@ $<

# Debug mode
debug: DEFINES += -DPRINT
debug: clean all

# Clean up generated files
clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean debug
