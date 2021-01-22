CC = g++

make:	main.o app.o view.o layer.o game.o anim.o entities.o
	g++ main.o app.o view.o layer.o game.o anim.o entities.o -o game -lallegro_font -lallegro_image -lallegro_primitives -lallegro_color -lallegro

main.o: main.cpp app.hpp game.hpp anim.hpp layer.hpp view.hpp
	$(CC) -c main.cpp 
app.o:	app.cpp app.hpp game.hpp anim.hpp layer.hpp view.hpp
	$(CC) -c app.cpp
view.o:	view.cpp view.hpp
	$(CC) -c view.cpp
layer.o: layer.cpp layer.hpp view.hpp
	$(CC) -c layer.cpp
game.o:	game.cpp game.hpp anim.hpp layer.hpp view.hpp
	$(CC) -c game.cpp
anim.o: anim.cpp anim.hpp layer.hpp view.hpp
	$(CC) -c anim.cpp
entities.o: entities.cpp entities.hpp
	$(CC) -c entities.cpp
clean:
	rm game;
	rm main.o;
	rm app.o;
	rm view.o;
	rm layer.o;
	rm game.o;
	rm anim.o;
	rm vgcore.*;
