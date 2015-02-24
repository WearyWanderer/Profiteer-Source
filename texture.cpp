#include "texture.hpp"

#pragma region Textures
Clstexture::Clstexture()
{
	
}


Clstexture::~Clstexture()
{
	delete[] data;
}

void Clstexture::SetName(std::string name)
{
	texName = name;
}

bool Clstexture::InitTexture(std::string filename)
{

	if (!HAPI->LoadTexture(filename, &data, &width, &height))
	{
		HAPI->UserMessage("ERROR: Background Texture could not be found, please check your /art directory", "Uh-oh!", eButtonTypeOk);
		HAPI->Close();
		return false;
	}
		
	sourceRect.right = width;
	sourceRect.bottom = height;

	SetName(filename);

	return true;
}

bool Clstexture::InitTexture(std::string filename, int tilesizeX, int tilesizeY, int frameCount) //if this used, then must animate
{
	animFrameCount = frameCount;

	if (!HAPI->LoadTexture(filename, &data, &width, &height))
	{
		HAPI->UserMessage("ERROR: A texture could not be found, please check your /art directory for " + filename, "Uh-oh!", eButtonTypeOk);
		HAPI->Close();
		return false;
	}

	width = tilesizeX;
	height = tilesizeY;

	spriteSizeX = tilesizeX;
	spriteSizeY = tilesizeY;
	isSpritesheet = true;

	sourceRect.right = spriteSizeX;
	sourceRect.bottom = spriteSizeY;

	SetName(filename);

	return true;
}

int Clstexture::ReturnHeight()
{
	return height;
}

int Clstexture::ReturnWidth()
{
	return width;
}

BYTE* Clstexture::ReturnPtr()
{
	return data;
}
BYTE* Clstexture::ReturnPtr(int frameID) //calculate ptr if spritesheet and onto current frameID
{
	return data + (frameID * (spriteSizeX * spriteSizeY) * 4);
}

#pragma endregion

#pragma region Rectangle

Clsrectangle::~Clsrectangle()
{
	
}

void Clsrectangle::ClipToRect(const Clsrectangle destRect)
{
	if (left < destRect.left)
		left = destRect.left;

	if (right > destRect.right)
		right = destRect.right;

	if (top < destRect.top)
		top = destRect.top;

	if (bottom > destRect.bottom)
		bottom = destRect.bottom;
}

void Clsrectangle::TranslateToRect(int posX, int posY)
{
	left += posX;
	right += posX;
	top += posY;
	bottom += posY;
}

void Clsrectangle::MoveRect(int x, int y, int width, int height)
{
	left = x;
	right = x + width;
	top = y;
	bottom = y + height;
}

Clsrectangle Clstexture::ReturnRect()
{
	return sourceRect;
}

Clsrectangle Clstexture::ReturnRect(int x, int y)
{
	Clsrectangle tempRect = sourceRect;
	tempRect.TranslateToRect(x, y);
	return tempRect;
}

#pragma endregion