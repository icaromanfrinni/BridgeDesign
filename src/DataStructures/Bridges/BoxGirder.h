#pragma once
#ifndef BOXGIRDER_H
#define BOXGIRDER_H

#include "Bridge.h"
//#include "Coord_System.h"
#include "Line.h"
#include "CircularArc.h"

class BoxGirder : public Bridge
{
public:
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
	BoxGirder(const std::string& _name, Road* _road, const float& _cross_station, const float& _vertical_clearance, const float& _horizontal_clearance);

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