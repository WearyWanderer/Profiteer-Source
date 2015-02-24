#include "GameState.h"
#include <memory>

#pragma region Constructor/Destructor

GameState::GameState(int state, VisualComponent* textureLib, WorldComponent* worldMngr)
{
	worldMngrRef = worldMngr;
	visualCompRef = textureLib;
	InitialiseState(state);
}

GameState::~GameState()
{
	for (unsigned int i = 0; i < entityList.size(); i++)
	{
		//some destructio of the sprite data here
		delete entityList[i];
	}
	for (unsigned int i = 0; i < buttonList.size(); i++)
	{
		//some destructio of the sprite data here
		delete buttonList[i];
	}
}

#pragma endregion

#pragma region StateSwitching

void GameState::InitialiseState(int state)
{
	switch (state)
	{
	case mainGame: //main game loop
	{
		currentState = 1;
		playerOfficesOwned = 0;
		playerProfitMade = 0; //clear both the scores in case we are restarting

		Clsplayer* player1 = new Clsplayer(visualCompRef->texturelist[3], visualCompRef->texturelist[4], visualCompRef->texturelist[5], visualCompRef->texturelist[6], 8, 26);
		player1->currentAnimFrame = 2; //set to initialise on different frame
		player1->SetPrice(creationProduct.GetPrice());
		player1->SetWeight(creationProduct.GetWeight() / 10);
		player1->SetType(creationProduct.GetType());
		entityList.push_back(player1);

		soundMngr.SwitchSong(1);
	}
		break;
		//setup for the main menu
	case menu:
	{
		currentState = 2;
		ClsButton* buttonBG = new ClsButton(visualCompRef->UITextureLibrary[0], 350, 210, true, 0); //zero means unused for callbacks, just a visual piece/backlayer
		ClsButton* exitButton = new ClsButton(visualCompRef->UITextureLibrary[3], 415, 310, true, exitGame);
		ClsButton* playButton = new ClsButton(visualCompRef->UITextureLibrary[4], 415, 220, true, productScreen);
		ClsButton* settingsButton = new ClsButton(visualCompRef->UITextureLibrary[5], 415, 265, true, goToSettings);
		ClsButton* buttonBG2 = new ClsButton(visualCompRef->UITextureLibrary[0], 350, 210, false, 0); //hide this to turn on when going to settings page
		ClsButton* backButton = new ClsButton(visualCompRef->UITextureLibrary[6], 415, 380, false, goToSettings);

		buttonList.push_back(buttonBG);
		buttonList.push_back(buttonBG2);
		buttonList.push_back(playButton);
		buttonList.push_back(exitButton);
		buttonList.push_back(backButton);
		buttonList.push_back(settingsButton);

		soundMngr.SwitchSong(2);
	}
		break;
	case productCreateScreen:
	{
		currentState = 3;
		ClsButton* buttonPlay = new ClsButton(visualCompRef->UITextureLibrary[4], 417, 495, true, playGame); //zero means unused for callbacks, just a visual piece/backlayer
		
		DraggableButton* dragPrice = new DraggableButton(visualCompRef->UITextureLibrary[8], 355, 301, 235, 0, true, draggablePrice); //this will need to become draggable button
		dragPrice->SetX(370);
		creationProduct.SetPrice(370 - 355); //set the defualt setup price
		DraggableButton* dragWeight = new DraggableButton(visualCompRef->UITextureLibrary[8], 355, 371, 235, 0, true, draggableWeight);
		dragWeight->SetX(365);
		creationProduct.SetWeight(1);
		
		ClsButton* leftButt = new ClsButton(visualCompRef->UITextureLibrary[9], 410, 440, true, itemLeft); // this is click to move left
		ClsButton* rightButt = new ClsButton(visualCompRef->UITextureLibrary[10], 535, 440, true, itemRight); //this is click to move right
		VisFeedbackButton* productVis = new VisFeedbackButton(visualCompRef->UITextureLibrary[11], visualCompRef->UITextureLibrary[12], visualCompRef->UITextureLibrary[13], 447, 420, true);
		
		buttonList.push_back(buttonPlay);
		buttonList.push_back(dragPrice);
		buttonList.push_back(dragWeight);
		buttonList.push_back(leftButt);
		buttonList.push_back(rightButt);
		buttonList.push_back(productVis);

		creationProduct.SetType(1);
	}
		break;
	case endGame:
	{
		currentState = 4;
		ClsButton* exitButton = new ClsButton(visualCompRef->UITextureLibrary[3], 415, 465, true, exitGame);
		buttonList.push_back(exitButton);
		cameraOffset.Set(0, 0);
		soundMngr.SwitchSong(1);
	}
		break;
		//error catching state
	default:
		break;
	}

}

void GameState::CleanState()
{
	for (unsigned int i = 0; i < entityList.size(); i++)
	{
		//some destructio of the sprite data here
		delete entityList[i];
	}
	for (unsigned int i = 0; i < buttonList.size(); i++)
	{
		//some destructio of the sprite data here
		delete buttonList[i];
	}

	entityList.clear();
	buttonList.clear();
}

void GameState::SwitchStateComplete(int state)
{
	CleanState();
	InitialiseState(state);
}

#pragma endregion

void GameState::UpdateZone(int zone, HAPI_TKeyboardData keyboardMap, HAPI_TControllerData controllerMap, HAPI_TMouseData mouseMap, BYTE* screen, int timer)
{
	if (mouseMap.x < screenRect.GetWidth() - 16 && mouseMap.y < screenRect.GetHeight() - 16)
	{
		mousePos.x = mouseMap.x;
		mousePos.y = mouseMap.y;
	}

	switch (zone)
	{
		//main game
		case mainGame:
		{
			int moveX = 0;
			int moveY = 0;
			if (keyboardMap.scanCode['W'] || controllerMap.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] > HK_GAMEPAD_LEFT_THUMB_DEADZONE)
			{
				moveY -= 1;
				moveX -= 2;
				entityList[0]->SetCurrentDirection(upLeft);
				if (!entityList[0]->CheckAnimation())
					entityList[0]->ToggleAnimated();
			}
			else if (keyboardMap.scanCode['D'] || controllerMap.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] > HK_GAMEPAD_LEFT_THUMB_DEADZONE)
			{
				moveX += 2;
				moveY -= 1;
				entityList[0]->SetCurrentDirection(upRight);
				if (!entityList[0]->CheckAnimation())
					entityList[0]->ToggleAnimated();
			}
			else if (keyboardMap.scanCode['S'] || controllerMap.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] < -HK_GAMEPAD_LEFT_THUMB_DEADZONE)
			{
				moveY += 1;
				moveX += 2;
				entityList[0]->SetCurrentDirection(downRight);
				if (!entityList[0]->CheckAnimation())
					entityList[0]->ToggleAnimated();
			}
			else if (keyboardMap.scanCode['A'] || controllerMap.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] < -HK_GAMEPAD_LEFT_THUMB_DEADZONE)
			{
				moveX -= 2;
				moveY += 1;
				entityList[0]->SetCurrentDirection(downLeft);
				if (!entityList[0]->CheckAnimation())
					entityList[0]->ToggleAnimated();
			}
			else
			{
				if (entityList[0]->CheckAnimation())
					entityList[0]->ToggleAnimated();
			}

			if (moveX != 0 || moveY != 0)
			{
				entityList[0]->UpdatePosition(moveX, moveY, worldMngrRef->GetMapObject()); //move the player based on filtered inputs
				#if _DEBUG
					HAPI->DebugText("xID - " + std::to_string(entityList[0]->GetXIDPos()) + " yID - " + std::to_string(entityList[0]->GetYIDPos()));
				#endif
				break;
			}

		}
			break;
		case productCreateScreen:
		{
			timeWhenGameStart = timer / 1000;
		}
			break;
		default:
		{

		}
			break;
	}



}

Clsrectangle GameState::GetScreenRect() const
{
	return screenRect;
}

void GameState::AnimationCycle()
{
	for (unsigned int i = 0; i < entityList.size(); i++)
	{
		if (entityList[i] != nullptr) //check if empty
		{
			if (entityList[i]->CheckAnimation())
			{
			if (entityList[i]->currentAnimFrame < entityList[i]->GetTexture()->ReturnFrameCount())
				entityList[i]->currentAnimFrame += 1;
			else
				entityList[i]->currentAnimFrame = 1;
			}
		}
	}
}
