#ifndef BIT_HPP
#define BIT_HPP
#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>

#define SHEET "../Media/overworld_tileset_grass.png"

typedef struct {
	ALLEGRO_TIMER* timer;
	ALLEGRO_EVENT_QUEUE* queue;
	ALLEGRO_FONT* font;
	ALLEGRO_DISPLAY* disp;
	ALLEGRO_MONITOR_INFO* monitor;
	ALLEGRO_EVENT event;
	bool done = false;
	bool redraw = true;
	struct {
		ALLEGRO_BITMAP* bitmap;
		ALLEGRO_BITMAP** tilemap;
	}map;
	unsigned char key[ALLEGRO_KEY_MAX];
}ALLEGRO;

extern ALLEGRO al;

typedef unsigned int	Color;
typedef unsigned char RGBValue;
typedef unsigned char Alpha;
struct RGB { RGBValue r, g, b; };
struct RGBA : public RGB { RGBValue a; };
/*
typedef RGB Palette[256];
void SetPalette (RGB* palette);

Color Make8  (RGBValue r, RGBValue g, RGBValue b);
Color Make16 (RGBValue r, RGBValue g, RGBValue b);
Color Make24 (RGBValue r, RGBValue g, RGBValue b);
Color Make32 (RGBValue r, RGBValue g, RGBValue b, Alpha alpha=0);*/

typedef unsigned short Dim;
struct Rect		{int x, y, w, h; };
struct Point	{int x, y; };
enum BitDepth	{bits8 = 8, bits16 = 16, bits24 = 24, bits32 = 32};

bool		Open (Dim rw, Dim rh, BitDepth depth);
void 		Close (void);
Dim 		GetResWidth (void);
Dim 		GetResHeight (void);
BitDepth GetDepth (void);

typedef void* Bitmap;
Bitmap	BitmapLoad 		(const std::string& path);
Bitmap	BitmapCreate	(Dim w, Dim h);
Bitmap	BitmapCopy		(Bitmap bmp);
Bitmap 	BitmapClear		(Bitmap bmp, Color c);
void		BitmapDestroy	(Bitmap bmp);
Bitmap	BitmapGetScreen(void);
Dim		BitmapGetWidth	(Bitmap bmp);
Dim		BitmapGetHeight(Bitmap bmp);
void	BitmapBlit(
						Bitmap src,  const Rect& from,
						Bitmap dest, const Point& to
				);
#endif