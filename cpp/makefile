# Makefile

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall

# Executable name
TARGET = run.exe

# Source files
SRCS = player.cpp game.cpp main.cpp

# Build and run tests
.PHONY: all
all:
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) -lglfw3 -lkernel32 -lopengl32 -lglu32 -lglew32
	./$(TARGET)

# Clean rule
.PHONY: clean
clean:
	rm -f $(TARGET)
