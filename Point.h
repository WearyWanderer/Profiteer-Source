#pragma once

class Point
{
public:

	Point();
	Point(int x, int y) : x(x), y(y) {};
	~Point();

	inline void Set(int newX, int newY) { x = newX; y = newY; }
	inline void SetIncremental(int newX, int newY) { x += newX; y += newY; }
	inline bool Compare(int compX, int compY) 
	{ 
		if (x == compX && y == compY)
			return true;
		else
			return false;
	
	}

	int x, y;
};