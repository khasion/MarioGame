#include "view.hpp"

ALLEGRO al;

Bitmap dpyBuffer=nullptr;
bool dpyChanged=true;
Dim dpyX=0, dpyY=0;

// Display routines
bool Open (Dim rw, Dim rh, BitDepth depth) {
	al_set_new_display_option(ALLEGRO_COLOR_SIZE, depth, ALLEGRO_SUGGEST);
	al.disp = al_create_display(rw, rh);
	al.monitor = (ALLEGRO_MONITOR_INFO*) malloc(sizeof(ALLEGRO_MONITOR_INFO));
	al_get_monitor_info(0, al.monitor);
	return al.disp? true : false;
}

void Close (void) {
	return;
}

Dim GetResWidth (void) {
	return al_get_display_width(al.disp);
}

Dim GetResHeight (void) {
	return al_get_display_height(al.disp);
}

BitDepth GetDepth (void) {
	return (BitDepth) al_get_display_option(al.disp, ALLEGRO_COLOR_SIZE);
}
// Bitmap routines
Bitmap BitmapLoad (const std::string& path) {
	return al_load_bitmap(path.c_str());
}

Bitmap BitmapCreate (Dim w, Dim h) {
	return al_create_bitmap(w, h);
}

Bitmap BitmapCopy (Bitmap bmp) {
	return al_clone_bitmap((ALLEGRO_BITMAP*)bmp);
}

Bitmap BitmapClear (Bitmap bmp, RGB c) {
	Bitmap prev = al_get_target_bitmap();
	al_set_target_bitmap((ALLEGRO_BITMAP*)bmp);
	al_clear_to_color(al_map_rgb(c.r, c.g, c.b));
	al_set_target_bitmap((ALLEGRO_BITMAP*)prev);
	return bmp;
}

void BitmapDestroy (Bitmap bmp) {
	al_destroy_bitmap((ALLEGRO_BITMAP*) bmp);
}

Bitmap BitmapGetScreen (void) {
	return al_get_target_bitmap();
}

Dim BitmapGetWidth (Bitmap bmp) {
	return al_get_bitmap_width((ALLEGRO_BITMAP*)bmp);
}

Dim BitmapGetHeight (Bitmap bmp) {
	return al_get_bitmap_height((ALLEGRO_BITMAP*)bmp);
}

void BitmapBlit (Bitmap src, const Rect& from, Bitmap dest, const Point& to) {
	Bitmap prev = al_get_target_bitmap();
	al_set_target_bitmap((ALLEGRO_BITMAP*)dest);
	al_draw_bitmap_region((ALLEGRO_BITMAP*)src, from.x, from.y, from.w, from.h, to.x, to.y, 0);
	al_set_target_bitmap((ALLEGRO_BITMAP*)prev);
}

TileMap tilemap;

void setTile(TileMap* m, Dim col, Dim row, Index index) {
	(*m)[row][col] = index;
}

void PutTile (Bitmap dest, Dim x, Dim y, Bitmap tiles, Index tile) {
	BitmapBlit(tiles, Rect{TileX(tile), TileY(tile), 16, 16},
	dest, Point {x, y});
}

Index GetTile (const TileMap* m, Dim col, Dim row) {
	return (*m)[row][col];
}

byte MakeIndex (byte row, byte col) {
	return row*12 + col;
}

byte GetCol (byte index) {
	return index % 12;
}

byte GetRow (byte index) {
	return index / 12;
}

Dim TileX (byte index) {
	return GetCol(index) << 4;
}

Dim TileY (byte index) {
	return GetRow(index) << 4;
}

void WriteBinMap (const TileMap* m, FILE* fp) {
	fwrite(m, sizeof(TileMap), 1, fp);
}

bool ReadBinMap (TileMap* m, FILE* fp) {
	return true;
}

void WriteTextMap (const TileMap* m, FILE* fp) {

}

bool ReadTextMap (std::string path) {
	std::ifstream file(path);
	int i = 0;
	memset(tilemap, 61, sizeof(tilemap));
	std::string input;
	while (file >> input) {
		std::string temp;
		int j = 0;
		for (auto it = input.cbegin(); it!=input.cend(); ++it) {
			if (*it == ',') {
				setTile(&tilemap, j++, i, std::stoi(temp));
				temp.clear();
			}
			else {
				temp.push_back(*it);
			}
		}
		i++;
	}
	return true;
}

Rect viewWin = {0, 0, 0, 0};

void TileTerrainDisplay(TileMap* map, Bitmap dest, const Rect& viewWin, const Rect& displayArea) {
	if(dpyChanged) {
		auto startCol= DIV_TILE_WIDTH(viewWin.x);
		auto startRow= DIV_TILE_HEIGHT(viewWin.y);
		auto endCol= DIV_TILE_WIDTH(viewWin.x + viewWin.w-1);
		auto endRow= DIV_TILE_HEIGHT(viewWin.y + viewWin.h-1);
		dpyX = MOD_TILE_WIDTH(viewWin.x);
		dpyY = MOD_TILE_WIDTH(viewWin.y);
		dpyChanged= false;
		for (Dim row = startRow; row <= endRow; ++row) {
			for(Dim col = startCol; col <= endCol; ++col) {
				PutTile(
						dpyBuffer,
						MUL_TILE_WIDTH(col-startCol),
						MUL_TILE_HEIGHT(row-startRow),
						al.map.bitmap,
						GetTile(map, col, row)
				);
			}
		}
	}
	BitmapBlit(dpyBuffer, {dpyX, dpyY, viewWin.w, viewWin.h},
	dest,
	{ displayArea.x, displayArea.y});
}

int GetMapPixelWidth (void) {
	return MAX_WIDTH*16;
}

int GetMapPixelHeight (void) {
	return MAX_HEIGHT*16;
}

static void FilterScrollDistance (int viewStartCoord,	int viewSize, int* d, int maxMapSize) {
	auto val = *d + viewStartCoord;
	if (val < 0) {
		*d = viewStartCoord;
	}
	else if ((val + viewSize) >= maxMapSize) {
		*d = maxMapSize - (viewStartCoord + viewSize);
	}
}

void FilterScroll (Rect& viewWin, int* dx, int* dy) {
	FilterScrollDistance(viewWin.x, viewWin.w, dx, GetMapPixelWidth());
	FilterScrollDistance(viewWin.y, viewWin.h, dy, GetMapPixelHeight());
	viewWin.x += *dx; viewWin.y += *dy;
}