#pragma once
#ifndef ROAD_H
#define ROAD_H

#include <iostream>
#include <cmath>
#include <string>
#include <vector>

#include "Segment.h"

struct VerticalAlignment
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
				return segments[i]->getPosition(t);
			else dist -= vPxz.length();
		}

		return segments.back()->getPosition(t);
	}

	// Return Station from Point
	float getStationFromPoint(CRAB::Vector4Df p)
	{
		CRAB::Vector4Df v = p - this->segments.front()->getStartPoint();
		v.y = 0.0f;
		return v.length();
	}
};

// Default Vertical curves values
const float t = 2.5f;	// brake reaction time
const float a = 3.4f;	// deceleration rate

class Road
{
public:
	std::string name;
	VerticalAlignment alignment;
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