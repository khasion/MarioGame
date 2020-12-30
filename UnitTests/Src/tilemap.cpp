#include "tilemap.hpp"

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
						MUL_TILE_WIDTH(col - startCol),
						MUL_TILE_HEIGHT(row - startRow),
						al.map.bitmap,
						GetTile(map, row, col)
				);
			}
		}
	}
	BitmapBlit(dpyBuffer, {dpyX, dpyY, viewWin.w, viewWin.h},
	dest,
	{ displayArea.x, displayArea.y});
}