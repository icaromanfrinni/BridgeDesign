#pragma once
#ifndef ROAD_H
#define ROAD_H

#include <iostream>
#include <cmath>
#include <string>
#include <vector>

#include "HalfEdge.h"
#include "Alignment.h"
#include "Vehicle.h"

class Road
{
public:
	std::string name;
	Alignment* alignment;
	//std::vector<HED::solid*> solids;
	float width;		// Roadway width
	float speed;		// Design speed
	Vehicle* vehicle;	// Design vehicle

	// DEFAULT CONSTRUCTOR
	Road();
	// OVERLOAD CONSTRUCTOR
	Road(const std::string& _name, const float& _width, const float& _speed, Alignment* _alignment, Vehicle* _vehicle);
	// DESTRUCTOR
	~Road();

	// ROAD DESIGN
	float StoppingSightDistance();

	// MODEL CONSTRUCTOR
	//void update();
};

#endif // ROAD_H