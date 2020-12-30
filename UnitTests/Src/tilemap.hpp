#ifndef TILE_HPP
#define TILE_HPP
#include "bitmap.hpp"

typedef unsigned char byte;
typedef unsigned short Dim;

byte MakeIndex (byte row, byte col);
byte GetCol (byte index);
byte GetRow (byte index);

Dim TileX (byte index);
Dim TileY (byte index);

typedef unsigned short Index;

#define MAX_WIDTH 12*16
#define MAX_HEIGHT 21*16

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

#endif