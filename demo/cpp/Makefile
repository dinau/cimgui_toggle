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

VPATH = $(IM_TOGGLE_DIR) : $(IMGUI_DIR) : $(IMGUI_DIR)/backends

SOURCES = main.cpp
SOURCES += $(wildcard $(IMGUI_DIR)/*.cpp)
SOURCES += backends/imgui_impl_glfw.cpp \
           backends/imgui_impl_opengl3.cpp
SOURCES += $(wildcard $(IM_TOGGLE_DIR)/*.cpp)

OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lGL

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

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

%.o:%.c
	@echo  [$(CC)]: $(notdir $<)
	@$(CC) $(CFLAGS) -c -o $@ $<

%.o:%.cpp
	@echo  [$(CXX)]: $(notdir $<)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	@$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	@rm -f $(EXE) $(OBJS)

run: all
	./$(EXE)
