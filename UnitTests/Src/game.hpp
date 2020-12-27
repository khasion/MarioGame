#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#define DISP_W 640
#define DISP_H 480
#define SPRITE_MAX 252
#define SPRITE_SHEET "../Media/overworld_tileset_grass.png"
#define TILE_W 16
#define TILE_H 16
#define SHEET_W 12*TILE_W
#define SHEET_H 21*TILE_H
#define TILE_MAP "../Media/map1_Kachelebene 1.csv"

#define KEY_SEEN 1
#define KEY_RELEASED 2

typedef struct {
	ALLEGRO_TIMER* timer;
	ALLEGRO_EVENT_QUEUE* queue;
	ALLEGRO_DISPLAY* disp;
	ALLEGRO_FONT* font;
	struct {
		ALLEGRO_BITMAP* sheet;
		ALLEGRO_BITMAP* sprite_map[SPRITE_MAX];
	} sprites;
	unsigned char key[ALLEGRO_KEY_MAX];
}ALLEGRO;

class Game {
	public:
	void MainLoop (ALLEGRO al);
	void MainLoopIteration (void);
};