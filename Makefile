# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall

# Define final target command
TARGET = todo

# Source and object files
SOURCE = main.cpp Task.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

main.o: main.cpp Task.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

Task.o: Task.cpp Task.h
	$(CXX) $(CXXFLAGS) -c Task.cpp -o Task.o

clean:
	rm -f $(TARGET) $(OBJECTS)
