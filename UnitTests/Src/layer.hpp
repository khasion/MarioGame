#ifndef LAYER_HPP
#define LAYER_HPP
#include "view.hpp"

class Background {
private:
	TileMap	map;
	Bitmap	tileSet = nullptr;
	Bitmap 	bg = nullptr;
	Rect 		viewWin;
public:
	void Scroll (int dx) {
		viewWin.x += dx;
		if (viewWin.x < 0) {
			viewWin.x = BitmapGetWidth(bg) + viewWin.x;
		}
		else if (viewWin.x >= BitmapGetWidth(bg)) {
			viewWin.x = viewWin.x - BitmapGetWidth(bg);
		}
	}
	void Display (Bitmap dest, int x, int y) {
		auto bg_w = BitmapGetWidth(bg);
		auto w1 = std::min(bg_w - viewWin.x, viewWin.w);
		BitmapBlit(bg, {viewWin.x, viewWin.y, w1, viewWin.h}, dest, {x, y});
		if (w1 < viewWin.w) {
			auto w2 = viewWin.w - w1;
			BitmapBlit(bg, {0, viewWin.y, w2, viewWin.h}, dest, {x + w1, y});
		}
	}
	void LoadBackground (std::string path) {
		int i = 0;
		for (auto row = 0; row < MAX_HEIGHT; row++) {
			for (auto col = 0; col < MAX_WIDTH; col++) {
				map[row][col] = 8;
			}
		}
		std::ifstream file(path);
		std::string input;
		while (file >> input) {
			std::string temp;
			int j = 0;
			for (auto it = input.cbegin(); it!=input.cend(); ++it) {
				if (*it == ',') {
					int index = std::stoi(temp);
					if (index == -1) { index = 8;}
					PutTile(bg, 16*j++, 16*i, tileSet, index);
					temp.clear();
				}
				else { temp.push_back(*it);}
			}
			i++;
		}
	}
	Background (Bitmap bmp) {
		tileSet = bmp;
		viewWin = {0, 0, 640, 480};
		bg = BitmapCreate(640, 480);
	}
};

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
	TileMap 			map;
	GridLayer*		grid = nullptr;
	Background* 	bg = nullptr;
	Dim				totalRows = 0, totalColumns = 0;
	Bitmap			tileSet = nullptr;
	Rect				viewWin;
	Bitmap			dpyBuffer = nullptr;
	bool				dpyChanged = true;
	Dim				dpyX = 0, dpyY = 0;
	void	Allocate (void) {
		grid = new GridLayer(MAX_HEIGHT, MAX_WIDTH);
		bg = new Background(tileSet);
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

	TileMap* 				GetTileMap	(void) 	{ return &map;}
	GridLayer*				GetGridLayer(void) 	{ return grid;}
	Background*				GetBg			(void)	{ return bg;}

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