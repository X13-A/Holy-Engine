CXX = g++
CXXFLAGS = -g -O0 -std=c++17 -Wall -Isrc/common -Isrc/geometry -Isrc/math -Isrc/camera -Isrc/gui/imgui -Isrc/gui/imgui/backend
LDFLAGS = -lGLEW -lGL -lglfw -ldl -lm

# Source directories
SRC_DIR = src
CAMERA_DIR = $(SRC_DIR)/camera
COMMON_DIR = $(SRC_DIR)/common
CONTROLS_DIR = $(SRC_DIR)/controls
GEOMETRY_DIR = $(SRC_DIR)/geometry
WIREFRAME_DIR = $(SRC_DIR)/wireframe
GUI_DIR = $(SRC_DIR)/gui
INPUT_DIR = $(SRC_DIR)/input
LIGHT_DIR = $(SRC_DIR)/light
MATERIALS_DIR = $(SRC_DIR)/materials
MATH_DIR = $(SRC_DIR)/math
MODEL_DIR = $(SRC_DIR)/model
UTILS_DIR = $(SRC_DIR)/utils
WINDOW_DIR = $(SRC_DIR)/window
POSTPROCESS_DIR = $(SRC_DIR)/postprocess

# Source files
CAMERA_SRC = $(wildcard $(CAMERA_DIR)/*.cpp)
COMMON_SRC = $(wildcard $(COMMON_DIR)/*.cpp)
CONTROLS_SRC = $(wildcard $(CONTROLS_DIR)/*.cpp)
GEOMETRY_SRC = $(wildcard $(GEOMETRY_DIR)/*.cpp)
WIREFRAME_SRC = $(wildcard $(WIREFRAME_DIR)/*.cpp)
GUI_SRC = $(wildcard $(GUI_DIR)/*.cpp)
IMGUI_SRC = $(wildcard $(GUI_DIR)/imgui/*.cpp)
IMGUI_BACKEND_SRC = $(wildcard $(GUI_DIR)/imgui/backend/*.cpp)
INPUT_SRC = $(wildcard $(INPUT_DIR)/*.cpp)
LIGHT_SRC = $(wildcard $(LIGHT_DIR)/*.cpp)
MATERIALS_SRC = $(wildcard $(MATERIALS_DIR)/*.cpp)
MATH_SRC = $(wildcard $(MATH_DIR)/*.cpp)
MODEL_SRC = $(wildcard $(MODEL_DIR)/*.cpp)
UTILS_SRC = $(wildcard $(UTILS_DIR)/*.cpp)
WINDOW_SRC = $(wildcard $(WINDOW_DIR)/*.cpp)
POSTPROCESS_SRC = $(wildcard $(POSTPROCESS_DIR)/*.cpp)
MAIN_SRC = $(SRC_DIR)/main.cpp

SRC = $(CAMERA_SRC) $(COMMON_SRC) $(CONTROLS_SRC) $(GEOMETRY_SRC) $(WIREFRAME_SRC) $(INPUT_SRC) $(LIGHT_SRC) $(MATERIALS_SRC) $(MATH_SRC) $(MODEL_SRC) $(UTILS_SRC) $(WINDOW_SRC) $(POSTPROCESS_SRC)  $(GUI_SRC) $(IMGUI_SRC) $(IMGUI_BACKEND_SRC) $(MAIN_SRC)

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
