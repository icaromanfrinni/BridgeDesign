#pragma once
#ifndef ROAD_H
#define ROAD_H

#include <string>

class Road
{
public:
	std::string name;
	float B; // width

	//DEFAULT CONSTRUCTOR
	Road();
	//OVERLOAD CONSTRUCTOR
	Road(const std::string& _name, const float& width);

	//DESTRUCTOR
	~Road();
};

#endif // ROAD_H