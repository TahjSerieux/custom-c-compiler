# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# Target executable
TARGET = mycc

# Source files
SOURCES = mycc.cpp Token.cpp Lexer.cpp Parser.cpp AST.cpp Tacky.cpp Assembly2.cpp
HEADERS = Token.hpp Lexer.hpp Parser.hpp AST.hpp Tacky.hpp Assembly2.hpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compile source files to object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)

# Rebuild everything
rebuild: clean all

# Run the compiler with a test file
test: $(TARGET)
	@echo "To test the compiler, run: ./$(TARGET) <source_file.c>"

.PHONY: all clean rebuild test