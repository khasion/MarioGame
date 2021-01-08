#ifndef LAYER_HPP
#define LAYER_HPP
#include "view.hpp"

class GridLayer {
private:
	GridMap		grid;
	unsigned		total = 0;
	Dim			totalRows = 0, totalColumns = 0;
	void Allocate (void) {
		for (auto rows = 0; rows < totalRows; rows++) {
			for (auto cols = 0; cols < totalColumns; cols++) {
				grid[rows][cols] = GRID_EMPTY_TILE;
			}
		}
	}
	void FilterGridMotionDown 	(const Rect& r, int* dy);
	void FilterGridMotionUp 	(const Rect& r, int* dy);
	void FilterGridMotionLeft 	(const Rect& r, int* dx);
	void FilterGridMotionRight (const Rect& r, int* dx);
public:
	void	FilterGridMotion 	(const Rect& r, int* dx, int* dy);
	bool	IsOnSolidGround	(const Rect& r) {
		int dy = 1;
		FilterGridMotionDown (r, &dy);
		return dy == 0;
	}
	GridMap* GetBuffer (void) { return &grid;}
	GridLayer (unsigned rows, unsigned cols);
};

class TileLayer {
private:
	TileMap 		map;
	GridLayer*	grid = nullptr;
	Dim			totalRows = 0, totalColumns = 0;
	Bitmap		tileSet = nullptr;
	Rect			viewWin;
	Bitmap		dpyBuffer = nullptr;
	bool			dpyChanged = true;
	Dim			dpyX = 0, dpyY = 0;
	void	Allocate (void) {
		grid = new GridLayer(MAX_HEIGHT, MAX_WIDTH);
		dpyBuffer = BitmapCreate(640, 480);
	}
public:
	void 	SetTile (Dim col, Dim row, Index index) {
		if (index == 65535) { index = 8;}
		map[row][col] = index;
	}
	Index GetTile (Dim col, Dim row) const {
		return map[row][col];
	}
	const Point Pick (Dim x, Dim y) const {
		return {DIV_TILE_WIDTH(x + viewWin.x), DIV_TILE_HEIGHT(y + viewWin.y)};
	}
	const Rect& GetViewWindow (void) const { return viewWin;}
	void 	SetViewWindow (const Rect& r) { viewWin = r; dpyChanged = true;}
	void 	Display (Bitmap dest, const Rect& displayArea);

	Bitmap 	GetBitmap 			(void) const { return dpyBuffer;}
	int 		GetMapPixelWidth 	(void) const { return totalColumns*TILE_WIDTH;}
	int 		GetMapPixelHeight (void) const { return totalRows*TILE_HEIGHT;}
	unsigned GetTileWidth 		(void) const { return DIV_TILE_WIDTH(viewWin.w);}
	unsigned GetTileHeight 		(void) const { return DIV_TILE_HEIGHT(viewWin.h);}

	TileMap* 	GetTileMap		(void) { return &map;}
	GridLayer*	GetGridLayer	(void) { return grid;}

	void Scroll 			(int dx, int dy);
	bool CanScrollHoriz	(int dx) const;
	bool CanScrollVert	(int dy) const;

	bool Load (const std::string& path) {
		tileSet = al_load_bitmap(path.c_str());
		return tileSet;
	}

	bool ReadText (std::string path);
	TileLayer (Dim _rows, Dim _cols, Bitmap _tileset);
	~TileLayer ();
};
 
extern TileLayer* tlayer;

#endif