#include "layer.hpp"

TileLayer* tlayer;
std::vector<std::pair<int, int>> coins_xy;
std::vector<std::pair<int, int>> boxes_xy;
std::vector<std::pair<int, int>> mush_xy;
std::vector<std::pair<int, int>> star_xy;
std::vector<std::pair<int, int>> up_xy;
std::vector<std::pair<int, int>> koopa_xy;
std::vector<std::pair<int, int>> redkoopa_xy;

void GridLayer::FilterGridMotionDown (const Rect& r, int* dy) {
	auto y2 = r.y + r.h + 1;
	auto y2_next = y2 + *dy;
	if (y2_next >= MAX_PIXEL_HEIGHT) {
		*dy = MAX_PIXEL_WIDTH - y2;
	}
	else {
		auto newRow = DIV_GRID_ELEMENT_HEIGHT(y2_next);
		auto currRow = DIV_GRID_ELEMENT_HEIGHT(y2);
		if (newRow != currRow) {
			assert(newRow - 1 == currRow);
			auto startCol = DIV_GRID_ELEMENT_WIDTH(r.x);
			auto endCol = DIV_GRID_ELEMENT_WIDTH(r.x + r.w -1);
			for (auto col = startCol; col <= endCol; ++col) {
				if (CanPassGridTile(&grid, col, newRow, GRID_TOP_SOLID_MASK)) {
					*dy = MUL_GRID_ELEMENT_WIDTH(newRow) - (y2 + 1);
					break;
				}
			}
		}
	}
}
void GridLayer::FilterGridMotionUp (const Rect& r, int* dy) {
	auto y1_next = r.y + *dy;
	if (y1_next < 0) {
		*dy = -r.y;
	}
	else {
		auto newRow = DIV_GRID_ELEMENT_WIDTH(y1_next);
		auto currRow = DIV_GRID_ELEMENT_WIDTH(r.y);
		if (newRow != currRow) {
			assert (newRow + 1 == currRow);
			auto startCol = DIV_GRID_ELEMENT_WIDTH(r.x);
			auto endCol = DIV_GRID_ELEMENT_WIDTH(r.x  + r.w  - 1);
			for (auto col = startCol; col <= endCol; ++col) {
				if (CanPassGridTile(&grid, col, newRow, GRID_BOTTOM_SOLID_MASK)) {
					*dy = MUL_GRID_ELEMENT_WIDTH(currRow) - r.y;
					break;
				}
			}
		}
	}
}
void GridLayer::FilterGridMotionLeft (const Rect& r, int* dx) {
	auto x1_next = r.x + *dx;
	if (x1_next < 0) {
		*dx = -r.x;
	}
	else {
		auto newCol = DIV_GRID_ELEMENT_WIDTH(x1_next);
		auto currCol = DIV_GRID_ELEMENT_WIDTH(r.x);
		if (newCol != currCol) {
			assert (newCol + 1 == currCol);
			auto startRow = DIV_GRID_ELEMENT_HEIGHT(r.y);
			auto endRow = DIV_GRID_ELEMENT_HEIGHT(r.y  + r.h  - 1);
			for (auto row = startRow; row <= endRow; ++row) {
				if (CanPassGridTile(&grid, newCol, row, GRID_RIGHT_SOLID_MASK)) {
					*dx = MUL_GRID_ELEMENT_WIDTH(currCol) - r.x;
					break;
				}
			}
		}
	}
}
void GridLayer::FilterGridMotionRight (const Rect& r, int* dx) {
	auto x2 = r.x + r.w + 1;
	auto x2_next = x2 + *dx;
	if (x2_next >= MAX_PIXEL_WIDTH) {
		*dx = MAX_PIXEL_WIDTH - x2;
	}
	else {
		auto newCol = DIV_GRID_ELEMENT_WIDTH(x2_next);
		auto currCol = DIV_GRID_ELEMENT_WIDTH(x2);
		if (newCol != currCol) {
			assert(newCol - 1 == currCol);
			auto startRow = DIV_GRID_ELEMENT_HEIGHT(r.y);
			auto endRow = DIV_GRID_ELEMENT_HEIGHT(r.y + r.h -1);
			for (auto row = startRow; row <= endRow; ++row) {
				if (CanPassGridTile(&grid, newCol, row, GRID_LEFT_SOLID_MASK)) {
					*dx = MUL_GRID_ELEMENT_WIDTH(newCol) - (x2 + 1);
					break;
				}
			}
		}
	}
}
void GridLayer::FilterGridMotion (const Rect& r, int* dx, int* dy) {
	assert(abs(*dx) <= GRID_ELEMENT_WIDTH && abs(*dy) <= GRID_ELEMENT_HEIGHT);
	if (*dx < 0) {
		FilterGridMotionLeft(r, dx);
	}
	else if (*dx > 0) {
		FilterGridMotionRight(r, dx);
	}
	if (*dy < 0) {
		FilterGridMotionUp(r, dy);
	}
	else if (*dy > 0) {
		FilterGridMotionDown(r, dy);
	}
}

GridLayer::GridLayer (unsigned rows, unsigned cols) {
	totalRows = rows; totalColumns = cols;
	total = rows * cols;
	Allocate();
}

Index IsEntity (Index index, int x, int y) {
	switch (index) {
		case COIN_INDEX:
			coins_xy.push_back(std::make_pair(MUL_TILE_WIDTH(x), MUL_TILE_HEIGHT(y)));
			return EMPTY_INV_INDEX;
		case BOX_INDEX:
			boxes_xy.push_back(std::make_pair(MUL_TILE_WIDTH(x), MUL_TILE_HEIGHT(y)));
			return SOLID_INV_INDEX;
		case MUSH_INDEX:
			mush_xy.push_back(std::make_pair(MUL_TILE_WIDTH(x), MUL_TILE_HEIGHT(y)));
			return EMPTY_INV_INDEX;
		case STAR_INDEX:
			star_xy.push_back(std::make_pair(MUL_TILE_WIDTH(x), MUL_TILE_HEIGHT(y)));
			return EMPTY_INV_INDEX;
		case UP_INDEX:
			up_xy.push_back(std::make_pair(MUL_TILE_WIDTH(x), MUL_TILE_HEIGHT(y)));
			return EMPTY_INV_INDEX;
		case KOOPA_INDEX:
			koopa_xy.push_back(std::make_pair(MUL_TILE_WIDTH(x), MUL_TILE_HEIGHT(y)));
			return EMPTY_INV_INDEX;
		case REDKOOPA_INDEX:
			redkoopa_xy.push_back(std::make_pair(MUL_TILE_WIDTH(x), MUL_TILE_HEIGHT(y)));
			return EMPTY_INV_INDEX;
	}
	return index;
}

bool TileLayer::ReadText (std::string path) {
	std::ifstream file(path);
	int i = 0;
	for (auto row = 0; row < MAX_HEIGHT; row++) {
		for (auto col = 0; col < MAX_WIDTH; col++) {
			SetTile(col, row, 8);
		}
	}
	std::string input;
	while (file >> input) {
		std::string temp;
		int j = 0;
		for (auto it = input.cbegin(); it!=input.cend(); ++it) {
			if (*it == ',') {
				Index index = IsEntity(std::stoi(temp), j, i);
				SetTile(j++, i, index);
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

bool TileLayer::CanScrollHoriz (int dx) const {
	auto val = dx + viewWin.x;
	if (val < 0) {
		dx = viewWin.x;
		return false;
	}
	else if ((val + viewWin.w) >= GetMapPixelWidth()) {
		dx = GetMapPixelWidth() - (viewWin.x + viewWin.w);
		return false;
	}
	return true;
}

bool TileLayer::CanScrollVert (int dy) const {
	auto val = dy + viewWin.y;
	if (val < 0) {
		dy = viewWin.y;
		return false;
	}
	else if ((val + viewWin.h) >= GetMapPixelHeight()) {
		dy = GetMapPixelHeight() - (viewWin.y + viewWin.h);
		return false;
	}
	return true;
}

void TileLayer::Scroll (int dx, int dy) {
	if (CanScrollHoriz(dx)) { bg->Scroll(dx); viewWin.x += dx;}
	if (CanScrollVert(dy)) 	{ viewWin.y += dy;}
}

void TileLayer::Display(Bitmap dest, const Rect& displayArea) {
	if(dpyChanged) {
		BitmapClear(dpyBuffer, RGB {0, 255, 255});
		auto startCol= DIV_TILE_WIDTH(viewWin.x);
		auto startRow= DIV_TILE_HEIGHT(viewWin.y);
		auto endCol= DIV_TILE_WIDTH(viewWin.x + viewWin.w-1);
		auto endRow= DIV_TILE_HEIGHT(viewWin.y + viewWin.h-1);
		dpyX = MOD_TILE_WIDTH(viewWin.x);
		dpyY = MOD_TILE_HEIGHT(viewWin.y);
		dpyChanged= false;
		bg->Display(dpyBuffer, dpyX, dpyY);
		for (Dim row = startRow; row <= endRow; ++row) {
			for(Dim col = startCol; col <= endCol; ++col) {
				PutTile(
						dpyBuffer,
						MUL_TILE_WIDTH(col-startCol),
						MUL_TILE_HEIGHT(row-startRow),
						tileSet,
						GetTile(col, row)
				);
			}
		}
	}
	BitmapBlit(dpyBuffer, {dpyX, dpyY, viewWin.w, viewWin.h},
	dest,
	{ displayArea.x, displayArea.y});
	DisplaySprites(dest, displayArea, tlayer);
}

TileLayer::TileLayer (Dim _rows, Dim _cols, Bitmap _tileset) {
	totalRows = _rows;
	totalColumns = _cols;
	tileSet = _tileset;
	viewWin = Rect {0, 0, 640, 480};
	Allocate();
}

TileLayer::~TileLayer () {
	al_destroy_bitmap((ALLEGRO_BITMAP*)tileSet);
}