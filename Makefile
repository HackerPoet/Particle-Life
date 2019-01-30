All:
	g++ -c Main.cpp Universe.cpp
	g++ Main.o Universe.o -o Particle_life -lsfml-graphics -lsfml-window -lsfml-system
