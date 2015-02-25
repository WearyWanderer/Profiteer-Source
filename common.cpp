#include "common.hpp"

using namespace std;

/*
#pragma region StarfieldFunctionality
void StrStar::birth(int x, int y){
	this->x = rand() % x;
	this->y = rand() % y;
	this->z = (float)(rand() % 100);
}

void InitStarfield(StrStar stars[], int starSize)
{
	for (int i = 0; i < starSize; i++)
	{
		stars[i].birth(screenWidth, screenHeight);
	}
}

void CreateStarfield(BYTE* screenptr, StrStar stars[], int starSize)
{
	int eyeDist = 100;
	ClearBackground(0, 0, 0, 255, screenptr);
	//star logic
	for (int i = 0; i < starSize; i++){
		stars[i].z -= 0.5f;
		//Check for stars becoming out of range and regenerate them should this occur.
		if (stars[i].z < 0 || stars[i].x<0 || stars[i].y<0 || stars[i].x>screenWidth || stars[i].y>screenHeight)
		{
			stars[i].birth(screenWidth, screenHeight);
		}
		//Calculate the 3D co-ordinates of the star.
		int starX = (int)(((eyeDist * (stars[i].x - screenWidth / 2)) / (eyeDist + stars[i].z)) + screenWidth / 2);
		int starY = (int)(((eyeDist * (stars[i].y - screenHeight / 2)) / (eyeDist + stars[i].z)) + screenHeight / 2);
		//Render the star.
		DrawPixel(255, 255, 255, 255, screenptr, starX, starY);
	}

}

#pragma endregion
*/

#pragma region DrawingFunctionality
void DrawPixel(int r, int g, int b, int a, BYTE* screenptr, int x, int y)
{
	HAPI_TColour col(r, g, b, a);

	int offset = (x + y * screenWidth) * 4;
	BYTE* pixel = screenptr + offset;
	*((DWORD*)(pixel)) = *((DWORD*)&col);

}

void ClearBackground(HAPI_TColour colour, BYTE* screenptr)
{
	//int offset = ((screenWidth - 1) + (screenHeight - 1)  * screenWidth) * 4;
	////safer way to maintain memory bounds
	//offset += 4;

	//for (int i = 0; i < offset; i += 4)
	//{
	//	BYTE* pixel = screenptr + i;
	//	memcpy(pixel, &colour, 4);
	//}

	memset(screenptr, 0, screenHeight * screenWidth * 4);
}

void DrawTexture(Clstexture* texture, BYTE* screenptr, int x, int y, int frameID)
{
	int height;
	int width;
	int destRow = screenWidth;
	BYTE* currentTexPixel;
	BYTE* destScreenPtr = screenptr + (x + y * destRow) * 4;

	height = texture->ReturnHeight();
	width = texture->ReturnWidth();

	//run a check if the frame is needed, otherwise just render texture as normal
	if (texture->checkIfSpritesheet())
		currentTexPixel = texture->ReturnPtr(frameID);
	else
		currentTexPixel = texture->ReturnPtr();


	//optimised to do whole rows at a time
	for (int i = 0; i < height; ++i)
	{
		memcpy(destScreenPtr, currentTexPixel, (width * 4));

		//advance to next row of pixels on screen
		currentTexPixel += width * 4;
		destScreenPtr += destRow * 4;

	}

}

void DrawTextureAlpha(Clstexture* texture, BYTE* screenptr, int x, int y, int frameID)
{
	int height;
	int width;
	int destRow = screenWidth;
	BYTE* currentTexPixel;
	BYTE* destScreenPtr = screenptr + (x + y * destRow) * 4;

	height = texture->ReturnHeight();
	width = texture->ReturnWidth();

	//run a check if the frame is needed, otherwise just render texture as normal
	if (texture->checkIfSpritesheet())
		currentTexPixel = texture->ReturnPtr(frameID);
	else
		currentTexPixel = texture->ReturnPtr();

	//create a HAPI colour struct and copy the current pixel's data into it to check for opacity. Then decide whether to blend [NOTE: Refactored to just overwrite each frame rather than reinitialising a new HAPI_TColour ever frame
	HAPI_TColour colour;
	HAPI_TColour screenPixelColour;

	//because of the individual checking of pixels for the alpha channel, needed to do all pixels seperately
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			int offset = (x + y * width) * 4;	

			//memcpy(&colour, currentTexPixel + offset, 4);
			BYTE* colour = currentTexPixel + offset;
			if (colour[3] != 0)
			{
				if (colour[3] == 255)
				{
					*((DWORD*)(destScreenPtr + offset)) = *((DWORD*)colour);
					//memcpy(destScreenPtr + offset, currentTexPixel + offset, 4);
				}
				else
				{
					*((DWORD*)&screenPixelColour) = *((DWORD*)(destScreenPtr + offset));
					//memcpy(&screenPixelColour, destScreenPtr + offset, 4);

					//blend the taken pixel with what we are adding to it, factor in the alpha channel
					screenPixelColour.blue = screenPixelColour.blue + ((colour[3] * (colour[0] - screenPixelColour.blue)) >> 8);
					screenPixelColour.green = screenPixelColour.green + ((colour[3] * (colour[1] - screenPixelColour.green)) >> 8);
					screenPixelColour.red = screenPixelColour.red + ((colour[3] * (colour[2] - screenPixelColour.red)) >> 8);

					*((DWORD*)(destScreenPtr + offset)) = *((DWORD*)&screenPixelColour);
					//memcpy(destScreenPtr + offset, &screenPixelColour, 4);
				}
			}
		}
		destScreenPtr += (destRow - width) * 4;
	}
}

void ClipTextureDraw(Clstexture* texture, BYTE* screenptr, const Clsrectangle destRect, const Clsrectangle sourceRect, int posX, int posY, int frameID)
{
	Clsrectangle clippedRect = sourceRect;

	clippedRect.ClipToRect(destRect);

	clippedRect.TranslateToRect(-posX, -posY);

	clippedRect.ClipToRect(destRect);

	if (posX < 0)
		posX = 0;

	if (posY < 0)
		posY = 0;

	int height = clippedRect.bottom;
	int width = clippedRect.right;
	int destRow = screenWidth;
	BYTE* currentTexPixel;

	//run a check if the frame is needed, otherwise just render texture as normal
	if (texture->checkIfSpritesheet())
		currentTexPixel = texture->ReturnPtr(frameID) + (clippedRect.left + clippedRect.top * sourceRect.GetWidth()) * 4;
	else
		currentTexPixel = texture->ReturnPtr() + (clippedRect.left + clippedRect.top * sourceRect.GetWidth()) * 4;

	BYTE* destScreenPtr = screenptr + (posX + posY * destRow) * 4;

	//optimised to do whole rows at a time
	if (clippedRect.GetWidth() > 0)
		for (int i = clippedRect.top; i < clippedRect.GetHeight(); ++i)
		{
		memcpy(destScreenPtr, currentTexPixel, (clippedRect.GetWidth() * 4));
		//advance to next row of pixels on screen
		currentTexPixel += sourceRect.GetWidth() * 4;
		destScreenPtr += destRow * 4;

		}


}

void ClipTextureDrawAlpha(Clstexture* texture, BYTE* screenptr, const Clsrectangle destRect, const Clsrectangle sourceRect, int posX, int posY, int frameID)
{
	Clsrectangle clippedRect = sourceRect;

	clippedRect.ClipToRect(destRect);

	clippedRect.TranslateToRect(-posX, -posY);

	if (posX < 0)
		posX = 0;

	if (posY < 0)
		posY = 0;

	int destRow = screenWidth;
	BYTE* currentTexPixel = texture->ReturnPtr();
	BYTE* destScreenPtr = screenptr + (posX + posY * screenWidth) * 4;

	//run a check if the frame is needed, otherwise just render texture as normal
	if (texture->checkIfSpritesheet())
		currentTexPixel = texture->ReturnPtr(frameID) + (clippedRect.left + clippedRect.top * sourceRect.GetWidth()) * 4;
	else
		currentTexPixel = texture->ReturnPtr() + (clippedRect.left + clippedRect.top * sourceRect.GetWidth()) * 4;

	int endOfLineDestIncrement = (screenWidth - (clippedRect.right - clippedRect.left)) * 4;
	int endOfLineSrcIncrement = (sourceRect.GetWidth() - (clippedRect.right - clippedRect.left)) * 4;

	HAPI_TColour colour;
	HAPI_TColour screenPixelColour;

	//if not completely off screen
	if (clippedRect.GetHeight() > 0 && clippedRect.GetWidth() > 0)
		for (int y = clippedRect.top; y < clippedRect.bottom; ++y)
		{
		for (int x = clippedRect.left; x < clippedRect.right; ++x)
		{

			memcpy(&colour, currentTexPixel, 4);

			if (colour.alpha != 0)
			{
				if (colour.alpha == 255)
				{
					*((DWORD*)(destScreenPtr)) = *((DWORD*)currentTexPixel);
				}
				else
				{	
					memcpy(&screenPixelColour, destScreenPtr, 4);

					//blend the taken pixel with what we are adding to it, factor in the alpha channel
					screenPixelColour.blue = screenPixelColour.blue + ((colour.alpha * (colour.blue - screenPixelColour.blue)) >> 8);
					screenPixelColour.green = screenPixelColour.green + ((colour.alpha * (colour.green - screenPixelColour.green)) >> 8);
					screenPixelColour.red = screenPixelColour.red + ((colour.alpha * (colour.red - screenPixelColour.red)) >> 8);

					*((DWORD*)(destScreenPtr)) = *((DWORD*)&screenPixelColour);
				}
			}
			destScreenPtr += 4;
			currentTexPixel += 4;
		}

		currentTexPixel += endOfLineSrcIncrement;
		destScreenPtr += endOfLineDestIncrement;
		}




}

void ClipTextureDrawAlpha(Clstexture* texture, BYTE* screenptr, const Clsrectangle destRect, const Clsrectangle sourceRect, int posX, int posY, int frameID, bool needsTranslate) //overload for tiles as they do not change xpos
{
	Clsrectangle clippedRect = sourceRect;

	clippedRect.TranslateToRect(posX, posY); // this needed adding for non entities, as they update rectangle automatically

	clippedRect.ClipToRect(destRect);

	clippedRect.TranslateToRect(-posX, -posY);

	if (posX < 0)
		posX = 0;

	if (posY < 0)
		posY = 0;

	int destRow = screenWidth;
	BYTE* currentTexPixel = texture->ReturnPtr();
	BYTE* destScreenPtr = screenptr + (posX + posY * screenWidth) * 4;

	//run a check if the frame is needed, otherwise just render texture as normal
	if (texture->checkIfSpritesheet())
		currentTexPixel = texture->ReturnPtr(frameID) + (clippedRect.left + clippedRect.top * sourceRect.GetWidth()) * 4;
	else
		currentTexPixel = texture->ReturnPtr() + (clippedRect.left + clippedRect.top * sourceRect.GetWidth()) * 4;


	int endOfLineDestIncrement = (screenWidth - (clippedRect.right - clippedRect.left)) * 4;
	int endOfLineSrcIncrement = (sourceRect.GetWidth() - (clippedRect.right - clippedRect.left)) * 4;

	HAPI_TColour colour;
	HAPI_TColour screenPixelColour;
	//if not completely off screen
	if (clippedRect.GetHeight() > 0 && clippedRect.GetWidth() > 0)
		for (int y = clippedRect.top; y < clippedRect.bottom; ++y)
		{
		for (int x = clippedRect.left; x < clippedRect.right; ++x)
		{
			memcpy(&colour, currentTexPixel, 4);

			if (colour.alpha != 0)
			{
				if (colour.alpha == 255)
				{
					*((DWORD*)(destScreenPtr)) = *((DWORD*)currentTexPixel);
				}
				else
				{
					memcpy(&screenPixelColour, destScreenPtr, 4);

					//blend the taken pixel with what we are adding to it, factor in the alpha channel
					screenPixelColour.blue = screenPixelColour.blue + ((colour.alpha * (colour.blue - screenPixelColour.blue)) >> 8);
					screenPixelColour.green = screenPixelColour.green + ((colour.alpha * (colour.green - screenPixelColour.green)) >> 8);
					screenPixelColour.red = screenPixelColour.red + ((colour.alpha * (colour.red - screenPixelColour.red)) >> 8);

					*((DWORD*)(destScreenPtr)) = *((DWORD*)&screenPixelColour);
				}
			}
			destScreenPtr += 4;
			currentTexPixel += 4;
		}
		currentTexPixel += endOfLineSrcIncrement;
		destScreenPtr += endOfLineDestIncrement;
		}
}

#pragma endregion

#pragma region IsometricTools

Point tileToScreen(int xID, int yID)
{
	int X = (xID - yID) * TILE_WIDTH_HALF;
	int Y = (xID + yID) * TILE_HEIGHT_HALF;
	Point thisPoint(X, Y);

	thisPoint.x += 775; //offsetting to start at central point of map
	thisPoint.y -= 450; //off cont

	return thisPoint;
}

Point screenToTile(int xpos, int ypos) //unused
{
	Point thisPoint(0, 0);

	return thisPoint;
}

#pragma endregion