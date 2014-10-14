all: game.out

game.out: main.cpp TextureManager.cpp
	g++ main.cpp TextureManager.cpp -o game.out -lGL -lglut -lfreeimage -lsfml-audio -lsfml-window -lsfml-system -std=c++11


clean:
	rm -rf game.out
