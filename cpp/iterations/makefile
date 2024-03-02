# Makefile

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall

# Executable name
TARGET = v9.exe

# Source files
SRCS = game9.cpp v9.cpp

# Build and run tests
.PHONY: all
all:
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) -lglfw3 -lkernel32 -lopengl32 -lglu32 -lglew32
	./$(TARGET)

# Clean rule
.PHONY: clean
clean:
	rm -f $(TARGET)
