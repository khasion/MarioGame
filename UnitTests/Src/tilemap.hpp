#ifndef TILE_HPP
#define TILE_HPP
#include "bitmap.hpp"

#define TILE_WIDTH 16
#define TILE_HEIGHT 16
#define ROW_MASK 0x0F
#define COL_MASK 0xF0
#define COL_SHIFT 4
typedef unsigned char byte;
typedef unsigned short Dim;

byte MakeIndex (byte row, byte col);
byte getCol (byte index);
byte getRow (byte index);

Dim TileX (byte index);

Dim TileY (byte index);

#define MUL_TILE_WIDTH(i)((i)<<4)
#define MUL_TILE_HEIGHT(i)((i)<<4)
#define DIV_TILE_WIDTH(i)((i)>>4)
#define DIV_TILE_HEIGHT(i)((i)&15)

Dim TileX2 (byte index);

Dim TileY2 (byte index);

typedef unsigned short Index;
#define TILEX_MASK 0xFF00
#define TILEX_SHIFT 8
#define TILEY_MASK 0x00FF

Index MakeIndex2(byte row, byte col);

Dim TileX3 (Index index);
Dim TileY3 (Index index);

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