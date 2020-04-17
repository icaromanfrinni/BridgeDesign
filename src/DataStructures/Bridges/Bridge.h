#pragma once
#ifndef BRIDGE_H
#define BRIDGE_H

#include <vector>
#include <string>

#include "HalfEdge.h"
#include "Road.h"

// Default Vertical curves values
const float h1 = 1.08;	// height of eye above roadway surface
const float h2 = 0.60;	// height of object above roadway surface

// Default bridge values
const float SLOPE = 0.02f;
//const float TOP_LAYER = 0.07f;
const float GUARD_RAIL = 0.40f;
const float INCLINATION_RATIO = 0.25f;

//struct Pier
//{
//	float b, h, L;
//	CRAB::Vector4Df base;
//};

class Bridge
{
	// ALIGNMENTS
	void Vertical_Alignment();

public:
	std::string name;
	Road* road;
	Alignment2DHorizontal path2Dh;
	Alignment2DVertical path2Dv;
	Alignment3D path3D;
	//Alignment2DHorizontal path3D;
	std::vector<HED::solid*> model;
	float CS; // cross station
	float VC; // vertical clearance
	float HC; // horizontal clearance
	//std::vector<Pier> piers;

	// Bridge attributes
	float B;		// Width
	float H;		// Height
	float mainSpan; // Main span

	// DEFAULT CONSTRUCTOR
	Bridge();
	// OVERLOAD CONSTRUCTOR
	Bridge(const std::string& _name, Road* _road, const float& cross_station, const float& vertical_clearance, const float& horizontal_clearance);
	// DESTRUCTOR
	virtual ~Bridge() = 0;

	// MODEL CONSTRUCTOR
	// -----------------
	virtual void update() = 0;
};

#endif // BRIDGE_H