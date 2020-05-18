#pragma once
#ifndef ROAD_H
#define ROAD_H

#include <iostream>
#include <cmath>
#include <string>
#include <vector>

#include "HalfEdge.h"
#include "Alignment.h"

// Default Vertical curves values
const float t = 2.5f;	// brake reaction time
const float a = 3.4f;	// deceleration rate

class Road
{
	//Alignment path3D;
public:
	std::string name;
	Alignment* alignment;
	std::vector<HED::solid*> model;
	float width;	// Roadway width
	float speed;	// Speed design
	int S;			// Stopping Sight Distance

	// DEFAULT CONSTRUCTOR
	Road();
	// OVERLOAD CONSTRUCTOR
	Road(const std::string& _name, const float& _width, const float& _speed, Alignment* _alignment);
	// DESTRUCTOR
	~Road();

	//MODEL CONSTRUCTOR
	void update();
};

#endif // ROAD_H