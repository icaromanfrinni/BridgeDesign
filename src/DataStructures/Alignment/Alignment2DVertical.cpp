#include "Alignment2DVertical.h"

Alignment2DVertical::Alignment2DVertical()
{

}

Alignment2DVertical::~Alignment2DVertical()
{

}

// Return Point from Station
CRAB::Vector4Df Alignment2DVertical::getPointFromStation(float dist)
{
	float t = 0.0f;
	CRAB::Matrix4 PlaneXZ = CRAB::Matrix4{
		1, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 };

	for (int i = 0; i < segments.size(); i++)
	{
		CRAB::Vector4Df vPxz = segments[i]->getEndPoint() - segments[i]->getStartPoint();
		vPxz = PlaneXZ * vPxz;
		t = dist / vPxz.length();
		if (t <= 1.0f)
			return segments[i]->getPosition(t);
		else dist -= vPxz.length();
	}

	return segments.back()->getPosition(t);
}

// Return Station from Point
float Alignment2DVertical::getStationFromPoint(CRAB::Vector4Df p)
{
	CRAB::Vector4Df v = p - this->segments.front()->getStartPoint();
	v.y = 0.0f;
	return v.length();
}