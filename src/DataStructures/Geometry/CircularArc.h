#pragma once
#ifndef CIRCULARARC_H
#define CIRCULARARC_H

#include "Geometry.h"

class CircularArc
	: public Geometry
{
	CRAB::Vector4Df p1, p2, p3;

public:
	// DEFAULT CONSTRUCTOR
	CircularArc();
	// OVERLOAD CONSTRUCTOR
	CircularArc(const CRAB::Vector4Df& _p1, const CRAB::Vector4Df& _p2, const CRAB::Vector4Df& _p3);
	// DESTRUCTOR
	~CircularArc();

	// RETURN
	glm::vec3 getStartPoint() const;
	glm::vec3 getMidPoint() const;
	glm::vec3 getEndPoint() const;
	CRAB::Vector4Df getStartPoint4D() const;
	CRAB::Vector4Df getMidPoint4D() const;
	CRAB::Vector4Df getEndPoint4D() const;
	// RETURNS THE SEGMENT LENGTH
	float getLength() const;
	// RETURN THE ELEVATION
	float getY(const float& x) const;
	// RETURN THE WEIGHT OF THE CONTROL POINT
	float midPointWeight() const;

	CRAB::Vector4Df Collision(const CRAB::Ray& ray) const;
	bool Contains(const CRAB::Vector4Df& p) const;
};

#endif // CIRCULARARC_H