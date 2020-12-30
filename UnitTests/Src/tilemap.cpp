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
	return GetCol(index)*16;
}

Dim TileY (byte index) {
	return GetRow(index)*16;
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
	std::vector<unsigned short> tempv (640, 61);
	std::ifstream file(path);
	int i = 0;
	std::string input;
	while (file >> input) {
		std::string temp;
		al.map.indexmap.push_back(tempv);
		int j = 0;
		for (auto it = input.cbegin(); it!=input.cend(); ++it) {
			if (*it == ',') {
				//setTile(&tilemap, j++, i, std::stoi(temp));
				al.map.indexmap[i][j++] = std::stoi(temp);
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