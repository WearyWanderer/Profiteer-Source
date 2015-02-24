#pragma once

#include <string>
#include <HAPI_lib.h>

class Clsrectangle
{
public:

	Clsrectangle(int l, int r, int t, int b) : left(l), right(r), top(t), bottom(b){};
	~Clsrectangle();

	void ClipToRect(const Clsrectangle destRect);
	void TranslateToRect(int posX, int posY);
	void MoveRect(int x, int y, int width, int height); //completely move the rect to a new location, for respawn or teleporting concepts etc.
	inline bool CheckCompletelyInside(const Clsrectangle sourceRect)
	{
		if (right <= sourceRect.right && left >= sourceRect.left && top >= sourceRect.top && bottom <= sourceRect.bottom)
		{
			return true;
		}
		return false;
	}
	inline bool CheckCompletelyOutside(const Clsrectangle destRect)
	{
		if (right < 0 || left > destRect.right || top > destRect.bottom || bottom < destRect.top)
		{
			return true;
		}
		return false;
	}
	//overloaded functions below for non-auto updated rectangles
	inline bool CheckCompletelyInside(const Clsrectangle sourceRect, int x, int y)
	{
		TranslateToRect(x, y);
		if (right <= sourceRect.right && left >= sourceRect.left && top >= sourceRect.top && bottom <= sourceRect.bottom)
		{
			return true;
		}
		return false;
	}
	inline bool CheckCompletelyOutside(const Clsrectangle destRect, int x, int y)
	{
		TranslateToRect(x, y);
		if (right < 0 || left > destRect.right || top > destRect.bottom || bottom < destRect.top)
		{
			return true;
		}
		return false;
	}
	inline int GetWidth() const { return right - left; }
	inline int GetHeight() const { return bottom - top; }
	int left, right, top, bottom;

private:




};

class Clstexture
{
public:
	Clstexture();
	~Clstexture();

	void SetName(std::string name);
	bool InitTexture(std::string filename);
	bool InitTexture(std::string filename, int tilesizeX, int tilesizeY, int frameCount);
	inline bool checkIfSpritesheet(){ return isSpritesheet; }
	inline void forceSpritesheet() { isSpritesheet = true; } //functionality to force a texture to be spritesheet at a later point (Profiteer uses this for the custom button cutoff rect)
	int ReturnHeight();
	int ReturnWidth();
	inline int ReturnFrameCount(){ return animFrameCount; }
	BYTE* ReturnPtr();
	BYTE* ReturnPtr(int frameID);
	Clsrectangle ReturnRect();
	Clsrectangle ReturnRect(int x, int y);

	int spriteSizeX, spriteSizeY; //this is only filled if the constructor explicitly has it, in which case this is a animated spritesheet

private:
	std::string texName;
	BYTE* data;
	int width;
	int height;
	Clsrectangle sourceRect = Clsrectangle(0, 0, 0, 0);

	//below is stuff used by animated textures
	
	bool isSpritesheet = false;
	int animFrameCount;
};
