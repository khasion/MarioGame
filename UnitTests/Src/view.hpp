#ifndef VIEW_HPP
#define VIEW_HPP
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
	struct {
		ALLEGRO_BITMAP* bitmap;
	}map;
	unsigned char key[ALLEGRO_KEY_MAX];
}ALLEGRO;

extern ALLEGRO al;

typedef unsigned int	Color;
typedef unsigned char RGBValue;
typedef unsigned char Alpha;
struct RGB { RGBValue r, g, b; };
struct RGBA : public RGB { RGBValue a; };

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


extern Bitmap dpyBuffer;
extern bool dpyChanged;
extern Dim dpyX, dpyY;

typedef unsigned char byte;
typedef unsigned short Dim;

byte MakeIndex (byte row, byte col);
byte GetCol (byte index);
byte GetRow (byte index);

Dim TileX (byte index);
Dim TileY (byte index);

typedef unsigned short Index;

#define MAX_WIDTH 50
#define MAX_HEIGHT 50

typedef Index TileMap[MAX_WIDTH][MAX_HEIGHT];
extern TileMap tilemap;

void PutTile (Bitmap dest, Dim x, Dim y, Bitmap tiles, Index tile);

void SetTile(TileMap* m, Dim col, Dim row, Index index);

Index GetTile (const TileMap* m, Dim col, Dim row);

void WriteBinMap (const TileMap* m, FILE* fp);
bool ReadBinMap (TileMap* m, FILE* fp);
void WriteTextMap (const TileMap*, FILE* fp);
bool ReadTextMap (std::string path);

TileMap* getTileMap (void);

#define MUL_TILE_WIDTH(i)((i)<<4)
#define MUL_TILE_HEIGHT(i)((i)<<4)
#define DIV_TILE_WIDTH(i)((i)>>4)
#define DIV_TILE_HEIGHT(i)((i)>>4)
#define MOD_TILE_WIDTH(i)((i)&15)
#define MOD_TILE_HEIGHT(i)((i)&15)

extern Rect viewWin;

void TileTerrainDisplay(TileMap* map, Bitmap dest, const Rect& viewWin, const Rect& displayArea);

int GetMapPixelWidth (void);
int GetMapPixelHeight (void);

static void FilterScrollDistance (
	int ViewStartCoord,
	int viewSize,
	int* d,
	int maxMapSize
);

void FilterScroll (Rect& viewWin, int* dx, int* dy);

#endif