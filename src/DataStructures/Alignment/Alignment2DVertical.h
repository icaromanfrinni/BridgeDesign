#pragma once
#include "Alignment.h"

class Alignment2DVertical :
	public Alignment
{
public:
	//DEFAULT CONSTRUCTOR
	Alignment2DVertical();
	//DESTRUCTOR
	~Alignment2DVertical();

	CRAB::Vector4Df getPointFromStation(float dist);
	float getStationFromPoint(CRAB::Vector4Df p);
};