CC = g++
CPPFLAGS = -std=c++14
SDLFLAGS = -lSDL2
OBJS = obj/SkiJump.o obj/Terrain.o obj/Game.o obj/Jumper.o obj/Physics.o obj/Numbers.o obj/Randomness.o obj/Judges.o

all: $(OBJS)
	$(CC) -o skijump $(OBJS) $(CPPFLAGS) $(SDLFLAGS)

obj/SkiJump.o: src/SkiJump.cpp src/SkiJump.hpp
	$(CC) src/SkiJump.cpp -c -o obj/SkiJump.o $(CPPFLAGS) $(SDLFLAGS)

obj/Terrain.o: src/Terrain.hpp src/Terrain.cpp src/SplineTerrain.hpp src/PatternTerrain.hpp src/ObjectTerrain.hpp
	$(CC) src/Terrain.cpp -c -o obj/Terrain.o $(CPPFLAGS) $(SDLFLAGS)

obj/Game.o: src/Game.hpp src/Game.cpp
	$(CC) src/Game.cpp -c -o obj/Game.o $(CPPFLAGS) $(SDLFLAGS)

obj/Jumper.o: src/Jumper.hpp src/Jumper.cpp
	$(CC) src/Jumper.cpp -c -o obj/Jumper.o $(CPPFLAGS) $(SDLFLAGS)

obj/Physics.o: src/Physics.hpp src/Physics.cpp
	$(CC) src/Physics.cpp -c -o obj/Physics.o $(CPPFLAGS) $(SDLFLAGS)

obj/Numbers.o: src/Numbers.hpp src/Numbers.cpp
	$(CC) src/Numbers.cpp -c -o obj/Numbers.o $(CPPFLAGS) $(SDLFLAGS)

obj/Randomness.o: src/Randomness.hpp src/Randomness.cpp
	$(CC) src/Randomness.cpp -c -o obj/Randomness.o $(CPPFLAGS)

obj/Judges.o: src/Judges.hpp src/Judges.cpp
	$(CC) src/Judges.cpp -c -o obj/Judges.o $(CPPFLAGS) 

clean:
	rm ./obj/*.o

