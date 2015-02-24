#pragma once

#include <string>

class Product
{
public:
	Product();
	~Product();

	inline int GetType(){ return typeID; }
	inline int GetPrice(){ return pricePerUnit; }
	inline int GetPrice(int quantity){ return (pricePerUnit * quantity); }
	inline int GetWeight(){ return weightPerUnit; }
	inline int GetWeight(int quantity){ return (weightPerUnit * quantity); }
	inline void SetType(int type){ typeID = type; }
	inline void SetPrice(int price){ pricePerUnit = price; }
	inline void SetWeight(int weight){ weightPerUnit = weight; }
	inline void SetName(std::string newName){ name = newName; }

private:

	int typeID;
	int pricePerUnit;
	int weightPerUnit;
	std::string name;
};

