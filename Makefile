# Makefile with organized folders

CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2
LDFLAGS = -liphlpapi -lws2_32 -static

# Define directories
OBJ_DIR = obj
TARGET = network_scanner.exe

# Source files
SRCS = main.cpp network_scanner.cpp

# Object files (This says: "Change .cpp to .o, and put them in obj/")
OBJS = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)

# Default rule
all: $(TARGET)

# Link executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files into the obj directory
# This special rule says: "To make obj/file.o, look for file.cpp"
# The "| $(OBJ_DIR)" part ensures the folder exists
$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create the directory if it doesn't exist
$(OBJ_DIR):
	mkdir $(OBJ_DIR)

# Clean now removes the obj folder too
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)
	# If on Windows cmd, use: del /Q $(OBJ_DIR)\*.o $(TARGET)

.PHONY: all clean