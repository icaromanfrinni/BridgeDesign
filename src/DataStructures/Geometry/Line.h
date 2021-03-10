#pragma once
#ifndef LINE_H
#define LINE_H

#include "Geometry.h"

class Line
	: public Geometry
{
	CRAB::Vector4Df p1, p2;

public:
	// DEFAULT CONSTRUCTOR
	Line();
	// OVERLOAD CONSTRUCTOR (from two points)
	Line(const CRAB::Vector4Df& _p1, const CRAB::Vector4Df& _p2);
	// DESTRUCTOR
	~Line();

	// RETURN
	glm::vec3 getStartPoint() const;
	glm::vec3 getMidPoint() const;
	glm::vec3 getEndPoint() const;
	CRAB::Vector4Df getStartPoint4D() const;
	CRAB::Vector4Df getMidPoint4D() const;
	CRAB::Vector4Df getEndPoint4D() const;
	// RETURNS THE SEGMENT LENGTH
	float getLength() const;
	// RETURNS THE ELEVATION
	float getY(const float& x) const;
	// RETURNS THE WEIGHT OF THE CONTROL POINT
	float midPointWeight() const;

	CRAB::Vector4Df Collision(const CRAB::Ray& ray) const;
	bool Contains(const CRAB::Vector4Df& p) const;
};

#endif // LINE_H