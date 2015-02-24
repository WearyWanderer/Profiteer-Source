#include "ClsBuilding.h"


ClsBuilding::ClsBuilding(Clstexture* texture, Clstexture* adjTexture, std::vector<Clstexture*> UITexLib, int xpos, int ypos)
{
	textureRef = texture;
	adjTex = adjTexture;
	xID = xpos;
	yID = ypos;
	buildingRect.right = texture->ReturnWidth();
	buildingRect.bottom = texture->ReturnHeight();
}

void ClsBuilding::CleanupFunc()
{
	for (unsigned int i = 0; i < buttonWindow.size(); i++)
	{
		//some destructio of the button data here
		delete buttonWindow[i];
	}
}

ClsBuilding::~ClsBuilding()
{
	CleanupFunc();
}

ClsShop::~ClsShop()
{
}

ClsOffice::~ClsOffice()
{
}

ClsFactory::~ClsFactory()
{
}
