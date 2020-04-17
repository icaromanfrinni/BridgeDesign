#pragma once
#ifndef ROAD_H
#define ROAD_H

#include <iostream>
#include <cmath>
#include <string>
#include <vector>

#include "Alignment2DHorizontal.h"
#include "Alignment2DVertical.h"
#include "Alignment3D.h"

// Default Vertical curves values
const float t = 2.5f;	// brake reaction time
const float a = 3.4f;	// deceleration rate

class Road
{
public:
	std::string name;
	Alignment2DHorizontal path2Dh;
	Alignment2DVertical path2Dv;
	Alignment3D path3D;
	float width;	// Roadway width
	float speed;	// Speed design
	int S;			// Stopping Sight Distance

	// DEFAULT CONSTRUCTOR
	Road();
	// OVERLOAD CONSTRUCTOR
	Road(const std::string& _name, const float& _width, const float& _speed);
	// DESTRUCTOR
	~Road();

	// ADD NEW CONTROL POINT
	void AddVerPoint(const CRAB::Vector4Df& p);
	void AddHorPoint(const CRAB::Vector4Df& p);
};

#endif // ROAD_H