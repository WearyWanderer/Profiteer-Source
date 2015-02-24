#pragma once

#include "texture.hpp"
#include "Point.h"
#include "ClsButton.h"
#include "common.hpp"
#include <vector>

class ClsBuilding
{
public:
	ClsBuilding(Clstexture* texture, Clstexture* adjTexture, std::vector<Clstexture*> UITexLib, int xpos, int ypos);
	virtual ~ClsBuilding();

	virtual inline Clstexture* GetTexture() 
	{ 
		if (!isAdj)
			return textureRef;
		else
			return adjTex;
	}
	inline Point GetPositionID() { return Point(xID, yID); }
	inline void ToggleAdjTex(bool toggleState) { isAdj = toggleState; } //manual rather than = !isAdj due to need for custom inputs at a later stage
	inline Clsrectangle GetRect() { return buildingRect; }
	inline std::vector<ClsButton*> GetMenuWindow(){ return buttonWindow; }
	void CleanupFunc();
	inline bool CheckFactory(){ return isFactory; }

#pragma region OfficeVirtuals
	virtual inline int GetPrice() { return 0; }
	virtual inline void ChangeOwnership(int owner) {};
	virtual inline int GetOwnerShip() { return 10; }
#pragma endregion

	bool requiresRedraw = true;
	

protected:
	Clstexture* textureRef;
	Clstexture* adjTex;

	std::vector<ClsButton*> buttonWindow;
	
	int xID, yID;
	bool isAdj = false;
	bool isFactory = false;
	Clsrectangle buildingRect = Clsrectangle(0, 0, 0, 0);
	
};

class ClsShop : public ClsBuilding
{
public:
	ClsShop(Clstexture* texture, Clstexture* adjTex, std::vector<Clstexture*> UITexLib , int xpos, int ypos) : ClsBuilding(texture, adjTex, UITexLib, xpos, ypos) 
	{
		ClsButton* background = new ClsButton(UITexLib[7], tileToScreen(xID, yID).x, tileToScreen(xID, yID).y, false, 0);
		ClsButton* closeButt = new ClsButton(UITexLib[14], tileToScreen(xID, yID).x + 215, tileToScreen(xID, yID).y + 3, false, closeWindow);
		ClsButton* sellButt = new ClsButton(UITexLib[16], tileToScreen(xID, yID).x + 15, tileToScreen(xID, yID).y + 40, false, sellProduct, 28, 19);
		ClsButton* sell2Butt = new ClsButton(UITexLib[17], tileToScreen(xID, yID).x + 15, tileToScreen(xID, yID).y + 87, false, sellProd2);
		ClsButton* sell5Butt = new ClsButton(UITexLib[18], tileToScreen(xID, yID).x + 49, tileToScreen(xID, yID).y + 87, false, sellProd5);
		buttonWindow.push_back(background);
		buttonWindow.push_back(closeButt);
		buttonWindow.push_back(sellButt);
		buttonWindow.push_back(sell2Butt);
		buttonWindow.push_back(sell5Butt);
	};
	~ClsShop();

private:
	int typeModifier;
	//int maxCapacity;

};

class ClsOffice : public ClsBuilding
{
public:

	ClsOffice(Clstexture* texture, Clstexture* adjTex, Clstexture* ownedTexRef, std::vector<Clstexture*> UITexLib, int xpos, int ypos, int setPrice) : ClsBuilding(texture, adjTex, UITexLib, xpos, ypos)
	{
		ownedTex = ownedTexRef;
		ClsButton* background = new ClsButton(UITexLib[7], tileToScreen(xID, yID).x, tileToScreen(xID, yID).y, false, 0);
		ClsButton* closeButt = new ClsButton(UITexLib[14], tileToScreen(xID, yID).x + 215, tileToScreen(xID, yID).y + 3, false, closeWindow);
		ClsButton* buyButt = new ClsButton(UITexLib[15], tileToScreen(xID, yID).x + 150, tileToScreen(xID, yID).y + 80, false, buyBuilding, 28, 19);
		buttonWindow.push_back(background);
		buttonWindow.push_back(closeButt);
		buttonWindow.push_back(buyButt);

		price = (rand() % 501) + 500; //random price between 500 and 1000
	};
	~ClsOffice();

	inline Clstexture* GetTexture()
	{
		if (ownershipID == 1)
		{
			return ownedTex;
		}
		else if (isAdj)
			return adjTex;
		else
			return textureRef;
	}

	inline int GetOwnerShip() { return ownershipID; }
	inline void ChangeOwnership(int owner) { ownershipID = owner; };
	inline int GetPrice() { return price; }


private:
	Clstexture* ownedTex;
	int ownershipID = 0;
	int price;
};

class ClsFactory : public ClsBuilding
{
public:
	ClsFactory(Clstexture* texture, Clstexture* adjTex, std::vector<Clstexture*> UITexLib, int xpos, int ypos) : ClsBuilding(texture, adjTex, UITexLib, xpos, ypos)
	{
		ClsButton* background = new ClsButton(UITexLib[7], tileToScreen(xID, yID).x, tileToScreen(xID, yID).y, false, 0);
		ClsButton* closeButt = new ClsButton(UITexLib[14], tileToScreen(xID, yID).x + 228, tileToScreen(xID, yID).y + 3, false, closeWindow);
		ClsButton* capUpButt = new ClsButton(UITexLib[19], tileToScreen(xID, yID).x + 15, tileToScreen(xID, yID).y + 3, false, upgradeCapacity, 28, 0);
		ClsButton* speedUpButt = new ClsButton(UITexLib[20], tileToScreen(xID, yID).x + 120, tileToScreen(xID, yID).y + 3, false, upgradeSpeed, 28, 0);
		ClsButton* switchToVanButt = new ClsButton(UITexLib[21], tileToScreen(xID, yID).x + 15, tileToScreen(xID, yID).y + 68, false, switchToVan, 0, 6);
		buttonWindow.push_back(background);
		buttonWindow.push_back(closeButt);
		buttonWindow.push_back(capUpButt);
		buttonWindow.push_back(speedUpButt);
		buttonWindow.push_back(switchToVanButt);
		isFactory = true;
	};
	~ClsFactory();

private:
};