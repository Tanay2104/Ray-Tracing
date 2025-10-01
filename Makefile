# =============================================================================
# Makefile for the Ray Tracing Project
# =============================================================================

# Compiler and Flags
# ------------------
# CXX: The C++ compiler to use.
# CXXFLAGS: Flags passed to the compiler.
#   -std=c++17: Use the C++17 language standard.
#   -g:         Include debugging information.
#   -Wall:      Enable all standard warnings.
#   -Iinclude:  Tell the compiler to look for header files in the 'include' directory.
#   -O2:        Enable level 2 optimizations.
#   -MMD -MP:   Generate dependency files (.d) to track header changes.
#   -pthread:   Required on some systems (like Linux) for linking thread libraries.
CXX      := g++
CXXFLAGS := -std=c++17 -g -Wall -Iinclude -O2 -MMD -MP -pthread

# Project Structure
# -----------------
# Define the name of the final executable and the directories used.
TARGET    := ray_tracer
SRC_DIR   := src
BUILD_DIR := build
INCLUDE_DIR := include

# Source and Object Files
# -----------------------
# Automatically find all .cpp files in the source directory.
# Generate a list of corresponding object files (.o) that will be placed in the build directory.
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Dependency Files
# ----------------
# Generate a list of dependency files (.d) that correspond to the object files.
DEPS := $(OBJS:.o=.d)

# =============================================================================
# --- Build Rules ---
# =============================================================================

# Default Rule: 'all'
# --------------------
# This is the rule that runs when you just type 'make'. It depends on the target executable.
.PHONY: all
all: $(TARGET)

# Linking Rule
# ------------
# This rule creates the final executable by linking all the object files together.
$(TARGET): $(OBJS)
	@echo "Linking..."
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
	@echo "Build finished. Executable is '$(TARGET)'"

# Compilation Rule
# ----------------
# This is a pattern rule that tells make how to build a .o file in the 'build' directory
# from a corresponding .cpp file in the 'src' directory.
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@# Create the build directory if it doesn't exist yet.
	@mkdir -p $(@D)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Cleanup Rule
# ------------
# 'make clean' will remove the executable and the entire build directory.
.PHONY: clean
clean:
	@echo "Cleaning up..."
	rm -rf $(BUILD_DIR) $(TARGET)

# Dependency Inclusion
# --------------------
# This line tells make to include all the generated dependency files.
# This makes the build system smart: if you change a header file, it will
# automatically recompile only the .cpp files that include it.
-include $(DEPS)