#pragma once

#include <HAPI_lib.h>
#include "texture.hpp"
#include "Point.h"
#include <string>
#include <memory>

#define TILE_WIDTH_HALF 32
#define TILE_HEIGHT_HALF 16

extern int screenHeight;
extern int screenWidth;
extern int tileWidth;
extern int tileHeight;

struct StrStar{
	int x, y; 
	float z;
	void birth(int x, int y);
};

using namespace std;

void ClearBackground(HAPI_TColour colour, BYTE* screenptr);

void DrawPixel(int r, int g, int b, int a, BYTE* screenptr, int x, int y);

void InitStarfield(StrStar stars[], int starSize);

void CreateStarfield(BYTE* screenptr, StrStar stars[], int starSize);

void ArrayToIso(int* gridArray);

#pragma region drawingFuns
void DrawTexture(Clstexture* texture, BYTE* screenptr, int x, int y, int frameID);

void DrawTextureAlpha(Clstexture* texture, BYTE* screenptr, int x, int y, int frameID);

void ClipTextureDraw(Clstexture * texture, BYTE* screenptr, const Clsrectangle destRect, const Clsrectangle sourceRect, int posX, int posY, int frameID);

void ClipTextureDrawAlpha(Clstexture * texture, BYTE* screenptr, const Clsrectangle destRect, const Clsrectangle sourceRect, int posX, int posY, int frameID);

void ClipTextureDrawAlpha(Clstexture* texture, BYTE* screenptr, const Clsrectangle destRect, const Clsrectangle sourceRect, int posX, int posY, int frameID, bool needsTranslate);
#pragma endregion

#pragma region isoFuns

Point tileToScreen(int xID, int yID);
Point screenToTile(int xpos, int ypos);

#pragma endregion