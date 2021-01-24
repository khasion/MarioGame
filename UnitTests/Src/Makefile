CC = g++
CXXFLAGS = -std=c++14

make:	main.o app.o view.o layer.o game.o anim.o entities.o
	g++ main.o app.o view.o layer.o game.o anim.o entities.o -o game -std=c++14 -lallegro_font -lallegro_image -lallegro_primitives -lallegro_color -lallegro -std=c++14 -std=gnu++14

main.o: main.cpp app.hpp game.hpp anim.hpp layer.hpp view.hpp entities.hpp
	$(CC) -c main.cpp -std=c++14
app.o:	app.cpp app.hpp game.hpp anim.hpp layer.hpp view.hpp entities.hpp
	$(CC) -c app.cpp -std=c++14
view.o:	view.cpp view.hpp
	$(CC) -c view.cpp -std=c++14
layer.o: layer.cpp layer.hpp view.hpp
	$(CC) -c layer.cpp -std=c++14
game.o:	game.cpp game.hpp anim.hpp layer.hpp view.hpp
	$(CC) -c game.cpp -std=c++14
anim.o: anim.cpp anim.hpp layer.hpp view.hpp
	$(CC) -c anim.cpp -std=c++14
entities.o: entities.cpp entities.hpp
	$(CC) -c entities.cpp -std=c++14
clean:
	rm game;
	rm main.o;
	rm app.o;
	rm view.o;
	rm layer.o;
	rm game.o;
	rm anim.o;
	rm vgcore.*;
