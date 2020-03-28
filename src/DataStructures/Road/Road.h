#pragma once
#ifndef ROAD_H
#define ROAD_H

#include <iostream>
#include <cmath>
#include <string>
#include <vector>

#include "Segment.h"

// Default Vertical curves values
const float t = 2.5f;	// brake reaction time
const float a = 3.4f;	// deceleration rate

class Road
{
public:
	std::string name;
	std::vector<Segment*> alignment;
	float width;	// Roadway width
	float speed;	// Speed design
	int S;			// Stopping Sight Distance

	//DEFAULT CONSTRUCTOR
	Road();
	//OVERLOAD CONSTRUCTOR
	Road(const std::string& _name, const float& _width, const float& _speed);

	//ADD NEW SEGMENT
	void AddSegment(Segment* _segment);

	//DESTRUCTOR
	~Road();
};

#endif // ROAD_H