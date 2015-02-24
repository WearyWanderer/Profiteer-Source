//include the HAPI librarys
#include <HAPI_lib.h>
#include <HAPI_InputCodes.h>

//include the game state files (these are for managing the background and some other stuff)
#include "GameState.h"
#include "inputs.hpp"
#include "SoundComponent.h"

//any other includes that are needed
#include <iostream>
#include <thread>
#include <chrono>
#include <assert.h>

/*Legend & Naming Conventions -

Variables are lowercase start and Uppercase for consecutive words - i.e int thisIsAnInt

Functions are wholly uppercase - i.e ThisIsAFunction()

All comments begin at the same indentation as the following line, no indentation differences enables easier checking of each comment definine the purpose of the following code

Structures have been defined with a Str prefix

Class is defined with a Cls prefix, unless stated to be a Component module

*/

int screenWidth = 960;
int screenHeight = 720;
int tileWidth = 64;
int tileHeight = 64;

void HAPI_Main()
{
	std::ifstream input("Timer_Config.txt"); //adjust this file to change the time limit
	int timer;
	input >> timer;
	#define TIME_LIMIT timer //this is the time limit for the main game, adjust as you see fit

	if (!HAPI->Initialise(&screenWidth, &screenHeight))
		return;

	//Create the empty pointer areas for the input data here
	HAPI_TKeyboardData keyboardMap;
	HAPI_TControllerData controllerMap;
	HAPI_TMouseData mouseMap;
	//Get the pointer to the screen's pixel data, for colouring the screen
	BYTE* screen(HAPI->GetScreenPointer());
	//Fill the keyboardMap with all the keys Bools
	HAPI->GetKeyboardData(&keyboardMap);
	HAPI->GetControllerData(0, &controllerMap);
	//Hide the cursor if this is set to false
	HAPI->SetShowCursor(false);
	HAPI->SetShowFPS(true, 10, screenHeight / 50);

	HAPI_TColour white(255, 255, 255);

	//set up components, visual manager etc.
	VisualComponent* visualMngr = new VisualComponent();
	WorldComponent* worldMngr = new WorldComponent();

	visualMngr->SetScreen(screen);
	worldMngr->LoadMapTiles(visualMngr, "maps/BaseLevelMap.xml"); //set up the map to load

	//Declare GameState's for the main game, should be the last component to declare before inputs
	GameState* mainGame = new GameState(2, visualMngr, worldMngr);
	InputComponent* inputMngr = new InputComponent(mainGame);
	HAPI->ChangeFont("kenpixel", 16, 700);

	int deltaTime = HAPI->GetTime(); //using milliseconds here
	int lastTime = 0;
	int animationTick = 0;
	int worldTick = 0;

	//BEGINNING THE MAIN GAME LOOP HERE----------------------------------------------------------------------------
	while (HAPI->Update())
	{
		int currentDelta = HAPI->GetTime();
		animationTick = lastTime / 100;
		worldTick = lastTime / 20;
		int worldCurrentTick = currentDelta / 20;
		int currentTick = currentDelta / 100;

		//Keep updating which keys are pressed at the start of the loop. Rather best to do it here so that we can be sure the buttons are true for the frame they are pressed
		HAPI->GetKeyboardData(&keyboardMap);
		HAPI->GetControllerData(0, &controllerMap);
		HAPI->GetMouseData(&mouseMap);
		//All your game code will be executed inside this loop

		if (animationTick != currentTick)
		{
			mainGame->AnimationCycle(); //advance the animation (put this in a loop check now that filtered out from rest of gameState loop)
		}
		if (worldTick != worldCurrentTick)
		{
			inputMngr->InputCycle(mouseMap, keyboardMap, controllerMap);
			mainGame->UpdateZone(mainGame->GetStateID(), keyboardMap, controllerMap, mouseMap, screen, currentDelta); //update the game info
		}

		if (mainGame->GetStateID() == 1)
		{
			HAPI->RenderText(screenWidth - 50, 10, white, std::to_string(TIME_LIMIT - (currentDelta / 1000) + mainGame->timeWhenGameStart));
			if (TIME_LIMIT - (currentDelta / 1000) + mainGame->timeWhenGameStart == 2)
			{
				soundMngr.PlayASound(endGameJingle);
			}
			if (TIME_LIMIT - (currentDelta / 1000) + mainGame->timeWhenGameStart == 0)
			{
				mainGame->SwitchStateComplete(endGame);
			}
		}

		visualMngr->RenderCycle(mainGame->GetStateID(), worldMngr->GetMapObject(), mainGame->entityList, mainGame->GetScreenRect(), mainGame->buttonList, mainGame->cameraOffset, mainGame->mousePos, mainGame->GetPlayerOfficesOwned(), mainGame->GetPlayerProfit());
		lastTime = currentDelta;
	}
	//End of main game loop here, release any memory
	delete mainGame;
	delete worldMngr;
	delete visualMngr;
	delete inputMngr;
}