#
# Cross Platform Makefile
# Compatible with MSYS2/MINGW, Ubuntu 14.04.1 and Mac OS X
#
# You will need GLFW (http://www.glfw.org):
# Linux:
#   apt-get install libglfw-dev
# Mac OS X:
#   brew install glfw
# MSYS2:
#   pacman -S --noconfirm --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-glfw
#

#CXX = g++
#CXX = clang++

EXE = example_glfw_opengl3

CIM_TOGGLE_DIR = ../..
LIBS_DIR       = $(CIM_TOGGLE_DIR)/libs
CIMGUI_DIR     = $(LIBS_DIR)/cimgui
IMGUI_DIR      = $(CIMGUI_DIR)/imgui
IM_TOGGLE_DIR  = $(LIBS_DIR)/imgui_toggle
BUILD_DIR      = .build

VPATH = $(IM_TOGGLE_DIR) : $(IMGUI_DIR) : $(IMGUI_DIR)/backends

SOURCES = main.cpp
SOURCES += $(wildcard $(IMGUI_DIR)/*.cpp)
SOURCES += backends/imgui_impl_glfw.cpp \
           backends/imgui_impl_opengl3.cpp
SOURCES += $(wildcard $(IM_TOGGLE_DIR)/*.cpp)

OBJS = $(addprefix $(BUILD_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))
UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lGL

CXXFLAGS += -MMD -MP
CXXFLAGS +=  -O1 -Wl,-s
CXXFLAGS += -std=c++11 -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends
CXXFLAGS += -g -Wall -Wformat
CXXFLAGS += -I$(IM_TOGGLE_DIR)
LIBS =

##---------------------------------------------------------------------
## OPENGL ES
##---------------------------------------------------------------------

## This assumes a GL ES library available in the system, e.g. libGLESv2.so
# CXXFLAGS += -DIMGUI_IMPL_OPENGL_ES2
# LINUX_GL_LIBS = -lGLESv2

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += $(LINUX_GL_LIBS) `pkg-config --static --libs glfw3`

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	LIBS += -L/usr/local/lib -L/opt/local/lib -L/opt/homebrew/lib
	#LIBS += -lglfw3
	LIBS += -lglfw

	CXXFLAGS += -I/usr/local/include -I/opt/local/include -I/opt/homebrew/include
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(OS), Windows_NT)
	ECHO_MESSAGE = "MinGW"
	LIBS += -lglfw3 -lgdi32 -lopengl32 -limm32

	CXXFLAGS += $(shell pkg-config --cflags glfw3)
	CFLAGS = $(CXXFLAGS)
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

all: $(BUILD_DIR) $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(BUILD_DIR):
	@-mkdir -p $@

$(BUILD_DIR)/%.o:%.c Makefile
	@echo [$(CC) ] $(notdir $<)
	@$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o:%.cpp Makefile
	@echo [$(CXX)] $(notdir $<)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

$(EXE): $(OBJS)
	@$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	@rm -rf $(BUILD_DIR)
	@rm -f $(EXE)

run: all
	./$(EXE)

include $(wildcard $(BUILD_DIR)/*.d)
