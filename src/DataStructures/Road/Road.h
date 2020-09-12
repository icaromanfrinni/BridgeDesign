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
	// Alignment path3D;
public:
	std::string name;
	Alignment* alignment;
	std::vector<HED::solid*> model;
	float width;		// Roadway width
	float speed;		// Speed design
	Vehicle* vehicle;	// Default vehicle

	// DEFAULT CONSTRUCTOR
	Road();
	// OVERLOAD CONSTRUCTOR
	Road(const std::string& _name, const float& _width, const float& _speed, Alignment* _alignment, Vehicle* _vehicle);
	// DESTRUCTOR
	~Road();

	// ROAD DESIGN
	int StoppingSightDistance() const;

	//// MODEL CONSTRUCTOR
	//void update();
};

#endif // ROAD_H