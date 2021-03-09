#pragma once
#ifndef ALIGNMENT_H
#define ALIGNMENT_H

#include "NURBS.h"
#include "HorSegment.h"
#include "VerSegment.h"

//struct ArcLength
//{
//	float t, s;
//	// CONSTRUCTOR
//	ArcLength(const float& _t, const float& _s)
//		: t(_t), s(_s)
//	{	}
//};

class Alignment
{
private:

public:
	NURBS path2D;	// 2D horizontal curve
	//NURBS path3D;	// 3D curve
	std::string name;
	std::vector<HorSegment*> plan;			// Horizontal alignment
	std::vector<VerSegment*> profile;		// Vertical alignment
	//std::vector<ArcLength> arcLength_table;	// Arc Length Table

	// DEFAULT CONSTRUCTOR
	Alignment();
	// DEFAULT CONSTRUCTOR
	Alignment(const std::string& _name, const std::vector<HorSegment*>& _plan, const std::vector<VerSegment*>& _profile);
	// DESTRUCTOR
	~Alignment();

	// FIND THE VERTICAL SEGMENT FROM STATION
	int findSegment(const float& station) const;

	// RETURN
	CRAB::Vector4Df getPosition(const float& t) const;
	CRAB::Vector4Df getTangent(const float& t) const;
	CRAB::Vector4Df getNormalUp(const float& t/*, const float& V*/) const;
	bool isClockwise(const float& t) const;
	float getCurvature(const float& t) const;
	float getRadius(const float& t) const;
	float getDistance(const float& t) const;
	//float getHorDistance(const float& t) const;

	// RETURNS THE 't' PARAMETER OF NURBS CURVE
	float findParameter(const float& s) const;
	// RETURNS THE LENGTH OF VERTICAL ALIGNMENT
	float getProfileLength() const;
	// RETURNS THE LENGTH OF HORIZONTAL ALIGNMENT
	float getRoadLength() const;
	// RETURNS THE 4D POINT FROM STATION
	CRAB::Vector4Df getPositionFromStation(const float& station) const;
	// RETURNS THE 4D TANGENT VECTOR FROM STATION
	CRAB::Vector4Df getTangentFromStation(const float& station) const;
};

#endif