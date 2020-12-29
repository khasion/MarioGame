#include "tilemap.hpp"

TileMap tilemap;

byte MakeIndex (byte row, byte col) {
	return (col << COL_SHIFT) | row;
}

byte getCol (byte index) {
	return index >> COL_SHIFT;
}

byte getRow (byte index) {
	return index & ROW_MASK;
}

Dim TileX (byte index) {
	return getCol(index) * TILE_WIDTH;
}

Dim TileY (byte index) {
	return getRow(index) * TILE_HEIGHT;
}

Dim TileX2 (byte index) {
	return MUL_TILE_WIDTH(getCol(index));
}

Dim TileY2 (byte index) {
	return MUL_TILE_HEIGHT(getRow(index));
}

Index MakeIndex2(byte row, byte col) {
	return ((MUL_TILE_WIDTH(col) << TILEX_SHIFT) | MUL_TILE_HEIGHT(row));
}

Dim TileX3 (Index index) { return index >> TILEX_SHIFT;}
Dim TileY3 (Index index) { return index & TILEY_MASK;}

void setTile(TileMap* m, Dim col, Dim row, Index index) {
	(*m)[row][col] = index;
}

void PutTile (Bitmap dest, Dim x, Dim y, Bitmap tiles, Index tile) {
	BitmapBlit(tiles, Rect{TileX3(tile), TileY3(tile), TILE_WIDTH, TILE_HEIGHT},
	dest, Point {x, y});
}

Index GetTile (const TileMap* m, Dim col, Dim row) {
	return (*m)[row][col];
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
/*	std::vector<std::vector<int>> v;
	std::vector<int> tempv;

	ALLEGRO_BITMAP** sprite_map = al.map.tilemap;*/
	std::ifstream file(path);
	int i = 0;
	std::string input;
	while (file >> input) {
		std::string temp;
		for (auto it = input.cbegin(); it!=input.cend(); ++it) {
			if (*it == ',') {
				setTile(&tilemap, getCol(i), getRow(i), std::stoi(temp));
				std::cout << tilemap[getRow(i)][getCol(i)] << std::endl;
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