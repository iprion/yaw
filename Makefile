
CXX = g++
#CXX = clang++

EXE = yaw
IMGUI_DIR = ../imgui
SOURCES = main.cpp
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_vulkan.cpp
SOURCES += $(wildcard src/*.cpp)
SOURCES += $(wildcard src/trackball/*.cpp)
SOURCES += $(wildcard src/application/*.cpp)
SOURCES += $(wildcard src/vulkan/*.cpp)
SOURCES += $(wildcard src/utils/*.cpp)
OBJDIR = obj
OBJS = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))
UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lvulkan
CXXFLAGS = -std=c++2b -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -I../glfw/include -I/Users/ogalizzi/VulkanSDK/1.3.268.1/macOS/include -I./src
CXXFLAGS += -g -Wall -Wformat -Wno-deprecated 
LIBS = -L../glfw/lib-x86_64 -L/Users/ogalizzi/VulkanSDK/1.3.268.1/macOS/lib 


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
	LIBS += -L/usr/local/lib -L/opt/local/lib 
	LIBS += -lglfw3 -lglew -lvulkan
	#LIBS += -lglfw

	CXXFLAGS += -I/usr/local/include -I/opt/local/include

	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(OS), Windows_NT)
	ECHO_MESSAGE = "MinGW"
	LIBS += -lglfw3 -lgdi32 -lopengl32 -limm32 -lvulkan

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

$(OBJDIR)/%.o:src/application/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/%.o:src/utils/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/%.o:src/vulkan/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/%.o:src/trackball/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/%.o:src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/%.o:$(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS)

print-%  : ; @echo $* = $($*) # make print-OBJS to print content of OBJS variable
