#pragma once
#ifndef BRIDGE_H
#define BRIDGE_H

#include <vector>
#include <string>

#include "Road.h"
#include "GlobalVariables.h"

// Default bridge values
const float SLOPE = 0.0f;
const float TOP_LAYER = 0.10f;
const float GUARD_RAIL = 0.40f;
//const float INCLINATION_RATIO = 0.25f;

// MAX Transverse Slope
//const float SLOPE_MAX = 0.06f;

struct Pier
{
	float b, h, L, station, ang, depth;
	CRAB::Vector4Df base, dir;
};

class Bridge
{
	// ALIGNMENTS
	std::vector<HorSegment*> Horizontal_Alignment();
	std::vector<VerSegment*> Vertical_Alignment();

public:
	std::string name;
	Road* road;
	Alignment* alignment;
	std::vector<HED::solid*> model;
	float CS;		// cross station
	float VC;		// vertical clearance
	float HC;		// horizontal clearance
	float start_S;	// start station
	float end_S;	// end station
	float mainSpan; // Main span
	float EL;		// elevation level
	float WS;		// water surface
	std::vector<Pier> piers;
	std::vector<Alignment*> abutments;

	// Bridge attributes
	float B;		// Width
	float H;		// Height

	bool hasWidening = true;
	bool haunchedGirder = true;
	bool automaticPiers = true;

	// DEFAULT CONSTRUCTOR
	Bridge();
	// OVERLOAD CONSTRUCTOR (Viaduct)
	Bridge(const std::string& _name, Road* _road, const float& cross_station, const float& vertical_clearance, const float& horizontal_clearance, const float& start_station, const float& end_station);
	// OVERLOAD CONSTRUCTOR (Overpass)
	Bridge(const std::string& _name, Road* _road, const float& cross_station, const float& vertical_clearance, const float& horizontal_clearance, const float& elevation_level, const float& start_station, const float& end_station);
	// OVERLOAD CONSTRUCTOR (Bridge)
	Bridge(const std::string& _name, Road* _road, const float& cross_station, const float& vertical_clearance, const float& horizontal_clearance, const float& elevation_level, const float& water_surface, const float& start_station, const float& end_station);
	// DESTRUCTOR
	virtual ~Bridge() = 0;

	// UPDATE ALL THE PARAMETERS
	void SetupBridge();

	// SUPERELEVATION
	float Superelevation(const float& t) const;
	// GET NORMAL VECTOR WITH SUPERELEVATION
	CRAB::Vector4Df getNormal(const float& t) const;
	// WIDENING
	float Widening(const float& t) const;

	// MODEL CONSTRUCTOR
	virtual void SetupSection() = 0;
	virtual void SetupPiers(const int& nPiers) = 0;
	virtual void UpdatePiers() = 0;
	virtual void AddPier() = 0;
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