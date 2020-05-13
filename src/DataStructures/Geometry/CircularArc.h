#pragma once
#ifndef CIRCULARARC_H
#define CIRCULARARC_H

#include "Geometry.h"

class CircularArc
	: public Geometry
{
	glm::vec3 p0, p1, p2;

public:
	// DEFAULT CONSTRUCTOR
	CircularArc();
	// OVERLOAD CONSTRUCTOR (from glm::vec3)
	CircularArc(const glm::vec3 &_p0, const glm::vec3 &_p1, const glm::vec3 &_p2);
	// OVERLOAD CONSTRUCTOR (from CRAB::Vector4Df)
	CircularArc(const CRAB::Vector4Df& _p0, const CRAB::Vector4Df& _p1, const CRAB::Vector4Df& _p2);
	// DESTRUCTOR
	~CircularArc();

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
	// RETURN THE ELEVATION
	float getY(const float& x) const;
	// RETURN THE WEIGHT OF THE CONTROL POINT
	float midPointWeight() const;
};

#endif // CIRCULARARC_H