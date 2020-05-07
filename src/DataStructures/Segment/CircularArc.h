#pragma once
#ifndef CIRCULARARC_H
#define CIRCULARARC_H

#include "Segment.h"

class CircularArc
	: public Segment
{
	glm::vec3 p0, p1, p2;

public:
	// DEFAULT CONSTRUCTOR
	CircularArc();
	// OVERLOAD CONSTRUCTOR (from CRAB::Vector4Df)
	CircularArc(const glm::vec3 &_p0, const glm::vec3 &_p1, const glm::vec3 &_p2);
	// DESTRUCTOR
	~CircularArc();

	// RETURN
	glm::vec3 getStartPoint() const;
	glm::vec3 getMidPoint() const;
	glm::vec3 getEndPoint() const;
	float getLength() const;
	float getY(const float& x) const;

	// RETURN THE WEIGHT OF THE CONTROL POINT
	float midPointWeight() const;
};

#endif // CIRCULARARC_H