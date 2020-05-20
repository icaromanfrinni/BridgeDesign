#pragma once
#ifndef BRIDGE_H
#define BRIDGE_H

#include <vector>
#include <string>

#include "Road.h"

// Default Vertical curves values
const float h1 = 1.08;	// height of eye above roadway surface
const float h2 = 0.60;	// height of object above roadway surface

// Default bridge values
const float SLOPE = 0.0f;
const float TOP_LAYER = 0.10f;
const float GUARD_RAIL = 0.40f;
const float INCLINATION_RATIO = 0.25f;

struct Pier
{
	float b, h, L;
	CRAB::Vector4Df base, dir;
};

class Bridge
{
	// INITIALIZES ALL THE PARAMETERS
	void SetupBridge();
	// ALIGNMENTS
	std::vector<HorSegment*> Horizontal_Alignment();
	std::vector<VerSegment*> Vertical_Alignment();

public:
	std::string name;
	Road* road;
	Alignment* alignment;
	std::vector<HED::solid*> model;
	float CS; // cross station
	float VC; // vertical clearance
	float HC; // horizontal clearance
	float EL; // elevation level
	float WS; // water surface
	std::vector<Pier> piers;

	// Bridge attributes
	float B;		// Width
	float H;		// Height
	float mainSpan; // Main span

	// DEFAULT CONSTRUCTOR
	Bridge();
	// OVERLOAD CONSTRUCTOR (Viaduct)
	Bridge(const std::string& _name, Road* _road, const float& cross_station, const float& vertical_clearance, const float& horizontal_clearance);
	// OVERLOAD CONSTRUCTOR (Overpass)
	Bridge(const std::string& _name, Road* _road, const float& cross_station, const float& vertical_clearance, const float& horizontal_clearance, const float& elevation_level);
	// OVERLOAD CONSTRUCTOR (Bridge)
	Bridge(const std::string& _name, Road* _road, const float& cross_station, const float& vertical_clearance, const float& horizontal_clearance, const float& elevation_level, const float& water_surface);
	// DESTRUCTOR
	virtual ~Bridge() = 0;

	// MODEL CONSTRUCTOR
	// -----------------
	virtual void update() = 0;
};

#endif // BRIDGE_H