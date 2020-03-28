#pragma once
#ifndef ROAD_H
#define ROAD_H

#include <iostream>
#include <cmath>
#include <string>

class Road
{
public:
	std::string name;
	float W;	// Roadway width
	int S;		// Stopping Sight Distance

	//DEFAULT CONSTRUCTOR
	Road();
	//OVERLOAD CONSTRUCTOR
	Road(const std::string& _name, const float& width, const float& speed);

	//DESTRUCTOR
	~Road();
};

#endif // ROAD_H