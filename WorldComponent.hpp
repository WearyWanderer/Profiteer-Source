#pragma once

#include <iostream>
#include <fstream> //reading in the ID's for movement from a text file
#include <algorithm>
#include "ClsMapData.hpp"
#include "VisualComponent.hpp" //grabs textures from the visual component of the game (referred to in these kind of circumstances as the textureLib due to it's purpose)
#include "PugiXml\pugixml.hpp"

class WorldComponent
{
public:
	WorldComponent();
	~WorldComponent();

	/**
	Used to access the tile information for drawing
	*/
	inline std::vector<Tile*> GetMapData(){ return mapInstance.tilesList; }
	inline ClsMapData* GetMapObject(){ return &mapInstance; }

	/**
	Loads map information from an XML file
	*/
	void LoadMapTiles(VisualComponent* textureLib, string filepath);
	/**
	Cleans up map info and resizes vector to 0
	*/
	void CleanMap();
	/**
	Seeds buildings to the map
	*/
	void SeedBuildings(VisualComponent* textureLib);
	/**
	Loads map information from a .map file for AI test
	*/
	void LoadAIMapTiles(VisualComponent* textureLib, string filepath);

	//checks below here----------------
	bool checkMovementValidityList(int tileID, int sourceList[], int length);
	bool checkMovementTileAdjacency(int xAdj, int yAdj);

private:
	ClsMapData mapInstance;
	
};
	

