#include "WorldComponent.hpp"

#pragma region Constructor/Destructor

WorldComponent::WorldComponent()
{
}


WorldComponent::~WorldComponent()
{
	mapInstance.~ClsMapData();
}

#pragma endregion

bool compare_entry(const Tile* e1, const Tile* e2) {
	if (e1->x != e2->x)
		return (e1->x < e2->x);
	return (e1->y < e2->y);
}

void WorldComponent::LoadMapTiles(VisualComponent* textureLib, string filepath)
{
	int movementIDArray[43];
	std::ifstream input("maps/Config_Movement.txt");

	for (int i = 0; i < 42; i++) 
	{
		input >> movementIDArray[i];
	}

	const char * c = filepath.c_str();

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(c);

	if (result) //if successful, we'll load the map here
	{
		int tilesToInit = doc.document_element().child("GridInfo").child("TileCount").text().as_int(); //amount of tiles we're drawing
		pugi::xml_node nodes = doc.document_element().child("TileData");

		for (pugi::xml_node tool = nodes.child("Tile"); tool; tool = tool.next_sibling("Tile"))
		{
			Tile* tempTile = new Tile();
			int xID = tool.child("xID").text().as_int(); //grab the xid
			int yID = tool.child("yID").text().as_int(); //grab the yid
			int spriteID = tool.child("spriteID").text().as_int();
			if (checkMovementValidityList(spriteID, movementIDArray, 42)) //make this broader or a function of some sort eventually
			{
				tempTile->movementTile = true;
			}
			else
			{
				tempTile->movementTile = false;
			}

				tempTile->InitTile(xID, yID, textureLib->tilesTextureLibrary[spriteID], spriteID); //create this tile
				mapInstance.tilesList.push_back(tempTile);
		}

		std::sort(mapInstance.tilesList.begin(), mapInstance.tilesList.end(), compare_entry);
	}
	else
	{
		HAPI->UserMessage(result.description(), "Error with map file");
	}

	SeedBuildings(textureLib);
}

void WorldComponent::SeedBuildings(VisualComponent* textureLib)
{
	srand(HAPI->GetTime());
	for (auto i : mapInstance.GetTileData())
	{
		if (i->isoID == 50 || i->isoID == 58 || i->isoID == 56)
		{
			if (checkMovementTileAdjacency(i->x + 1, i->y))
			{
				int chance = rand() % 100;
				if (chance >= 63) // 47% chance to spawn
				{
					int texID = rand() % 3;
					ClsOffice* tempOffice = new ClsOffice(textureLib->buildingTextureLibrary[texID], textureLib->buildingTextureLibrary[texID + 6], textureLib->buildingTextureLibrary[texID + 12], textureLib->UITextureLibrary, i->x, i->y, 300);
					mapInstance.buildingList.push_back(tempOffice);
				}
			}
			else if (checkMovementTileAdjacency(i->x, i->y + 1))
			{
				int chance = rand() % 100;
				if (chance >= 63) // 47% chance to spawn
				{
					int texID = (rand() % 3) + 3;
					ClsShop* tempShop = new ClsShop(textureLib->buildingTextureLibrary[texID], textureLib->buildingTextureLibrary[texID + 6], textureLib->UITextureLibrary, i->x, i->y);
					mapInstance.buildingList.push_back(tempShop);
				}
			}
		}
	}

	ClsFactory* spawnFactory = new ClsFactory(textureLib->buildingTextureLibrary[15], textureLib->buildingTextureLibrary[16], textureLib->UITextureLibrary, 7, 26);
	mapInstance.buildingList.push_back(spawnFactory);

}

bool WorldComponent::checkMovementValidityList(int tileID, int sourceList[], int length)
{
	for (int i = 0; i < length; i++) //for each possible movement tile ID
	{
		if (tileID == sourceList[i]) //if we have a match
		{
			return true;
		}
	}
	return false; //otherwise return false and do not initialise as movement tile
}

bool WorldComponent::checkMovementTileAdjacency(int xAdj, int yAdj)
{
	for (unsigned int i = 0; i < mapInstance.GetTileData().size(); i++)
	{
		if (xAdj == mapInstance.tilesList[i]->x && yAdj == mapInstance.tilesList[i]->y) //if we are looking at the adjacent tile
		{
			if (mapInstance.tilesList[i]->movementTile)
			{
				return true;
			}
		}
	}

	return false; //otherwise return false
}