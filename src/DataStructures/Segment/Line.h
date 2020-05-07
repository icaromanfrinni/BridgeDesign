#pragma once
#ifndef LINE_H
#define LINE_H

#include "Segment.h"

class Line
	: public Segment
{
	glm::vec3 p0, p1;

public:
	// DEFAULT CONSTRUCTOR
	Line();
	// OVERLOAD CONSTRUCTOR (from CRAB::Vector4Df)
	Line(const glm::vec3 &_p0, const glm::vec3 &_p1);
	// DESTRUCTOR
	~Line();

	// RETURN
	glm::vec3 getStartPoint() const;
	glm::vec3 getMidPoint() const;
	glm::vec3 getEndPoint() const;
	float getLength() const;
	float getY(const float& x) const;

	// RETURN THE WEIGHT OF THE CONTROL POINT
	float midPointWeight() const;
};

#endif // LINE_H