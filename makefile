CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
LD_FLAGS := -std=c++14 -lSDL2
CC_FLAGS := -std=c++14 -lSDL2

main: $(OBJ_FILES)
	g++ $^ -o $@  $(LD_FLAGS)

obj/%.o: src/%.cpp
	g++ $< -c -o $@ $(CC_FLAGS)

clean:
	rm obj/*
