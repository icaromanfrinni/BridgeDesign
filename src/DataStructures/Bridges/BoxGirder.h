#pragma once
#ifndef BOXGIRDER_H
#define BOXGIRDER_H

#include "Bridge.h"
#include <algorithm>

struct Span
{
	float start;
	float end;
};

class BoxGirder
	: public Bridge
{
	std::vector<CRAB::Vector4Df> TopLayer_section(const float& t) const;
	std::vector<CRAB::Vector4Df> Deck_section(const float& t);
	std::vector<CRAB::Vector4Df> U_section(const float& t);
	//void OLD_Update();
	
	//std::vector<float> span_vector; // TODO: vetor de ponteiros
	std::vector<Span> span_vector;

	// REORDER PIERS
	void merge(std::vector<Pier>& X, int start, int mid, int end, std::vector<Pier>& aux);
	void mergesort(std::vector<Pier>& X, int start, int end, std::vector<Pier>& aux);
	void mergesort(std::vector<Pier>& X);

public:
	// Box-Girder Bridge Attributes
	float Lb;		// slab cantilever
	float h;		// slab thickness
	float bw;		// webs slab thickness
	float iw = 0.1f;// web inclination
	bool iLocked = false;
	float b;		// bottom slab width
	float th;		// horizontal haunch
	float tv;		// vertical haunch
	float dH;		// haunched girder

	//DEFAULT CONSTRUCTOR
	BoxGirder();
	// OVERLOAD CONSTRUCTOR (Viaduct)
	BoxGirder(const std::string& _name, Road* _road, const float& cross_station, const float& vertical_clearance, const float& horizontal_clearance, const std::vector<float>& _stations, const float& start_station, const float& end_station);
	// OVERLOAD CONSTRUCTOR (Overpass)
	BoxGirder(const std::string& _name, Road* _road, const float& cross_station, const float& vertical_clearance, const float& horizontal_clearance, const float& elevation_level, const float& start_station, const float& end_station);
	// OVERLOAD CONSTRUCTOR (Bridge)
	BoxGirder(const std::string& _name, Road* _road, const float& cross_station, const float& vertical_clearance, const float& horizontal_clearance, const float& elevation_level, const float& water_surface, const float& start_station, const float& end_station);
	// DESTRUCTOR
	~BoxGirder();

	// MODEL CONSTRUCTOR
	void SetupSection();
	void SetupSection(const float& t);
	void SetupPiers(const int& _nPiers);
	void SetupPiers(const std::vector<float>& _stations);
	void UpdatePiers();
	void AddPier();
	void Update();

	// RETURN SECTION PARAMETERS
	//float get_HaunchedGirder(const float& t) const;
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