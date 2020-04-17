#pragma once
#ifndef BOXGIRDER_H
#define BOXGIRDER_H

#include "Bridge.h"
//#include "Coord_System.h"

class BoxGirder : public Bridge
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
	//OVERLOAD CONSTRUCTOR
	BoxGirder(const std::string& _name, Road* _road, const float& cross_station, const float& vertical_clearance, const float& horizontal_clearance);

	//DESTRUCTOR
	~BoxGirder();

	//MODEL CONSTRUCTOR
	void update();
};

#endif // BOXGIRDER_H