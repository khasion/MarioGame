#ifndef VIEW_HPP
#define VIEW_HPP
#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#define SHEET "../../ThirdParty/super_mario_sprite_sheet.png"

typedef struct {
	ALLEGRO_TIMER* timer;
	ALLEGRO_EVENT_QUEUE* queue;
	ALLEGRO_FONT* font;
	ALLEGRO_DISPLAY* disp;
	ALLEGRO_MONITOR_INFO* monitor;
	ALLEGRO_EVENT event;
	ALLEGRO_SAMPLE *sample;
	bool done = false;
	unsigned char key[ALLEGRO_KEY_MAX];
}ALLEGRO;
;
extern ALLEGRO al;

/* Display.*/
typedef unsigned int	Color;
typedef unsigned char RGBValue;
typedef unsigned char Alpha;
struct RGB { RGBValue r, g, b;};
struct RGBA : public RGB { RGBValue a;};

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
Bitmap 	BitmapClear		(Bitmap bmp, RGB c);
void		BitmapDestroy	(Bitmap bmp);
Bitmap	BitmapGetScreen(void);
Dim		BitmapGetWidth	(Bitmap bmp);
Dim		BitmapGetHeight(Bitmap bmp);
void	BitmapBlit(
						Bitmap src,  const Rect& from,
						Bitmap dest, const Point& to
				);
void MaskedBlit (
						Bitmap src, const Rect& from,
						Bitmap dest, const Point& to
				);

class BitmapLoader {
private:
	using 	Bitmaps = std::map<std::string, Bitmap>;
	Bitmaps	bitmaps;
	Bitmap	GetBitmap (const std::string& path) const {
		auto i = bitmaps.find(path);
		return i != bitmaps.end() ? i->second : nullptr;
	}
public:
	Bitmap	Load (const std::string& path) {
		auto b = GetBitmap(path);
		if (!b) {
			bitmaps[path] = b = BitmapLoad(path);
			assert(b);
		}
		return b;
	}
	void		CleanUp (void) {
		bitmaps.clear();
	}
	BitmapLoader (void) {}
	~BitmapLoader(void) {CleanUp(); }
};

typedef unsigned short byte;
typedef unsigned short Dim;

byte MakeIndex (byte row, byte col);
byte GetCol (byte index);
byte GetRow (byte index);

Dim TileX (byte index);
Dim TileY (byte index);

typedef unsigned short Index;

#define TILE_WIDTH 16
#define TILE_HEIGHT 16
#define SHEET_WIDTH 26
#define SHEET_HEIGHT 24
#define SHEET_PIXEL_WIDTH SHEET_WIDTH*TILE_W
#define SHEET_PIXEL_HEIGHT SHEET_HEIGHT*TILE_H
#define MAX_WIDTH 233 
#define MAX_HEIGHT 30 

typedef Index TileMap[MAX_HEIGHT][MAX_WIDTH];

void PutTile (Bitmap dest, Dim x, Dim y, Bitmap tiles, Index tile);

#define MUL_TILE_WIDTH(i)((i)<<4)
#define MUL_TILE_HEIGHT(i)((i)<<4)
#define DIV_TILE_WIDTH(i)((i)>>4)
#define DIV_TILE_HEIGHT(i)((i)>>4)
#define MOD_TILE_WIDTH(i)((i)&15)
#define MOD_TILE_HEIGHT(i)((i)&15)

int GetMapPixelWidth (void);
int GetMapPixelHeight (void);

#define GRID_ELEMENT_WIDTH TILE_WIDTH
#define GRID_ELEMENT_HEIGHT TILE_HEIGHT

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
typedef GridIndex GridMap[GRID_MAX_HEIGHT][GRID_MAX_WIDTH];

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

extern bool IsTileIndexAssumedEmpty (Index index);
void ComputeTileGridBlocks1 (const TileMap* map, GridMap* grid);

extern std::vector<bool> empty_tiles;

#endif