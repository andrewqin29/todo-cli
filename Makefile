# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall -g

# Name for final target executable
TARGET = todo

# Source, object and header files
SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.h)
OBJECTS = $(SOURCES:.cpp=.o)

# Default final executable
all: $(TARGET)

# Linker
$(TARGET): $(OBJECTS)
	@echo "Linking..."
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "Build complete. Executable is '$(TARGET)'"

# Compile .cpp files into .o
%.o: %.cpp $(HEADERS)
	@echo "Building .o files..."
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo ".o files built."

# Make clean to clean directory
.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJECTS)
