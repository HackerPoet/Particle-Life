# Particle-Life Makefile
# Galen Asphaug <galenasphaug@gmail.com>

CC_PARAMS=-Wall -pedantic -std=c++11
SFML_LIBS=-lsfml-system -lsfml-window -lsfml-graphics

particle-life: Main.o Universe.o
	g++ $(CC_PARAMS) Main.cpp Universe.cpp -o $@ $(SFML_LIBS)

Main.o: Main.cpp Universe.h
	g++ $(CC_PARAMS) -c $< -o $@

Universe.o: Universe.h Particles.h HSV.h
	g++ $(CC_PARAMS) -c $< -o $@

clean:
	rm -f particle-life *.o

run: particle-life
	./particle-life
