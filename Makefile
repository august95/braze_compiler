# Paths
BUILD_DIR = build

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g -Icompiler_lib

# Recursively grab all .cpp files inside compiler_lib
SOURCES := $(shell find compiler_lib -name "*.cpp")

# Preserve directory structure inside build/
OBJECTS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

TARGET = braze

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

# Generic rule that works for ALL cpp files
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	rm -f test_file test_file.c.asm test_file.o

.PHONY: all clean
