#pragma once
#ifndef LINE_H
#define LINE_H

#include "Geometry.h"

class Line
	: public Geometry
{
	glm::vec3 p0, p1;

public:
	// DEFAULT CONSTRUCTOR
	Line();
	// OVERLOAD CONSTRUCTOR (from two points)
	Line(const glm::vec3 &_p0, const glm::vec3 &_p1);
	// OVERLOAD CONSTRUCTOR (from CRAB::Vector4Df)
	Line(const CRAB::Vector4Df& _p0, const CRAB::Vector4Df& _p1);
	// DESTRUCTOR
	~Line();

	// RETURN (glm::vec3)
	glm::vec3 getStartPoint() const;
	glm::vec3 getMidPoint() const;
	glm::vec3 getEndPoint() const;
	// RETURN (CRAB::Vector4Df)
	CRAB::Vector4Df getStart4DPoint() const;
	CRAB::Vector4Df getMid4DPoint() const;
	CRAB::Vector4Df getEnd4DPoint() const;
	// RETURNS THE SEGMENT LENGTH
	float getLength() const;
	// RETURNS THE ELEVATION
	float getY(const float& x) const;
	// RETURNS THE WEIGHT OF THE CONTROL POINT
	float midPointWeight() const;
};

#endif // LINE_H