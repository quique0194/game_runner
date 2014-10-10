all: game.out

game.out: main.cpp
	g++ main.cpp -o game.out -lGL -lglut

clean:
	rm -rf game.out
