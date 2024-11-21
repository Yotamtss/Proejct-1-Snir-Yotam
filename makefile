# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.
# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -Iinclude -std=c++17

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BIN_DIR = bin

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BIN_DIR)/%.o)

# Target executable
TARGET = $(BIN_DIR)/simulation

# Rules
all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR)/*.o $(TARGET)

.PHONY: all clean