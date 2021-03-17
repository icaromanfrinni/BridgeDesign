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
	// ROAD DESIGN
	void StoppingSightDistance();
	// Alignment path3D;
public:
	std::string name;
	Alignment* alignment;
	std::vector<HED::solid*> model;
	float width;		// Roadway width
	float speed;		// Design speed
	float SSD;			// Stopping Sight Distance
	Vehicle* vehicle;	// Design vehicle

	// DEFAULT CONSTRUCTOR
	Road();
	// OVERLOAD CONSTRUCTOR
	Road(const std::string& _name, const float& _width, const float& _speed, Alignment* _alignment, Vehicle* _vehicle);
	// DESTRUCTOR
	~Road();

	// MODEL CONSTRUCTOR
	void update();
};

#endif // ROAD_H