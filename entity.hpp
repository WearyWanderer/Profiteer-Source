#pragma once

#include <string>
#include <HAPI_lib.h>
#include "texture.hpp"
#include "ClsMapData.hpp"
#include "ClsBuilding.h"
#include "common.hpp" //common is a library of isometric functions that most classes can and should use
#include "Product.h"
#include <memory>
#include <cmath> //for trunc

enum directionInts
{
	upRight = 1,
	upLeft = 2,
	downLeft = 3,
	downRight = 4
};

class Clsentity
{
public:
	Clsentity(Clstexture* usedtexture, int xpos, int ypos);
	virtual inline ~Clsentity(){};

	void SetName(std::string);
	inline std::string GetName() { return name; }
	inline int GetXPos(){ return x; }
	inline int GetXIDPos(){ return xID; }
	Point GetPointPos(bool needsConversion);
	inline int GetWidth(){ return texture->ReturnWidth(); }
	inline int GetCenterX(){ return x + (texture->ReturnWidth() / 2); }
	inline int GetYPos(){ return y; }
	inline int GetYIDPos(){ return yID; }
	inline int GetHeight(){ return texture->ReturnHeight(); }
	inline int GetCenterY(){ return y + (texture->ReturnHeight() / 2); }
	inline void SetXPos(int xPos){ x = xPos; }
	inline void SetYPos(int yPos){ y = yPos; }
	inline Clsrectangle GetEntityRect(){ return entityRect; }
	virtual inline void SetCurrentDirection(int direction) { }
	inline int GetSpeed(){ return speed; }
	inline void SetSpeed(int speedSetter)
	{ 
		if (speed + speedSetter >= 1 && speed + speedSetter <= 3)
			speed += speedSetter;
	}

#pragma region player based functions 

	void UpdatePosition(int offsetx, int offsety, ClsMapData* mapData); // update position locally to the map data
	bool CheckMovementTile(int xpos, int ypos, ClsMapData* mapData); //check if movement is restricted along map tiles
	void CheckAdjBuildingLogic(ClsMapData* mapData, bool toggleState);
	void OpenAdjBuildingMenu(ClsMapData* mapData);
	virtual inline int GetCurrency(){ return 0; }
	virtual inline Product* GetProduct() { return nullptr; }
	virtual inline int GetMaxCapacity() { return 0; }
	virtual inline int GetCurrentCapacity(){ return 0; }
	virtual inline bool SpendMoney(int cost) { return false; };
	virtual inline void SetPrice(int price){};
	virtual inline void SetWeight(int weight){};
	virtual inline void SetType(int type){};
	virtual inline bool SetCapacity(int capacityToAdd){ return false; };
	virtual inline int GetVehicle(){ return 0; }
	virtual inline void ChangeVehicle(Clstexture* new1, Clstexture* new2, Clstexture* new3, Clstexture* new4, int capacity, int speedSetter){};

#pragma endregion

	bool IsTouchingPixel(int x, int y);
	Clstexture* GetTexture();
	Clsrectangle GetTextureRect();
	virtual inline Clstexture* GetDirectSprite() { return texture; } //return the id for directional sprite switching
	virtual inline bool CheckAnimation() { return true; } //virtual for the players in the entity list
	virtual inline void ToggleAnimated() {}
	
	int currentAnimFrame = 1;

protected:

	int speed = 1;
	std::string name;
	Clstexture* texture;
	Clsrectangle entityRect = Clsrectangle(0, 0, 0, 0);
	int x; //the offset that could be renamed to be offset1 and offset2
	int y;

	int xID; //the tile currently on's xID
	int yID; //the tile currently on's yID

};

//inherited class that uses 4 textures for directional spritesheeting, has default positions for movement sprites
class Clsplayer : public Clsentity
{
public:
	Clsplayer(Clstexture* usedtexture1, Clstexture* usedtexture2, Clstexture* usedtexture3, Clstexture* usedtexture4, int xpos, int ypos) : Clsentity(usedtexture1, xpos, ypos)
	{
		upLeftTex = usedtexture2;
		downLeftTex = usedtexture3;
		downRightTex = usedtexture4;
	};
	~Clsplayer();

	Clstexture* GetDirectSprite();
	inline void SetCurrentDirection(int direction) { currentDirection = direction; }
	inline void ToggleAnimated() { isMoving = !isMoving; }
	inline bool CheckAnimation() { return isMoving; }
	inline int GetCurrency() { return currentMoney; }
	inline Product* GetProduct() { return &playerProduct; }
	inline void SetPrice(int price){ playerProduct.SetPrice(price); };
	inline void SetWeight(int weight)
	{ 
		if (weight != 0)
			playerProduct.SetWeight(weight);
		else
			playerProduct.SetWeight(1);
	};
	inline void SetType(int type){ playerProduct.SetType(type); };
	bool SpendMoney(int cost);
	void SetMoney(int profit);
	inline int GetCurrentCapacity(){ return currentCapacity / playerProduct.GetWeight(); }
	inline int GetMaxCapacity(){ return maxCapacity; }
	inline void SetMaxCapacity(int add){ maxCapacity += add; }
	inline bool SetCapacity(int capacityToAdd)
	{ 
		if (currentCapacity + capacityToAdd <= maxCapacity && currentCapacity + capacityToAdd >= 0)
		{
			currentCapacity += capacityToAdd;
			return true;
		}
		else
			return false;
	}
	inline int GetVehicle(){ return vehicleID; }
	void ChangeVehicle(Clstexture* new1, Clstexture* new2, Clstexture* new3, Clstexture* new4, int capacity, int speedSetter);

private:
	Clstexture* upLeftTex;
	Clstexture* downLeftTex;
	Clstexture* downRightTex;
	Product playerProduct = Product();
	int currentMoney = 1000;
	int currentCapacity = 100;
	int maxCapacity = 100;
	int vehicleID = 1;

	int currentDirection = 2;
	bool isMoving = true;
};