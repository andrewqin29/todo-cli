# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -g

TARGET = todo

# Source and object files
SOURCES = Main.cpp Task.cpp
HEADERS = Task.h
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

Main.o: Main.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

Task.o: Task.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c Task.cpp -o Task.o

clean:
	rm -f $(TARGET) $(OBJECTS)
