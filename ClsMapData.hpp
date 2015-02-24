#pragma once

#include <string>
#include <vector>
#include <HAPI_lib.h>
#include "texture.hpp" //each tile uses a texture
#include "ClsBuilding.h"


//structure of each tile in the isometric grid
struct Tile
{
	int x, y;
	Clstexture* usedTexture;
	Clsrectangle tileRect = Clsrectangle(0, 0, 0, 0);
	int isoID;
	bool movementTile;
	void InitTile(int x, int y, Clstexture* usedTexture,int isoID);
	inline Clsrectangle GetRect(){ return tileRect; }

	bool requiresRedraw = true; //this bool should be changed if any sort of animation or movement occurs above a tile. This is to improve performance
	
	bool operator < (const Tile& str) const
	{
		return (x < str.x);
	}
	bool operator << (const Tile& str) const
	{
		return (y < str.y);
	}

};

class ClsMapData
{
public:
	ClsMapData();
	~ClsMapData();

	std::vector<Tile*> tilesList;
	std::vector<ClsBuilding*> buildingList;
	/**
	Used to access the tile information for drawing
	*/
	inline std::vector<Tile*> GetTileData(){ return tilesList; }
	/**
	Used to access the shop information for drawing or entity ownership
	*/
	inline std::vector<ClsBuilding*> GetBuildingData(){ return buildingList; }

private:
	int rows;
	int columns;
	std::string mapName;



};

