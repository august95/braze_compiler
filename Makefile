# Paths
SRC_DIR = compiler_lib/source
BUILD_DIR = build
BIN_DIR = bin

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g -Icompiler_lib

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
	compiler_lib/source/scope.cpp \
	compiler_lib/source/stackMonitor.cpp \
	compiler_lib/source/symbolResolver.cpp \
	compiler_lib/source/token.cpp 
 

OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(SOURCES)))

TARGET = braze

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

# Compile sources in compiler_lib/source/
$(BUILD_DIR)/%.o: compiler_lib/source/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile main file
$(BUILD_DIR)/braze_compiler.o: braze_compiler/braze_compiler.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean