#pragma once
#ifndef BOXGIRDER_H
#define BOXGIRDER_H

#include "Bridge.h"

class BoxGirder
	: public Bridge
{
public:
	// Box-Girder Bridge Attributes
	float Lb;   // slab cantilever
	float h;    // slab thickness
	float bw;   // webs slab thickness
	float b;    // bottom slab width
	float th;   // horizontal haunch
	float tv;   // vertical haunch

	//DEFAULT CONSTRUCTOR
	BoxGirder();
	// OVERLOAD CONSTRUCTOR (Viaduct)
	BoxGirder(const std::string& _name, Road* _road, const float& cross_station, const float& horizontal_clearance);
	// OVERLOAD CONSTRUCTOR (Overpass)
	BoxGirder(const std::string& _name, Road* _road, const float& cross_station, const float& horizontal_clearance, const float& elevation_level);
	// OVERLOAD CONSTRUCTOR (Bridge)
	BoxGirder(const std::string& _name, Road* _road, const float& cross_station, const float& horizontal_clearance, const float& elevation_level, const float& water_surface);
	// DESTRUCTOR
	~BoxGirder();

	// MODEL CONSTRUCTOR
	void SetupSection();
	void SetupPiers(const int& nPiers);
	void UpdatePiers();
	void AddPier();
	void Update();

	// RETURN SECTION PARAMETERS
	float get_CantileverLength() const;
	float get_DeckThickness() const;
	float get_WebThickness() const;
	float get_HaunchHeight() const;
	float get_BottomWidth() const;
	float get_HaunchWidth() const;

	// ALLOCATION SECTION PARAMETERS
	void set_CantileverLength(const float& _Lb);
	void set_DeckThickness(const float& _h);
	void set_WebThickness(const float& _tw);
	void set_HaunchHeight(const float& _hc);
	void set_BottomWidth(const float& _b);
	void set_HaunchWidth(const float& _bc);
};

#endif // BOXGIRDER_H