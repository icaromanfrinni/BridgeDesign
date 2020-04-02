#pragma once
#ifndef ROAD_H
#define ROAD_H

#include <iostream>
#include <cmath>
#include <string>
#include <vector>

#include "Segment.h"

struct Alignment
{
	// List of segments
	std::vector<Segment*> segments;

	// Return Point from Station
	CRAB::Vector4Df getPointFromStation(float dist)
	{
		float t = 0.0f;
		CRAB::Matrix4 PlaneXZ = CRAB::Matrix4{
			1, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1 };

		for (int i = 0; i < segments.size(); i++)
		{
			CRAB::Vector4Df vPxz = segments[i]->getEndPoint() - segments[i]->getStartPoint();
			vPxz = PlaneXZ * vPxz;
			t = dist / vPxz.length();
			if (t <= 1.0f)
				return segments[i]->getPoint(t);
			else dist -= vPxz.length();
		}

		return segments.back()->getPoint(t);
	}
};

// Default Vertical curves values
const float t = 2.5f;	// brake reaction time
const float a = 3.4f;	// deceleration rate

class Road
{
public:
	std::string name;
	Alignment alignment;
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