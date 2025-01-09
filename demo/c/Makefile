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
UTILS_DIR      = ../utils
BUILD_DIR = .build

VPATH = $(IM_TOGGLE_DIR): $(CIM_TOGGLE_DIR) : $(CIMGUI_DIR): $(IMGUI_DIR) : $(UTILS_DIR) : $(IMGUI_DIR)/backends

CSOURCES = main.c setupFonts.c utils.c
SOURCES += themeGold.cpp
SOURCES += imgui_impl_glfw.cpp \
           imgui_impl_opengl3.cpp \
					 cimgui.cpp

SOURCES += $(notdir $(wildcard $(IMGUI_DIR)/*.cpp))
SOURCES += $(notdir $(wildcard $(IM_TOGGLE_DIR)/*.cpp))
SOURCES += $(notdir $(wildcard $(CIM_TOGGLE_DIR)/*.cpp))

CXXFLAGS += -MMD -MP
CXXFLAGS += -O1 -Wl,-s
CXXFLAGS += -I$(IM_TOGGLE_DIR) -I$(CIMGUI_DIR) -I$(CIMGUI_DIR)/generator/output
CXXFLAGS += -I$(UTILS_DIR) -I$(CIM_TOGGLE_DIR)
CXXFLAGS += -I$(IM_TOGGLE_DIR) -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends
CXXFLAGS += -DCIMGUI_USE_OPENGL3 -DCIMGUI_USE_GLFW -DIMGUI_IMPL_OPENGL_LOADER_GL3W

OBJS = $(addprefix $(BUILD_DIR)/,$(CSOURCES:.c=.o)) $(addprefix $(BUILD_DIR)/,$(SOURCES:.cpp=.o))
UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lGL


CXXFLAGS += -g -Wall -Wformat
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

	CXXFLAGS += -DIMGUI_IMPL_API="extern \"C\""
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

	CXXFLAGS += -DIMGUI_IMPL_API="extern \"C\" __declspec(dllexport)"
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
