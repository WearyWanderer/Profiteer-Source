#pragma once

#include <string>
#include <functional>
#include <HAPI_lib.h>
#include "texture.hpp"

//below is a large enumerator list that will determine the actions for each button when the input manager reecieves the callback integer from them, if they have been clicked
enum ButtonResponseList
{
	playGame = 1,
	exitGame = 2,
	goToAI = 3,
	unPauseGame = 4,
	productScreen = 5,
	itemLeft = 6,
	itemRight = 7,
	draggablePrice = 8,
	draggableWeight = 9,
	closeWindow = 10,
	buyBuilding = 11,
	sellProduct = 12,
	sellProd2 = 13,
	sellProd5 = 14,
	upgradeCapacity = 15,
	upgradeSpeed = 16,
	switchToVan = 17
};

class ClsButton
{
public:
	ClsButton(Clstexture* buttonTexRef, int x, int y, bool initShow, int buttonType) : buttonTex(buttonTexRef), x(x), y(y), doShow(initShow), buttonBoundsRect(buttonTexRef->ReturnRect(x, y)), callbackID(buttonType) {} //this 
	ClsButton(Clstexture* buttonTexRef, int x, int y, bool initShow, int buttonType, int spriteRectCutoffX, int spriteRectCutoffY) : buttonTex(buttonTexRef), x(x), y(y), doShow(initShow), buttonBoundsRect(buttonTexRef->ReturnRect(x, y)), callbackID(buttonType) 
	{
		cutoffX = spriteRectCutoffX; //this is the amount of the sprite that is cut off in button clicks X
		cutoffY = spriteRectCutoffY; //this is the Y amount
	}
	~ClsButton();
	inline bool CheckIfShow(){ return doShow; }
	inline void ToggleShow(){ doShow = !doShow; }
	virtual inline int GetX(){ return x; }
	virtual inline int GetY(){ return y; }
	virtual inline Clstexture* GetTexture(){ return buttonTex; }
	inline Clsrectangle GetRect(){ return buttonBoundsRect; }

#pragma region DraggableButtonFuns

	virtual inline void ChangeVis(int change){}; //this is virtual that is needed to call on the vis buttons in the vector
	virtual inline void ChangeVis(int change, ClsButton* priceSlider, ClsButton* weightSlider){};
	virtual inline int GetVisID(){ return 0; };
	virtual inline void SetX(int thisX){}
	virtual inline void SetY(int thisY){}
	virtual inline int DistanceX(){ return 0; }
	virtual inline int DistanceY(){ return 0; }

#pragma endregion

	int buttonClicked();
	const int callbackID;
	int cutoffX = 0;
	int cutoffY = 0;

protected:
	Clstexture* buttonTex;
	Clsrectangle buttonBoundsRect;
	int x, y;
	bool doShow; //this is for toggling visibility of buttons
};

class VisFeedbackButton : public ClsButton //class of non-clickable button for toggleable visual feedbacks
{
public:
	VisFeedbackButton(Clstexture* visFeed1, Clstexture* visFeed2, Clstexture* visFeed3, int x, int y, bool initShow) : ClsButton(visFeed1, x, y, initShow, 0)
	{
		secondVisual = visFeed2;
		thirdVisual = visFeed3;
	};

	inline Clstexture* GetTexture()
	{
		switch (currentVis)
		{
		case 1:
			return buttonTex;
			break;
		case 2:
			return secondVisual;
			break;
		case 3:
			return thirdVisual;
			break;
		default:
			return buttonTex;
			break;
		}
	}

	void ChangeVis(int change, ClsButton* priceSlider, ClsButton* weightSlider);
	int GetVisID(){ return currentVis; }

protected:
	Clstexture* secondVisual;
	Clstexture* thirdVisual;

	int currentVis = 1;
};

class DraggableButton : public ClsButton
{
public:
	DraggableButton(Clstexture* visFeed1, int x, int y, int threshX, int threshY, bool initShow, int typeofDrag) : ClsButton(visFeed1, x, y, initShow, typeofDrag)
	{
		endX = x;
		endY = y;
		maxX = threshX;
		maxY = threshY;
	};

	inline int GetX(){ return endX; }
	inline int GetY(){ return endY; }
	inline void SetX(int thisX)
	{ 
		if (thisX <= x + maxX && thisX >= x)
		{
			endX = thisX;
			buttonBoundsRect.MoveRect(thisX - 5, endY, 26, 32); //minor alterations to the rect of bounds to enable easy clicking to drag in reverse
		}
	}
	inline void SetY(int thisY)
	{
		if (thisY <= y + maxY && thisY >= y)
		{
			endY = thisY;
			buttonBoundsRect.MoveRect(endX, thisY - 4, 16, 36);
		}
	}
	inline int DistanceX(){ return endX - x; }
	inline int DistanceY(){ return endY - y; }

protected:

	int endX; //the finalised point, x is the root point form which this originated
	int endY;
	int maxX; //maximum movement
	int maxY; //maximum movement
};

