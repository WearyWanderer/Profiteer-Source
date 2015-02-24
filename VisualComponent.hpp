#pragma once

#include <HAPI_lib.h>
#include <vector>
#include "entity.hpp"
#include "common.hpp"
#include "ClsButton.h"
#include "Point.h"
#include <windows.h>
#include <stdio.h>
#include <iostream> //including for file reading function

enum gameStateEnums
{
	mainGame = 1,
	menu = 2,
	productCreateScreen = 3,
	endGame = 4,
	aiTest = 5
};

class VisualComponent
{
public:

	VisualComponent();
	~VisualComponent();

	inline void SetScreen(BYTE* screenptr) { screen = screenptr; }

	/**
	Clears the screen to a default colour at the start of each frame

	@param int - pass the current state in so the screen knows what to clear to
	*/
	void UpdateScreen(int state);

	/**
	This will check the state of our game component and decide what to draw
	*/
	void RenderCycle(int state, ClsMapData* mapRef, std::vector<Clsplayer*> entityList, Clsrectangle screenRect, std::vector<ClsButton*> buttonList, Point cameraOffset, Point mousePos, int officesScore, int profitScore);

	/**
	Renders entities onto the screen (to be used after the map drawing stage)

	@param std::vector<Clsentity*> entities to draw
	*/
	void RenderEntities(std::vector<Clsplayer*> entityList, Clsrectangle screenRect, Point cameraOffset);

	/**
	Renders map onto the screen (to be used before the entity drawing stage)

	@param ClsMapData* location of tile vector to draw tiles from
	*/
	void RenderMap(std::vector<Tile*> tileList, Clsrectangle screenRect, Point cameraOffset);

	/**
	Renders buildings across the map

	@param ClsMapData* location of tile vector to draw tiles from
	*/
	void RenderBuildings(std::vector<ClsBuilding*> buildingList, Clsrectangle screenRect, Point cameraOffset);

	/**
	Converts the x and y ID of a tile into suitable units for drawing
	*/
	void RenderUI(std::vector<ClsButton*> buttonList, std::vector<Clsplayer*> entityList, std::vector<ClsBuilding*> buildingData, Clsrectangle screenRect, Point cameraOffset, int state);

	/**
	Draws Mouse
	*/
	void RenderMouse(Point mousePos);

	/**
	Reusable function to load library directories into texture holding vectors

	@param std::vector<Clstexture*> libraryLoc location that the textures will go
	@param std::string directory location of textures I.E tilesLocExt
	@param std::string texInitString location that the textures will go I.E /art/Tiles/

	*/
	void LoadTextureLibrary(std::vector<Clstexture*> *libraryLoc, std::string directory, std::string texInitString);

	std::vector<Clstexture*> texturelist; //any misc textures go here
	std::vector<Clstexture*> tilesTextureLibrary; //world tile textures
	std::vector<Clstexture*> buildingTextureLibrary; //buildings go here
	std::vector<Clstexture*> entityTextureLibrary; //moving entities. Almost all animated textures will end up here
	std::vector<Clstexture*> UITextureLibrary; //Textures used in the creation of the UI

private:
	BYTE* screen;

	string tilesLocExt = "art\\Tiles\\*.png"; //extension type of all the images we are looking for to use as tiles
	string entLocExt = "art\\People\\*.png"; //extension type of all the images we are looking for to use as entities
	string UILocExt = "art\\Buttons\\*.png"; //extension type of all the images we are looking for to use as UI components
	string BuildingLocExt = "art\\Buildings\\*.png"; //extension type of all the images we are looking for to use as buildings
};