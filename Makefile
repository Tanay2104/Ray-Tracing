# =============================================================================
# Makefile for the Ray Tracing Project
# =============================================================================

# Compiler and Flags
# ------------------
# CXX: The C++ compiler to use. 
# CXXFLAGS: Flags passed to the compiler for every file.
#   -std=c++23: Use the C++23 language standard.
#   -g:         Include debugging information in the executable.
#   -Wall:      Enable all standard warnings. Highly recommended!
#   -O2:        Enable level 2 optimizations (good for release builds).
#               You can remove this or change to -O0 for faster debug builds.
#   -MMD -MP:   These are for automatic dependency generation (see below).
CXX = g++
CXXFLAGS = -std=c++23 -g -Wall -O2 -MMD -MP

# Executable Name
# ---------------
# The name of the final program to be created.
TARGET = ray_tracer

# Source and Object Files
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)

# Default Rule: 'all'
.PHONY: all
all: $(TARGET)

# Linking Rule
$(TARGET): $(OBJS)
	@echo "Linking..."
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
	@echo "Build finished. Executable is '$(TARGET)'"

# Compilation Rule
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Cleanup Rule
.PHONY: clean
clean:
	@echo "Cleaning up..."
	rm -f $(OBJS) $(DEPS)

# Dependency Inclusion
-include $(DEPS)