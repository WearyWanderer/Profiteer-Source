#include "ClsMapData.hpp"

void Tile::InitTile(int x, int y, Clstexture* usedTexture, int isoID)
{
	this->x = x;
	this->y = y;
	this->usedTexture = usedTexture;
	this->isoID = isoID;
	this->tileRect.right = usedTexture->ReturnWidth();
	this->tileRect.bottom = usedTexture->ReturnHeight();

	
}

#pragma region Constructor/Destructor

ClsMapData::ClsMapData()
{
	
}

ClsMapData::~ClsMapData()
{
	for (unsigned int i = 0; i < tilesList.size(); i++)
	{
		//some destructio of the sprite data here
		delete tilesList[i];
	}
	for (unsigned int i = 0; i < buildingList.size(); i++)
	{
		//some destructio of the sprite data here
		delete buildingList[i];
	}
}

#pragma endregion


