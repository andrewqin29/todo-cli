# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -g

# Name for final target executable
TARGET = todo

# Source, object and header files
SOURCES = Main.cpp Task.cpp
HEADERS = Task.h
OBJECTS = $(SOURCES:.cpp=.o)

# Default final executable
all: $(TARGET)

# Linker
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compile .cpp files into .o
Main.o: Main.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

Task.o: Task.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c Task.cpp -o Task.o

# Make clean to clean directory
clean:
	rm -f $(TARGET) $(OBJECTS)
