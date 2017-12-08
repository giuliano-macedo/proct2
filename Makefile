CXX      := -c++
CXXFLAGS := -Wall -Wno-switch
LDFLAGS  := -L/usr/lib -lstdc++ -lm
BUILD    := build
INCLUDE  := -Iinclude/
SRC      :=                      \
	$(wildcard src/shape/*.cpp) \
	$(wildcard src/texture/*.cpp) \
	$(wildcard src/threshold/*.cpp) \
	$(wildcard src/*.c*)         \

# OBJECTS := $(SRC:%.c%=build/%.o)
OBJECTS := $(SRC:%=build/%.o)

all: build proct2

# build/%.o: %.c%
build/%.o: %
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

proct2: $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) -o proct2 $(OBJECTS)

.PHONY: all build clean


clean:
	rm -rf build/*
	rm proct2