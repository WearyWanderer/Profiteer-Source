#pragma once

#include <HAPI_lib.h>
#include "texture.hpp"
#include "GameState.h"
#include "Point.h"

#include <vector>
#include <string>

class  InputComponent //this needs to have the button list and redner after everything else, rework tonight
{
public:
	 InputComponent(GameState* stateMngrRef);
	~ InputComponent();

	void InputCycle(HAPI_TMouseData mouseMap, HAPI_TKeyboardData keyboardMap, HAPI_TControllerData controllerMap);

private:

	int clickID = 1;
	Point prevMousePos;

	void PlayerMovement();
	void MouseMovement();
	bool MouseInButton(HAPI_TMouseData mouseMap, Clsrectangle boundRect);
	bool MouseInButton(HAPI_TMouseData mouseMap, Clsrectangle boundRect, int cutoffX, int cutoffY); //overloaded version for non-full button sprites
	void CallbackEvent(int buttonCallbackID, HAPI_TMouseData mouseMap);
	GameState* gameStateMngrRef; //reference to the gamestate, to enable buttons to affect it

	bool previousLeftMouseDown = false;
	bool textEntryActive = false;
	HAPI_TKeyboardData previousActivity;

	//declaration of toggleables
	bool showFPS = true;
};