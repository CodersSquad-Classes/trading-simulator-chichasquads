# Compiler configuration
CXX = clang++
CXXFLAGS = -std=c++20 -Wall

# Paths and target
SRC_DIR = Trading
TARGET = trading_simulator.exe

# Source files
SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/clob.cpp

# Default rule to build the program
all: $(TARGET)

# Build the executable from the source files
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

# Run the program after building
run: $(TARGET)
	./$(TARGET)

# Clean generated binary
clean:
	rm -f $(TARGET)