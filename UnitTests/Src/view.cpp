#include "view.hpp"

ALLEGRO al;

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

void PutTile (Bitmap dest, Dim x, Dim y, Bitmap tiles, Index tile) {
	BitmapBlit(tiles, Rect{TileX(tile), TileY(tile), 16, 16},
	dest, Point {x, y});
}

byte MakeIndex (byte row, byte col) {
	return row*SHEET_WIDTH + col;
}

byte GetCol (byte index) {
	return index % SHEET_WIDTH;
}

byte GetRow (byte index) {
	return index / SHEET_WIDTH;
}

Dim TileX (byte index) {
	return GetCol(index) << 4;
}

Dim TileY (byte index) {
	return GetRow(index) << 4;
}

void SetGridTile (GridMap* m, Dim col, Dim row, GridIndex index) {
	(*m)[row][col] = index;
}

GridIndex GetGridTile (const GridMap* m, Dim col, Dim row) {
	return (*m)[row][col];
}

void SetSolidGridTile (GridMap* m, Dim col, Dim row) {
	SetGridTile(m, col, row, GRID_SOLID_TILE);
}

void SetEmptyGridTile (GridMap* m, Dim col, Dim row) {
	SetGridTile(m, col, row, GRID_EMPTY_TILE);
}

void SetGridTileFlags (GridMap* m, Dim col, Dim row, GridIndex flags) {
	SetGridTile(m, col, row, flags);
}

void SetGridTileTopSolidOnly (GridMap* m, Dim col, Dim row) {
	SetGridTile(m, col, row, GRID_TOP_SOLID_MASK);
}

bool CanPassGridTile (GridMap* m, Dim col, Dim row, GridIndex flags) {
	return GetGridTile(m, col, row) & (flags != 0);
}

bool IsTileIndexAssumedEmpty (Index index) {
	if (index < 48 || index == 61 || index == 142 || (index >= 54 && index <= 59) ||
	index == 69 || index == 71 || (index >= 81 && index <= 83) ||
	index == 84 || index == 85 || index == 96 || index == 97)  {
		return true;
	}
	return false;
}

void ComputeTileGridBlocks1 (const TileMap* map, GridMap* gridmap) {
	for (auto row = 0; row < MAX_HEIGHT; ++row) {
		for (auto col = 0; col < MAX_WIDTH; ++col) {
			(*gridmap)[row][col] = IsTileIndexAssumedEmpty((*map)[row][col]) ? GRID_EMPTY_TILE : GRID_SOLID_TILE;
		}
	}
}