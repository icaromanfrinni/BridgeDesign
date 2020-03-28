#pragma once
#ifndef BOXGIRDER_H
#define BOXGIRDER_H

#include "Bridge.h"
#include "Alignment.h"
//#include "Coord_System.h"
#include "Line.h"
#include "CircularArc.h"

// Default bridge values
const float SLOPE = 0.02f;
//const float TOP_LAYER = 0.07f;
const float GUARD_RAIL = 0.40f;
const float INCLINATION_RATIO = 0.25f;

class BoxGirder : public Bridge
{
	std::vector<Segment*> path;
	std::vector<HED::solid*> model;

public:
	// Intersection
	float CS;	// cross-station
	float VC;	// vertical clearance
	float HC;	// horizontal clearance

	// Bridge Attributes
	float B;    // Width
	float H;    // Height
	float Lb;   // slab cantilever
	float h;    // slab thickness
	float bw;   // webs slab thickness
	float b;    // bottom slab width
	float th;   // horizontal haunch
	float tv;   // vertical haunch
	float span; // main span

	//DEFAULT CONSTRUCTOR
	BoxGirder();
	//OVERLOAD CONSTRUCTOR
	//BoxGirder(const Alignment &roadway, const CRAB::Vector4Df& _start, const CRAB::Vector4Df& _end);
	BoxGirder(const Alignment& roadway, const float& _CS, const float& _VC, const float& _HC);

	//DESTRUCTOR
	~BoxGirder();

	//MODEL CONSTRUCTOR
	void update();
	void updateAutoSection();

	//ALLOCATION
	void setSpan(const float& _span);
	void setWidth(const float& _B);
	void setH(const float& _H);
	void setBw(const float& _bw);

	// return values
	std::vector<HED::solid*> getModel() const;
	std::vector<Segment*> getPath() const;
	float getWidth() const;
	float getSpan() const;
	float getHeight() const;
	float getLb() const;
	float getDeckH() const;
	float getBw() const;
	float getSlabWidth() const;
	float getTH() const;
	float getTV() const; 
};

#endif // BOXGIRDER_H