CXX      := -c++
CXXFLAGS := -Wall
LDFLAGS  := -L/usr/lib -lstdc++ -lm
BUILD    := build
INCLUDE  := -Iinclude/
SRC      :=                      \
	$(wildcard src/shape/*.cpp) \
	$(wildcard src/texture/*.cpp) \
	$(wildcard src/threshold/*.cpp) \
	$(wildcard src/*.c*)         \

OBJECTS := $(SRC:%.cpp=build/%.o)

all: build proct2

build/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

proct2: $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) -o proct2 $(OBJECTS)

.PHONY: all build clean


clean:
	rm -rf build/*
