#pragma once
#ifndef ALIGNMENT_H
#define ALIGNMENT_H

#include "NURBS.h"
#include "HorSegment.h"
#include "VerSegment.h"

class Alignment
{
private:
	NURBS path2D;	// 2D horizontal curve

public:
	std::string name;
	std::vector<HorSegment*> roadplan;		// Horizontal alignment
	std::vector<VerSegment*> profile;		// Vertical alignment
	std::vector<CRAB::Vector4Df*> VPI_list;	// 2D Intersection points

	// DEFAULT CONSTRUCTOR
	Alignment();
	// DEFAULT CONSTRUCTOR
	Alignment(const std::string& _name, const std::vector<HorSegment*>& _roadplan, const std::vector<VerSegment*>& _profile);
	// DESTRUCTOR
	~Alignment();

	// FIND THE VERTICAL SEGMENT FROM STATION
	int findSegment(const float& s) const;

	// RETURN
	glm::vec3 getPosition3D(const float& t) const;
	CRAB::Vector4Df getPosition(const float& t) const;
	CRAB::Vector4Df getTangent(const float& t) const;
	CRAB::Vector4Df getNormalUp(const float& t) const;
	bool isClockwise(const float& t) const;
	float getCurvature(const float& t) const;
	float getRadius(const float& t) const;
	float getDistance(const float& t) const;
	//float getHorDistance(const float& t) const;

	// RETURNS THE 't' PARAMETER OF NURBS CURVE
	float findParameter(const float& s) const;
	// RETURNS THE LENGTH OF VERTICAL ALIGNMENT
	//float getProfileLength() const;
	// RETURNS THE SPACE POSITION FROM STATION
	glm::vec3 getPositionFromStation3D(const float& s) const;
	CRAB::Vector4Df getPositionFromStation(const float& s) const;
	// RETURNS THE 4D TANGENT VECTOR FROM STATION
	CRAB::Vector4Df getTangentFromStation(const float& s) const;
};

#endif