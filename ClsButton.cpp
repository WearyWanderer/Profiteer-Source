#include "ClsButton.h"

ClsButton::~ClsButton()
{
	
}

int ClsButton::buttonClicked() //if generic function pointers were allowed this would have been my go-to, using callback functions
{
	switch (callbackID)
	{
	case playGame:

		return 1;
		break;
	case exitGame:

		return 2;
		break;
	case goToAI:

		return 3;
		break;
	case productScreen:

		return 4;
		break;
	default:
		return 0;
		break;
	}
}

void VisFeedbackButton::ChangeVis(int change, ClsButton* priceSlider, ClsButton* weightSlider)
{
	if (currentVis + change <= 3 && currentVis + change >= 1)
		currentVis += change;

	switch (currentVis)
	{
	case 1:
	{
		priceSlider->SetX(370);
		weightSlider->SetX(365);
	}
		break;
	case 2:
	{
		priceSlider->SetX(410);
		weightSlider->SetX(400);
	}
		break;
	case 3:
	{
		priceSlider->SetX(555);
		weightSlider->SetX(465);
	}
		break;
	default:
		break;
	}
		
}
