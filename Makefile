CXX = g++
CXXFLAGS = -std=c++11 -Wall -Isrc/common -Isrc/geometry -Isrc/math -Isrc/camera
LDFLAGS = -lGLEW -lGL -lglfw -ldl -lm

# Source directories
SRC_DIR = src
COMMON_DIR = $(SRC_DIR)/common
GEOMETRY_DIR = $(SRC_DIR)/geometry
MATH_DIR = $(SRC_DIR)/math
CAMERA_DIR = $(SRC_DIR)/camera

# Source files
COMMON_SRC = $(wildcard $(COMMON_DIR)/*.cpp)
GEOMETRY_SRC = $(wildcard $(GEOMETRY_DIR)/*.cpp)
CAMERA_SRC = $(wildcard $(CAMERA_DIR)/*.cpp)
MATH_SRC = $(wildcard $(MATH_DIR)/*.cpp)
MAIN_SRC = $(SRC_DIR)/main.cpp
SRC = $(COMMON_SRC) $(MATH_SRC) $(GEOMETRY_SRC) $(CAMERA_SRC) $(MAIN_SRC)

# Object files
OBJ = $(SRC:.cpp=.o)

# Executable
EXEC = main

# Target to compile the executable
$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: clean
