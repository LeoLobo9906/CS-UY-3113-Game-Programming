# Source and target
SRCS   = main.cpp CS3113/cs3113.cpp CS3113/Entity.cpp
TARGET = lunar_lander

# OS detection
UNAME_S := $(shell uname -s)

CXX      = g++
CXXFLAGS = -std=c++11

RAYLIB_CFLAGS = $(shell pkg-config --cflags raylib)
RAYLIB_LIBS   = $(shell pkg-config --libs   raylib)

# ---------- macOS ----------
ifeq ($(UNAME_S),Darwin)
    CXXFLAGS += -arch arm64 $(RAYLIB_CFLAGS)
    LIBS = $(RAYLIB_LIBS) \
           -framework OpenGL \
           -framework Cocoa \
           -framework IOKit \
           -framework CoreVideo
    EXEC = ./$(TARGET)

# ---------- Windows (MinGW) ----------
else ifneq (,$(findstring MINGW,$(UNAME_S)))
    CXXFLAGS += -IC:/raylib/include
    LIBS = -LC:/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm
    TARGET := $(TARGET).exe
    EXEC = ./$(TARGET)

# ---------- Linux ----------
else
    CXXFLAGS += $(RAYLIB_CFLAGS)
    LIBS = $(RAYLIB_LIBS) -lGL -lm -lpthread -ldl -lrt -lX11
    EXEC = ./$(TARGET)
endif

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS) $(LIBS)

.PHONY: clean run

clean:
	@rm -f $(TARGET) $(TARGET).exe

run: $(TARGET)
	$(EXEC)