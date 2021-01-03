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
/* Display.*/
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
/*Display.*/
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

#define TILE_WIDTH 16
#define TILE_HEIGHT 16
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
/*Motion control.*/
static void FilterScrollDistance (
	int ViewStartCoord,
	int viewSize,
	int* d,
	int maxMapSize
);

void FilterScroll (Rect& viewWin, int* dx, int* dy);

#define GRID_ELEMENT_WIDTH 16
#define GRID_ELEMENT_HEIGHT 16

#if TILE_WIDTH % GRID_ELEMENT_WIDTH != 0
#error "TILE_WIDTH % GRID_ELEMENT_WIDTH must be zero!"
#endif

#if TILE_HEIGHT % GRID_ELEMENT_HEIGHT != 0
#error "TILE_HEIGHT % GRID_ELEMENT_HEIGHT must be zero"
#endif

#define GRID_BLOCK_COLUMNS	 		(TILE_WIDTH / GRID_ELEMENT_WIDTH)
#define GRID_BLOCK_ROWS 			(TILE_HEIGHT / GRID_ELEMENT_HEIGHT) 
#define GRID_ELEMENTS_PER_TILE 	(GRID_BLOCK_ROWS * GRID_BLOCK_COLUMNS)
#define GRID_MAX_HEIGHT 			(MAX_HEIGHT * GRID_BLOCK_ROWS)
#define GRID_MAX_WIDTH 				(MAX_WIDTH * GRID_BLOCK_COLUMNS)

using GridIndex = byte;
typedef GridIndex GridMap[GRID_MAX_WIDTH][GRID_MAX_HEIGHT];
extern GridMap grid;

void SetGridTile (GridMap* m, Dim col, Dim row, GridIndex index);
GridIndex GetGridTile (const GridMap* m, Dim col, Dim row);

#define GRID_THIN_AIR_MASK			0x0000	// element is ignored
#define GRID_LEFT_SOLID_MASK		0x0001	// bit 0
#define GRID_RIGHT_SOLID_MASK 	0x0002	// bit 1
#define GRID_TOP_SOLID_MASK		0x0004	// bit 2
#define GRID_BOTTOM_SOLID_MASK	0x0008	// bit 3
#define GRID_GROUND_MASK			0x0010	// bit 4, keep objects top / bottom (gravity)
#define GRID_FLOATING_MASK			0x0020	// bit 5, keep objects anywhere inside (gravity)

#define GRID_EMPTY_TILE	GRID_THIN_AIR_MASK
#define GRID_SOLID_TILE \
(GRID_LEFT_SOLID_MASK | GRID_RIGHT_SOLID_MASK | GRID_TOP_SOLID_MASK | GRID_BOTTOM_SOLID_MASK)

void SetSolidGridTile (GridMap* m, Dim col, Dim row);
void SetEmptyGridTile (GridMap* m, Dim col, Dim row);
void SetGridTileFlags (GridMap* m, Dim col, Dim row, GridIndex flags);
void SetGridTileTopSolidOnly (GridMap* m, Dim col, Dim row);
bool CanPassGridTile (GridMap* m, Dim col, Dim row, GridIndex flags);

#define MAX_PIXEL_WIDTH 				MUL_TILE_WIDTH(MAX_WIDTH)	
#define MAX_PIXEL_HEIGHT 				MUL_TILE_HEIGHT(MAX_HEIGHT)
#define DIV_GRID_ELEMENT_WIDTH(i)	((i)>>4)
#define DIV_GRID_ELEMENT_HEIGHT(i)	((i)>>4)
#define MUL_GRID_ELEMENT_WIDTH(i)	((i)<<4)
#define MUL_GRID_ELEMENT_HEIGHT(i)	((i)<<4)

void FilterGridMotion (GridMap* m, Rect& r, int* dx, int* dy);

void FilterGridMotionLeft(GridMap* m, const Rect& r, int* dx);
void FilterGridMotionRight(GridMap* m, const Rect& r, int* dx);
void FilterGridMotionUp(GridMap* m, const Rect& r, int* dy);
void FilterGridMotionDown(GridMap* m, const Rect& r, int* dy);

extern bool IsTileIndexAssumedEmpty (Index index);
void ComputeTileGridBlocks1 (const TileMap* map, GridMap* grid);

#endif