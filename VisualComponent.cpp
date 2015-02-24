#include "VisualComponent.hpp"

#pragma region Constructor/Destructor

VisualComponent::VisualComponent() //set up function for the visual component (this is where all textures are loaded for the game)
{
	//load the textures and put them into the vector ready for looping through all
	Clstexture* character = new Clstexture();
	Clstexture* playerCar1 = new Clstexture(); //up right
	Clstexture* playerCar2 = new Clstexture(); //up left
	Clstexture* playerCar3 = new Clstexture(); //down left
	Clstexture* playerCar4 = new Clstexture(); //down right
	Clstexture* playerVan1 = new Clstexture();
	Clstexture* playerVan2 = new Clstexture();
	Clstexture* playerVan3 = new Clstexture();
	Clstexture* playerVan4 = new Clstexture();
	Clstexture* menubg = new Clstexture();
	Clstexture* productbg = new Clstexture();
	Clstexture* endGame = new Clstexture();

	character->InitTexture("art/People/Person1LeftDown.png", 16, 28, 8);
	texturelist.push_back(character); //needs moving to entity library

	menubg->InitTexture("art/Backgrounds/MainBack.png");
	texturelist.push_back(menubg);

	productbg->InitTexture("art/Backgrounds/ProductCreateBack.png");
	texturelist.push_back(productbg);

	playerCar1->InitTexture("art/Player/Red1/UpRight.png", 32, 32, 2); //total 12 sprites
	texturelist.push_back(playerCar1); //needs moving to entity library
	playerCar2->InitTexture("art/Player/Red1/UpLeft.png", 32, 32, 2);
	texturelist.push_back(playerCar2); //needs moving to entity library
	playerCar3->InitTexture("art/Player/Red1/DownLeft.png", 32, 32, 2);
	texturelist.push_back(playerCar3); //needs moving to entity library
	playerCar4->InitTexture("art/Player/Red1/DownRight.png", 32, 32, 2);
	texturelist.push_back(playerCar4); //needs moving to entity library
	playerVan1->InitTexture("art/Player/UpRightVan.png", 32, 32, 2); //total 12 sprites
	texturelist.push_back(playerVan1); //needs moving to entity library
	playerVan2->InitTexture("art/Player/UpLeftVan.png", 32, 32, 2);
	texturelist.push_back(playerVan2); //needs moving to entity library
	playerVan3->InitTexture("art/Player/DownLeftVan.png", 32, 32, 2);
	texturelist.push_back(playerVan3); //needs moving to entity library
	playerVan4->InitTexture("art/Player/DownRightVan.png", 32, 32, 2);
	texturelist.push_back(playerVan4); //needs moving to entity library

	endGame->InitTexture("art/Backgrounds/EndGame.png");
	texturelist.push_back(endGame);

	//auto load the tiles, buttons and other directories here
	LoadTextureLibrary(&tilesTextureLibrary, tilesLocExt, "art/Tiles/");
	LoadTextureLibrary(&UITextureLibrary, UILocExt, "art/Buttons/");
	LoadTextureLibrary(&buildingTextureLibrary, BuildingLocExt, "art/Buildings/");

}

VisualComponent::~VisualComponent()
{
	for (unsigned int i = 0; i < texturelist.size(); i++)
	{
		//some destructio of the sprite data here
		delete texturelist[i];
	}
	for (unsigned int i = 0; i < tilesTextureLibrary.size(); i++)
	{
		//some destructio of the tile sprite data here
		delete tilesTextureLibrary[i];
	}
	for (unsigned int i = 0; i < buildingTextureLibrary.size(); i++)
	{
		//some destructio of the tile sprite data here
		delete buildingTextureLibrary[i];
	}
	for (unsigned int i = 0; i < entityTextureLibrary.size(); i++)
	{
		//some destructio of the tile sprite data here
		delete entityTextureLibrary[i];
	}
	for (unsigned int i = 0; i < UITextureLibrary.size(); i++)
	{
		//some destructio of the tile sprite data here
		delete UITextureLibrary[i];
	}

}

#pragma endregion

void VisualComponent::RenderCycle(int state, ClsMapData* mapRef, std::vector<Clsplayer*> entityList, Clsrectangle screenRect, std::vector<ClsButton*> buttonList, Point cameraOffset, Point mousePos, int officesScore, int profitScore)
{
	UpdateScreen(state);
	switch (state) //render based on gameState
	{
	case mainGame:
	{
		RenderMap(mapRef->GetTileData(), screenRect, cameraOffset);
		RenderEntities(entityList, screenRect, cameraOffset);
		RenderBuildings(mapRef->GetBuildingData(), screenRect, cameraOffset);
		DrawTextureAlpha(UITextureLibrary[7], screen, 680, 580, 1);
		RenderUI(buttonList, entityList, mapRef->GetBuildingData(), screenRect, cameraOffset, state);
		RenderMouse(mousePos);
	}
		break;
	case menu:
	{
		RenderEntities(entityList, screenRect, cameraOffset);
		RenderUI(buttonList, entityList, mapRef->GetBuildingData(), screenRect, cameraOffset, state);
		RenderMouse(mousePos);
	}
		break;
	case productCreateScreen:
	{
		RenderUI(buttonList, entityList, mapRef->GetBuildingData(), screenRect, cameraOffset, state);
		RenderMouse(mousePos);
	}
		break;
	case endGame:
	{
		HAPI->RenderText(470, 295, HAPI_TColour(0, 0, 0), std::to_string(officesScore));
		HAPI->RenderText(456, 340, HAPI_TColour(0, 0, 0), std::to_string(profitScore));
		HAPI->RenderText(452, 410, HAPI_TColour(0, 0, 0), std::to_string(profitScore * officesScore));
		RenderUI(buttonList, entityList, mapRef->GetBuildingData(), screenRect, cameraOffset, state);
		RenderMouse(mousePos);
	}
		break;
	}
} //could just pass gamestate and use get functions to create references inside here

#pragma region StateRenderFunctions

void VisualComponent::UpdateScreen(int state)
{
	switch (state)
	{
	case mainGame:
	{
		ClearBackground(HAPI_TColour(24,56,32), screen);
	}
		break;
	case menu:
	{
		ClipTextureDraw(texturelist[1], screen, Clsrectangle(0, 960, 0, 720), texturelist[1]->ReturnRect(), 0, 0, 1);
	}
		break;
	case productCreateScreen:
	{
		ClipTextureDraw(texturelist[2], screen, Clsrectangle(0, 960, 0, 720), texturelist[2]->ReturnRect(), 0, 0, 1);
	}
		break;
	case endGame:
	{
		ClipTextureDraw(texturelist[11], screen, Clsrectangle(0, 960, 0, 720), texturelist[11]->ReturnRect(), 0, 0, 1);
	}
		break;
	}
}

void VisualComponent::RenderEntities(std::vector<Clsplayer*> entityList, Clsrectangle screenRect, Point cameraOffset) //function that loops through any entities in the game's list and will render them(currentley assumes alpha, need to add a check function)
{
	for (unsigned int i = 0; i < entityList.size(); i++)
	{
		if (entityList[i] != nullptr) //check if empty
		{
			int x = entityList[i]->GetPointPos(true).x - cameraOffset.x;
			int y = entityList[i]->GetPointPos(true).y - cameraOffset.y;
			Clsrectangle tempRect = entityList[i]->GetEntityRect();
			tempRect.TranslateToRect(-cameraOffset.x, -cameraOffset.y);
			if (!tempRect.CheckCompletelyOutside(screenRect))
			{
				if (tempRect.CheckCompletelyInside(screenRect))
				{
					DrawTextureAlpha((entityList[i]->GetDirectSprite()), screen, x, y, entityList[i]->currentAnimFrame);
				}
				else
				{
					ClipTextureDrawAlpha((entityList[i]->GetDirectSprite()), screen, screenRect, tempRect, x, y, entityList[i]->currentAnimFrame);
				}
			}
		}
	}
}

void VisualComponent::RenderMap(std::vector<Tile*> tileList, Clsrectangle screenRect, Point cameraOffset) //this could be optimised further
{
	for (unsigned int i = 0; i < tileList.size(); i++)
	{
		if (tileList[i]->usedTexture != nullptr) //check if empty
		{
			Point tempPoint = tileToScreen(tileList[i]->x, tileList[i]->y);
			tempPoint.x -= cameraOffset.x;
			tempPoint.y -= cameraOffset.y;

			if (tileList[i]->requiresRedraw)
			{
				if (!tileList[i]->GetRect().CheckCompletelyOutside(screenRect, tempPoint.x, tempPoint.y))
				{
					if (tileList[i]->GetRect().CheckCompletelyInside(screenRect, tempPoint.x, tempPoint.y))
					{
						DrawTextureAlpha((tileList[i]->usedTexture), screen, tempPoint.x, tempPoint.y, 1); //all tiles are same size so this is valid method of configuring to right distance (for now)
						//tileList[i]->requiresRedraw = false;
						//HAPI->RenderText(isoX + 50, isoY + 20, HAPI_TColour(255, 255, 255), std::to_string(tileList[i]->x) + " & " + std::to_string(tileList[i]->y)); // -REMOVE
					}
					else {
						ClipTextureDrawAlpha((tileList[i]->usedTexture), screen, screenRect, tileList[i]->GetRect(), tempPoint.x, tempPoint.y, 1, true); //all tiles are same size so this is valid method of configuring to right distance (for now)
						//tileList[i]->requiresRedraw = false;
					}
				}
			}
		}
	}
}

void VisualComponent::RenderBuildings(std::vector<ClsBuilding*> buildingList, Clsrectangle screenRect, Point cameraOffset) //buildings need a slight offset to draw on tiles properly
{
	for (unsigned int i = 0; i < buildingList.size(); i++)
	{
		if (buildingList[i]->GetTexture() != nullptr) //check if empty
		{
			Point tempPoint = tileToScreen(buildingList[i]->GetPositionID().x, buildingList[i]->GetPositionID().y);
			if (buildingList[i]->GetTexture()->ReturnHeight() == 64 && !buildingList[i]->CheckFactory()) //if it is a 64x64 sprite, do these offstes
			{
				tempPoint.x += 8;
				tempPoint.y -= 20;
			}
			else if (buildingList[i]->CheckFactory()) //if the last building and thus the spawn
			{
				tempPoint.x -= 20;
				tempPoint.y -= 35;
			}
			else //otherwise use the taller tile offsets
			{
				tempPoint.x += 8;
				tempPoint.y -= 85;
			}
			tempPoint.x -= cameraOffset.x;
			tempPoint.y -= cameraOffset.y;

			if (buildingList[i]->requiresRedraw)
			{
				if (!buildingList[i]->GetRect().CheckCompletelyOutside(screenRect, tempPoint.x, tempPoint.y))
				{
					if (buildingList[i]->GetRect().CheckCompletelyInside(screenRect, tempPoint.x, tempPoint.y))
					{
						DrawTextureAlpha((buildingList[i]->GetTexture()), screen, tempPoint.x, tempPoint.y, 1); //all tiles are same size so this is valid method of configuring to right distance (for now)
						//tileList[i]->requiresRedraw = false;
						//HAPI->RenderText(isoX + 50, isoY + 20, HAPI_TColour(255, 255, 255), std::to_string(tileList[i]->x) + " & " + std::to_string(tileList[i]->y)); // -REMOVE
					}
					else 
					{
						ClipTextureDrawAlpha((buildingList[i]->GetTexture()), screen, screenRect, buildingList[i]->GetRect(), tempPoint.x, tempPoint.y, 1, true); //all tiles are same size so this is valid method of configuring to right distance (for now)
						//tileList[i]->requiresRedraw = false;
					}
				}
			}
		}
	}

}

void VisualComponent::RenderUI(std::vector<ClsButton*> buttonList, std::vector<Clsplayer*> entityList, std::vector<ClsBuilding*> buildingData, Clsrectangle screenRect, Point cameraOffset, int state) // eventually maybe pass in just the UI component here
{
	switch (state)
	{
	case mainGame:
	{
		for each (ClsBuilding* building in buildingData) //render all the showable buildings interactive menus
		{
			if (building->GetOwnerShip() != 1)
			{
				for each (ClsButton* i in building->GetMenuWindow()) //if it is not an office, show usual window
				{
					if (i->CheckIfShow())
					{
						int x = i->GetX() - cameraOffset.x;
						int y = i->GetY() - cameraOffset.y;
						Clsrectangle tempRect = i->GetRect();
						tempRect.TranslateToRect(-cameraOffset.x, -cameraOffset.y);
						ClipTextureDrawAlpha(i->GetTexture(), screen, screenRect, tempRect, x, y, 1);
					}
				}
				if (building->GetOwnerShip() == 0) //if it is an unowned office, show price
				{
					if (building->GetMenuWindow()[0]->CheckIfShow()) // if the menu is showing
					{
						HAPI->ChangeFont("kenpixel", 19, 600);
						HAPI->RenderText(building->GetMenuWindow()[0]->GetX() - cameraOffset.x + 20, building->GetMenuWindow()[0]->GetY() - cameraOffset.y + 16, HAPI_TColour(0, 0, 0), "Office Price: " + std::to_string(building->GetPrice()));
						HAPI->ChangeFont("kenpixel", 16, 700);
					}
				}
			}
		}
		for (auto i : buttonList) //render all the showing buttons
		{
			if (i->CheckIfShow())
			{
				DrawTextureAlpha(i->GetTexture(), screen, i->GetX(), i->GetY(), 1);
			}
		}
		HAPI->RenderText(685, 600, HAPI_TColour(), "Current Profit: " + std::to_string(entityList[0]->GetCurrency()));
		HAPI->RenderText(685, 630, HAPI_TColour(), "Product Price: " + std::to_string(entityList[0]->GetProduct()->GetPrice()));
		HAPI->RenderText(685, 660, HAPI_TColour(), "Product Weight: " + std::to_string(entityList[0]->GetProduct()->GetWeight()));
		HAPI->RenderText(685, 690, HAPI_TColour(), "Quantity in Car: " + std::to_string(entityList[0]->GetCurrentCapacity()));
	}
		break;
	case menu:
	{
		for (auto i : buttonList)
		{
			if (i->CheckIfShow())
			{
				DrawTextureAlpha(i->GetTexture(), screen, i->GetX(), i->GetY(), 1);
			}
		}
	}
		break;
	case productCreateScreen:
	{
		for (auto i : buttonList)
		{
			if (i->CheckIfShow())
			{
				DrawTextureAlpha(i->GetTexture(), screen, i->GetX(), i->GetY(), 1);
			}
		}

		HAPI->RenderText(480, 290, HAPI_TColour(0, 0, 0), std::to_string(buttonList[1]->DistanceX()));
		HAPI->RenderText(435, 275, HAPI_TColour(0, 0, 0), "Price Per Unit");
		HAPI->RenderText(480, 355, HAPI_TColour(0, 0, 0), std::to_string(buttonList[2]->DistanceX() / 10));
		HAPI->RenderText(425, 343, HAPI_TColour(0, 0, 0), "Weight Per Unit");

		switch (buttonList[5]->GetVisID())
		{
		case 1:
		{
			HAPI->ChangeFont("kenpixel", 26, 700);
			HAPI->RenderText(420, 230, HAPI_TColour(0, 0, 0), "Mega-Veg");
			HAPI->ChangeFont("kenpixel", 16, 700);
		}
			break;
		case 2:
		{
			HAPI->ChangeFont("kenpixel", 26, 700);
			HAPI->RenderText(400, 230, HAPI_TColour(0, 0, 0), "Giga-Toolbox");
			HAPI->ChangeFont("kenpixel", 16, 700);
		}
			break;
		case 3:
		{
			HAPI->ChangeFont("kenpixel", 21, 700);
			HAPI->RenderText(355, 230, HAPI_TColour(0, 0, 0), "Nvidia KeithForce Titan");
			HAPI->ChangeFont("kenpixel", 16, 700);
		}
			break;
		default:
			break;
		}
	}
		break;
	case endGame:
	{
		for (auto i : buttonList) //render all the showing buttons
		{
			if (i->CheckIfShow())
			{
				DrawTextureAlpha(i->GetTexture(), screen, i->GetX(), i->GetY(), 1);
			}
		}
	}
		break;
	default:
		break;
	}

}

void VisualComponent::RenderMouse(Point mousePos)
{
	DrawTextureAlpha(UITextureLibrary[2], screen, mousePos.x, mousePos.y, 1); //draw mouse
}

#pragma endregion

#pragma region LibraryLoadingFunctions

void VisualComponent::LoadTextureLibrary(std::vector<Clstexture*> *libraryLoc, std::string directory, std::string texInitString)
{
	//NOTE- now added seperate vectors, next step is look at efficiency (would maps be better?)	
	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile((LPCSTR)directory.c_str(), &findFileData);
	HAPI->DebugText("Texture files in dir:" + directory);
	HAPI->DebugText((std::string)findFileData.cFileName);

	Clstexture* firstTile = new Clstexture();
	firstTile->InitTexture(texInitString + (std::string)findFileData.cFileName);
	libraryLoc->push_back(firstTile);

	while (FindNextFile(hFind, &findFileData))
	{
		Clstexture* nextTile = new Clstexture();
		nextTile->InitTexture(texInitString + (std::string)findFileData.cFileName);
		libraryLoc->push_back(nextTile);

		HAPI->DebugText((std::string)findFileData.cFileName);
	}
}

#pragma endregion
