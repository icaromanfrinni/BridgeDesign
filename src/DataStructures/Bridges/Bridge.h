#pragma once
#ifndef BRIDGE_H
#define BRIDGE_H

#include <vector>
#include <string>

#include "HalfEdge.h"
#include "Segment.h"
#include "Road.h"

// Default Vertical curves values
const float h1 = 1.08;	// height of eye above roadway surface
const float h2 = 0.60;	// height of object above roadway surface

// Default bridge values
const float SLOPE = 0.02f;
//const float TOP_LAYER = 0.07f;
const float GUARD_RAIL = 0.40f;
const float INCLINATION_RATIO = 0.25f;

class Bridge
{
public:
	std::string name;
	Road* road;
	std::vector<Segment*> alignment;
	std::vector<HED::solid*> model;
	float cross_station, vertical_clearance, horizontal_clearance;

	// DEFAULT CONSTRUCTOR
	Bridge();
	// DESTRUCTOR
	virtual ~Bridge() = 0;

	// MODEL CONSTRUCTOR
	// -----------------
	virtual void update() = 0;
	virtual void updateAutoSection() = 0;

	// VERTICAL ALIGNMENT
	// ------------------
	std::vector<Segment*> CrestVerticalCurve();
	std::vector<Segment*> SagVerticalCurve();

	// ALLOCATION
	// ----------
	virtual void setSpan(const float& _span) = 0;
	virtual void setWidth(const float& _B) = 0;
	virtual void setBw(const float& _bw) = 0;
	virtual void setH(const float& _H) = 0;
	
	// RETURN
	// ------
	/*virtual std::vector<HED::solid*> getModel() const = 0;
	virtual std::vector<Segment*> getPath() const = 0;*/
	virtual float getWidth() const = 0;
	virtual float getSpan() const = 0;
	virtual float getHeight() const = 0;
	virtual float getLb() const = 0;
	virtual float getDeckH() const = 0;
	virtual float getBw() const = 0;
	virtual float getSlabWidth() const = 0;
	virtual float getTH() const = 0;
	virtual float getTV() const = 0;
};

#endif // BRIDGE_H