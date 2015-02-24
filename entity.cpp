#include "entity.hpp"

#define TILE_WIDTH_HALF 32
#define TILE_HEIGHT_HALF 16

#pragma region Constructor/Destructor

Clsentity::Clsentity(Clstexture* usedtexture, int xpos, int ypos)
{
	texture = usedtexture;
	xID = xpos;
	yID = ypos;

	entityRect.left = tileToScreen(xpos,ypos).x;
	entityRect.right = tileToScreen(xpos, ypos).x + usedtexture->ReturnRect().right;
	entityRect.top = tileToScreen(xpos,ypos).y;
	entityRect.bottom = tileToScreen(xpos, ypos).y + usedtexture->ReturnRect().bottom;
}

Clsplayer::~Clsplayer()
{
}

#pragma endregion

void Clsentity::SetName(std::string entityName)
{
	name = entityName;
}

void Clsentity::UpdatePosition(int offsetx, int offsety, ClsMapData* mapData) //runs the checks for movement restrictions and calculates the offset conversion
{
	x += (offsetx * speed);
	y += (offsety * speed);

	//comence the if statements to change which tile we are in

	if (y > 8 && x < -16) //this could be where we do the movement tile check to save resources(?)
	{
		if (CheckMovementTile(xID, yID + 1, mapData))
		{
			CheckAdjBuildingLogic(mapData, false);
			yID += 1; //move to the next tile in y axis
			y = -8; //be at top of this tile's offset threshold
			x = 16;
			CheckAdjBuildingLogic(mapData, true);
			return;
		}
		else //otherwise revert the changes to the offset
		{
			x -= (offsetx * speed);
			y -= (offsety * speed);
		}
	}
	else if (y > 8 && x > 16)
	{
		if (CheckMovementTile(xID + 1, yID, mapData))
		{
			CheckAdjBuildingLogic(mapData, false);
			xID += 1;
			x = -16;
			y = -8;
			CheckAdjBuildingLogic(mapData, true);
			return;
		}
		else //otherwise revert the changes to the offset
		{
			x -= (offsetx * speed);
			y -= (offsety * speed);
		}
	}
	else if (y < -8 && x < -16)
	{
		if (CheckMovementTile(xID - 1, yID, mapData))
		{
			CheckAdjBuildingLogic(mapData, false);
			xID -= 1;
			x = 16;
			y = 8;
			CheckAdjBuildingLogic(mapData, true);
			return;
		}
		else //otherwise revert the changes to the offset
		{
			x -= (offsetx * speed);
			y -= (offsety * speed);
		}
	}
	else if (y < -8 && x > 16)
	{
		if (CheckMovementTile(xID, yID - 1, mapData))
		{
			CheckAdjBuildingLogic(mapData, false);
			yID -= 1;
			y = 8;
			x = -16;
			CheckAdjBuildingLogic(mapData, true);
			return;
		}
		else //otherwise revert the changes to the offset
		{
			x -= (offsetx * speed);
			y -= (offsety * speed);
		}
	}

	if (x > 30 || x < -30 || y > 30 || y < -30)
	{
		x = 0;
		y = 0;
		return;
	}

#if _DEBUG
	HAPI->DebugText("x offset - " + std::to_string(x) + " y offset - " + std::to_string(y));
#endif
	
}

bool Clsentity::CheckMovementTile(int xpos, int ypos, ClsMapData* mapData) //check if the movement will end on a legitimate road tile UNOPTIMISED AS RUNS EVERY FRAME FOR EVERY TILE
{
	for (auto i : mapData->GetTileData())
	{
		if (xpos == i->x && ypos == i->y)
		{
			if (i->movementTile)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	return false; //if none of the results come back true, disallow and disregard the movement attempt
}

void Clsentity::CheckAdjBuildingLogic(ClsMapData* mapData, bool toggleState) //toggles the selection visuals of the local buildings adjacent to player chars
{
	for (auto i : mapData->GetBuildingData())
	{
		int xCheck = i->GetPositionID().x;
		int yCheck = i->GetPositionID().y;

		if (xCheck == xID + 1 && yCheck == yID || xCheck == xID - 1 && yCheck == yID || yCheck == yID - 1 && xCheck == xID || yCheck == yID + 1 && xCheck == xID)
		{
			i->ToggleAdjTex(toggleState);
			if (toggleState == false) //if we are leaving a square then hide the menu windows
			{
				for each (ClsButton* button in i->GetMenuWindow())
				{
					if (button->CheckIfShow()) //only toggle if actually open
						button->ToggleShow();
				}
			}
			if (i->CheckFactory() && toggleState == true)
			{
				SetCapacity(GetMaxCapacity() - GetCurrentCapacity());
			}
		}
	}

}

void Clsentity::OpenAdjBuildingMenu(ClsMapData* mapData) //toggles the selection visuals of the local buildings adjacent to player chars
{
	for (auto i : mapData->GetBuildingData())
	{
		int xCheck = i->GetPositionID().x;
		int yCheck = i->GetPositionID().y;

		if (xCheck == xID + 1 && yCheck == yID || xCheck == xID - 1 && yCheck == yID || yCheck == yID - 1 && xCheck == xID || yCheck == yID + 1 && xCheck == xID)
			for each (ClsButton* tile in i->GetMenuWindow())
			{
			tile->ToggleShow();
			}
	}

}

bool Clsentity::IsTouchingPixel(int checkX, int checkY)
{
	if (x < checkX && x + entityRect.right > checkX && y < checkY && y + entityRect.bottom > checkY){
		return true;
	}
	return false;
}

Clstexture* Clsentity::GetTexture()
{
	return texture;
}

Clsrectangle Clsentity::GetTextureRect()
{
	return texture->ReturnRect();
}

#pragma region ClsPlayer

Clstexture* Clsplayer::GetDirectSprite()
{
	switch (currentDirection)
	{
	case upRight:
		return texture;
		break;
	case upLeft:
		return upLeftTex;
		break;
	case downLeft:
		return downLeftTex;
		break;
	case downRight:
		return downRightTex;
		break;
	default:
		return texture;
		break;
	}
}

bool Clsplayer::SpendMoney(int price)
{
	if (currentMoney - price >= 0) //if the player has enough money
	{
		currentMoney -= price; //spend the money
		return true; //return true and allow the purchase conditions to be met
	}
	else
	{
		return false;
	}
}

void Clsplayer::ChangeVehicle(Clstexture* new1, Clstexture* new2, Clstexture* new3, Clstexture* new4, int capacity, int speedSetter)
{
	texture = new1;
	upLeftTex = new2;
	downLeftTex = new3;
	downRightTex = new4;
	maxCapacity = capacity;
	speed = speedSetter;
	vehicleID = 2;
}

void Clsplayer::SetMoney(int profit)
{ 
	currentMoney += profit; 
}

#pragma endregion

#pragma region IsometricPositioningFuns

Point Clsentity::GetPointPos(bool needsConversion)
{
	if (needsConversion)
	{
		int thisX = tileToScreen(xID, yID).x + x; //convert to a tile position
		thisX += TILE_WIDTH_HALF; //shift to center of tile
		thisX -= GetWidth() / 2;
		int thisY = tileToScreen(xID, yID).y + y; //convert to a tile position
		thisY += TILE_HEIGHT_HALF; //shift to center of tile
		entityRect.MoveRect(thisX,thisY, texture->spriteSizeX, texture->spriteSizeY);

		return Point(thisX,thisY); //this would be the basic tile point
	}
	else
		return Point(0, 0);
}

#pragma endregion
