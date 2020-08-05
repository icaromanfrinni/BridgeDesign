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
	float mainSpan; // Main span
	float EL; // elevation level
	float WS; // water surface
	std::vector<Pier> piers;

	// Bridge attributes
	float B;		// Width
	float H;		// Height

	// DEFAULT CONSTRUCTOR
	Bridge();
	// OVERLOAD CONSTRUCTOR (Viaduct)
	Bridge(const std::string& _name, Road* _road, const float& cross_station, const float& vertical_clearance, const float& horizontal_clearance, const float& main_span);
	// OVERLOAD CONSTRUCTOR (Overpass)
	Bridge(const std::string& _name, Road* _road, const float& cross_station, const float& vertical_clearance, const float& horizontal_clearance, const float& main_span, const float& elevation_level);
	// OVERLOAD CONSTRUCTOR (Bridge)
	Bridge(const std::string& _name, Road* _road, const float& cross_station, const float& vertical_clearance, const float& horizontal_clearance, const float& main_span, const float& elevation_level, const float& water_surface);
	// DESTRUCTOR
	virtual ~Bridge() = 0;

	// MODEL CONSTRUCTOR
	virtual void Setup() = 0;
	virtual void Update() = 0;

	// RETURN SECTION PARAMETERS
	virtual float get_CantileverLength() const = 0;
	virtual float get_DeckThickness() const = 0;
	virtual float get_WebThickness() const = 0;
	virtual float get_HaunchHeight() const = 0;
	virtual float get_BottomWidth() const = 0;
	virtual float get_HaunchWidth() const = 0;

	// ALLOCATION SECTION PARAMETERS
	virtual void set_CantileverLength(const float& _Lb) = 0;
	virtual void set_DeckThickness(const float& _h) = 0;
	virtual void set_WebThickness(const float& _tw) = 0;
	virtual void set_HaunchHeight(const float& _hc) = 0;
	virtual void set_BottomWidth(const float& _b) = 0;
	virtual void set_HaunchWidth(const float& _bc) = 0;
};

#endif // BRIDGE_H