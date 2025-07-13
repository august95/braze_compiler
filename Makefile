# Paths
SRC_DIR = compiler_lib/source
BUILD_DIR = build
BIN_DIR = bin

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g -Icompiler_lib

# Sources with full paths
SOURCES = \
    compiler_lib/source/asmWriter.cpp \
    compiler_lib/source/braze_compiler.cpp \
    compiler_lib/source/codeGenerator.cpp \
    compiler_lib/source/compilerProcess.cpp \
    compiler_lib/source/datatype.cpp \
    compiler_lib/source/filePosition.cpp \
    compiler_lib/source/lexer.cpp \
    compiler_lib/source/node.cpp \
    compiler_lib/source/parser.cpp \
    compiler_lib/source/precedenceHandler.cpp \
    compiler_lib/source/resolver.cpp \
    compiler_lib/source/resolverEntity.cpp \
    compiler_lib/source/resolverEntityData.cpp \
    compiler_lib/source/resolverResult.cpp \
    compiler_lib/source/resolverScope.cpp \
    compiler_lib/source/resolverScopeData.cpp \
    compiler_lib/source/scope.cpp \
    compiler_lib/source/stackMonitor.cpp \
    compiler_lib/source/symbolResolver.cpp \
    compiler_lib/source/token.cpp 
   # braze_compiler/braze_compiler.cpp

# Object files with build/ prefix (remove directory from source and just prepend build/)
OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(SOURCES)))

TARGET = $(BIN_DIR)/braze_compiler

all: $(TARGET)

# Link without adding build/ prefix again (just use OBJECTS as-is)
$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $@

# Compile sources in compiler_lib/source/
$(BUILD_DIR)/%.o: compiler_lib/source/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile braze_compiler/braze_compiler.cpp separately
$(BUILD_DIR)/braze_compiler.o: braze_compiler/braze_compiler.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean
