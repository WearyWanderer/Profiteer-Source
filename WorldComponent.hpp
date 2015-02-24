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
	Loads map information (eventually from a XML file)
	*/
	void LoadMapTiles(VisualComponent* textureLib, string filepath);
	void SeedBuildings(VisualComponent* textureLib);
	//checks below here
	bool checkMovementValidityList(int tileID, int sourceList[], int length);
	bool checkMovementTileAdjacency(int xAdj, int yAdj);

private:
	ClsMapData mapInstance;
	
};
	

