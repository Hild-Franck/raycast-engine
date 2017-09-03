myBuild: hello-world.cpp
	g++ -Wall -o $@ $^ -lSDL -lSDL_image -lSDL_ttf

mrproper:
	rm -f myBuild