#pragma once

#include <HAPI_lib.h>
#include "texture.hpp"
#include "common.hpp"
#include "entity.hpp"
#include "Point.h"
#include "Product.h"
#include "VisualComponent.hpp"
#include "WorldComponent.hpp"
#include "SoundComponent.h"

#include <string>
#include <vector>
#include <memory>
#include <iostream>

#define soundMngr SoundComponent::getInstance()

class GameState
{
public:
	GameState(int state, VisualComponent* textureLib, WorldComponent* worldMngr);
	~GameState();

	std::vector<Clsplayer*> entityList;
	std::vector<ClsButton*> buttonList;

	/**
	Initialising function for a new state.
	@param int - state to init
	*/
	void InitialiseState(int state);
	/**
	essentially has everything a destructor should have, for cleaning before fully switching to a new state
	*/
	void CleanState();
	/**
	Switches the current state of this gameState instance. Complete version rather than pause, as this is for new game and exit current game functionality
	@param int - state to switch to
	*/
	void SwitchStateComplete(int state);
	/**
	Function update to be ran in the HAPI update loop
	*/
	void UpdateZone(int zone, HAPI_TKeyboardData keyboardMap, HAPI_TControllerData controllerMap, HAPI_TMouseData mouseMap, BYTE* screen, int timer);

	inline void IncreasePlayerProfit(int profitTally){ playerProfitMade += profitTally; };
	inline void IncreasePlayerOfficesOwned(){ playerOfficesOwned += 1; };
	inline int GetPlayerProfit(){ return playerProfitMade; };
	inline int GetPlayerOfficesOwned(){ return playerOfficesOwned; };

	inline WorldComponent* GetWorld(){ return worldMngrRef; }
	inline VisualComponent* GetLibrary(){ return visualCompRef; }

	Clsrectangle GetScreenRect() const;
	void AnimationCycle();
	inline int GetStateID(){ return currentState; }
	inline Product GetCreationProduct(){ return creationProduct; }

	Point cameraOffset;
	Point mousePos;
	Product creationProduct;
	std::string nameString;

	int timeWhenGameStart;

private:

	int currentState;
	int playerOfficesOwned;
	int playerProfitMade;
	const Clsrectangle screenRect = Clsrectangle(0, screenWidth, 0, screenHeight);
	
	WorldComponent* worldMngrRef;
	VisualComponent* visualCompRef;
};

