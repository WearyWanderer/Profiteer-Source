#include "inputs.hpp"

#pragma region Constructor/Destructor

InputComponent::InputComponent(GameState* stateMngrRef)
{
	gameStateMngrRef = stateMngrRef;
}

InputComponent::~InputComponent()
{

}

#pragma endregion

void InputComponent::InputCycle(HAPI_TMouseData mouseMap, HAPI_TKeyboardData keyboardMap, HAPI_TControllerData controllerMap)
{

#pragma region OpenBuildingMenu

	if (keyboardMap.scanCode['E'])
	{
		if (gameStateMngrRef->GetStateID() == 1)
			if (previousActivity.scanCode['E'] != true)
				gameStateMngrRef->entityList[0]->OpenAdjBuildingMenu(gameStateMngrRef->GetWorld()->GetMapObject());
	}

#pragma endregion

#pragma region DraggableCamera

	if (mouseMap.rightButtonDown && (gameStateMngrRef->GetStateID() == mainGame || gameStateMngrRef->GetStateID() == aiTest)) //if the right mouse button is down and we're trying to get the map to move
	{
		if (prevMousePos.Compare(0, 0) == false) //if the right mouse button was held down last frame
		{
			gameStateMngrRef->cameraOffset.SetIncremental(prevMousePos.x - mouseMap.x, prevMousePos.y - mouseMap.y);
		}
		prevMousePos.Set(mouseMap.x, mouseMap.y); //capture shot of the current frame
	}
	else
	{
		prevMousePos.Set(0, 0); //reset so that the attem
	}

#pragma endregion

#pragma region ClickDownEvent & Buy/Sell System

	if (mouseMap.leftButtonDown)
	{
		if (!previousLeftMouseDown) //if this is a button down event
		{
			for (unsigned int i = 0; i < gameStateMngrRef->buttonList.size(); i++) //for each button registered
			{
				if (gameStateMngrRef->buttonList[i]->CheckIfShow()) //if the button is visible and on-screen
				{
					Clsrectangle offsetRect(gameStateMngrRef->buttonList[i]->GetRect());
					offsetRect.TranslateToRect(-gameStateMngrRef->cameraOffset.x, -gameStateMngrRef->cameraOffset.y); //create a temp rect that is translated to the camera offset
					if (MouseInButton(mouseMap, offsetRect)) //if the mouse is withing the button and has been clicked
					{
						if (gameStateMngrRef->buttonList[i]->callbackID == draggablePrice || gameStateMngrRef->buttonList[i]->callbackID == draggableWeight)
						{
							CallbackEvent(gameStateMngrRef->buttonList[i]->callbackID, mouseMap);
							soundMngr.PlayASound(click);
						}
						else
						{
							CallbackEvent(gameStateMngrRef->buttonList[i]->callbackID, mouseMap);
							soundMngr.PlayASound(click);
							previousLeftMouseDown = true; //reset for next check
						}
					}

				}
			}
			for each (ClsBuilding* building in gameStateMngrRef->GetWorld()->GetMapObject()->buildingList)
			{
				for each (ClsButton* button in building->GetMenuWindow())
				{
					if (button->CheckIfShow())
					{
						if (button->callbackID == closeWindow)
						{
							Clsrectangle offsetRect(button->GetRect());
							offsetRect.TranslateToRect(-gameStateMngrRef->cameraOffset.x, -gameStateMngrRef->cameraOffset.y); //create a temp rect that is translated to the camera offset
							if (MouseInButton(mouseMap, offsetRect))
							{
								for each (ClsButton* thisButton in building->GetMenuWindow())
								{
									thisButton->ToggleShow();
									soundMngr.PlayASound(click);
								}
							}
						}
						else //some button clicks that directly impact the player or buidlings cannot be held in the generic button class, and must be invoked from within input instead to remain engine-neutral and not game-specific
						{
							Clsrectangle offsetRect(button->GetRect());
							offsetRect.TranslateToRect(-gameStateMngrRef->cameraOffset.x, -gameStateMngrRef->cameraOffset.y); //create a temp rect that is translated to the camera offset
							if (MouseInButton(mouseMap, offsetRect, button->cutoffX, button->cutoffY))
							{
								switch (button->callbackID)
								{
								case sellProduct:
								{
									previousLeftMouseDown = true; //reset for next check
									if (gameStateMngrRef->entityList[0]->SetCapacity(-gameStateMngrRef->entityList[0]->GetProduct()->GetWeight()))
									{
										gameStateMngrRef->entityList[0]->SetMoney(gameStateMngrRef->entityList[0]->GetProduct()->GetPrice());
										gameStateMngrRef->IncreasePlayerProfit(gameStateMngrRef->entityList[0]->GetProduct()->GetPrice());
										soundMngr.PlayASound(sell);
									}
								}
									break;
								case sellProd2:
								{
									previousLeftMouseDown = true; //reset for next check
									if (gameStateMngrRef->entityList[0]->SetCapacity(-(gameStateMngrRef->entityList[0]->GetProduct()->GetWeight()) * 2))
									{
										gameStateMngrRef->entityList[0]->SetMoney(gameStateMngrRef->entityList[0]->GetProduct()->GetPrice(2));
										gameStateMngrRef->IncreasePlayerProfit(gameStateMngrRef->entityList[0]->GetProduct()->GetPrice(2));
										soundMngr.PlayASound(sell);
									}
								}
									break;
								case sellProd5:
								{
									previousLeftMouseDown = true; //reset for next check
									if (gameStateMngrRef->entityList[0]->SetCapacity(-(gameStateMngrRef->entityList[0]->GetProduct()->GetWeight()) * 5))
									{
										gameStateMngrRef->entityList[0]->SetMoney(gameStateMngrRef->entityList[0]->GetProduct()->GetPrice(5));
										gameStateMngrRef->IncreasePlayerProfit(gameStateMngrRef->entityList[0]->GetProduct()->GetPrice(5));
										soundMngr.PlayASound(sell);
									}
								}
									break;
								case buyBuilding:
								{
									if (gameStateMngrRef->entityList[0]->SpendMoney(building->GetPrice()))
									{
										building->ChangeOwnership(1); //this sets the ownership to the player
										gameStateMngrRef->IncreasePlayerOfficesOwned(); //add to the player score
										for each (ClsButton* thisButton in building->GetMenuWindow())
										{
											thisButton->ToggleShow();
											soundMngr.PlayASound(buy);
										}
									}
								}
									break;
								case upgradeCapacity:
								{
									previousLeftMouseDown = true; //reset for next check
									if (gameStateMngrRef->entityList[0]->SpendMoney(350))
									{
										gameStateMngrRef->entityList[0]->SetMaxCapacity(10); //increase capacity by 10
										soundMngr.PlayASound(upgrade);
									}
								}
									break;
								case upgradeSpeed:
								{
									previousLeftMouseDown = true; //reset for next check
									if (gameStateMngrRef->entityList[0]->GetVehicle() != 2 && gameStateMngrRef->entityList[0]->GetSpeed() <= 3 && gameStateMngrRef->entityList[0]->SpendMoney(550))
									{
										gameStateMngrRef->entityList[0]->SetSpeed(1); //increase speed by 1
										soundMngr.PlayASound(upgrade);
									}
								}
									break;
								case switchToVan:
								{
									previousLeftMouseDown = true; //reset for next check
									if (gameStateMngrRef->entityList[0]->GetVehicle() != 2 && gameStateMngrRef->entityList[0]->SpendMoney(1250))
									{
										gameStateMngrRef->entityList[0]->ChangeVehicle(gameStateMngrRef->GetLibrary()->texturelist[7], gameStateMngrRef->GetLibrary()->texturelist[8], gameStateMngrRef->GetLibrary()->texturelist[9], gameStateMngrRef->GetLibrary()->texturelist[10], 200, 1);
										soundMngr.PlayASound(upgrade);
									}
								}
									break;
								default:
									break;
								}
							}
						}

					}
				}
			}
		}
	}
	else
	{
		previousLeftMouseDown = false; //button is not being held
	}
#pragma endregion

#pragma region ToggleFPSShow

	if (keyboardMap.scanCode[0x70])
	{
		if (previousActivity.scanCode[0x70] != true)
		{
			HAPI->SetShowFPS(!showFPS);
			showFPS = !showFPS;
		}
	}

#pragma endregion

#pragma region EscClose

	if (keyboardMap.scanCode[0x1B])
	{
		HAPI->Close();
	}

#pragma endregion

	previousActivity = keyboardMap;
}

void InputComponent::PlayerMovement()
{

}

void InputComponent::MouseMovement()
{

}

bool InputComponent::MouseInButton(HAPI_TMouseData mouseMap, Clsrectangle boundRect)
{
	if (mouseMap.x < boundRect.right && mouseMap.x > boundRect.left && mouseMap.y < boundRect.bottom && mouseMap.y > boundRect.top)
	{
		return true;
	}
	return false;
}

bool InputComponent::MouseInButton(HAPI_TMouseData mouseMap, Clsrectangle boundRect, int cutoffX, int cutoffY)
{
	if (mouseMap.x < boundRect.right - cutoffX && mouseMap.x > boundRect.left && mouseMap.y < boundRect.bottom - cutoffY && mouseMap.y > boundRect.top)
	{
		return true;
	}
	return false;
}

void InputComponent::CallbackEvent(int buttonCallbackID, HAPI_TMouseData mouseMap)
{
	switch (buttonCallbackID)
	{
		case playGame:
		{
			gameStateMngrRef->creationProduct.SetType(gameStateMngrRef->buttonList[5]->GetVisID());
			gameStateMngrRef->creationProduct.SetPrice(gameStateMngrRef->buttonList[1]->DistanceX());
			gameStateMngrRef->creationProduct.SetWeight(gameStateMngrRef->buttonList[2]->DistanceX());
			gameStateMngrRef->SwitchStateComplete(mainGame);
		}
			break;
		case exitGame:
		{
			HAPI->Close();
		}
			break;
		case goToAI:
		{
			gameStateMngrRef->SwitchStateComplete(aiTest);
		}
			break;
		case productScreen:
		{
			gameStateMngrRef->SwitchStateComplete(productCreateScreen);
		}
		case unPauseGame:
		{

		}
			break;
		case itemLeft:
		{
			gameStateMngrRef->buttonList[5]->ChangeVis(-1, gameStateMngrRef->buttonList[1], gameStateMngrRef->buttonList[2]);
		}
			break;
		case itemRight:
		{
			gameStateMngrRef->buttonList[5]->ChangeVis(1, gameStateMngrRef->buttonList[1], gameStateMngrRef->buttonList[2]);
		}
			break;
		case draggablePrice:
		{
			gameStateMngrRef->buttonList[1]->SetX(mouseMap.x);
		}
			break;
		case draggableWeight:
		{
			gameStateMngrRef->buttonList[2]->SetX(mouseMap.x);
		}
			break;
		case buyBuilding:
		{

		}
			break;
	default:
		break;
	}

}