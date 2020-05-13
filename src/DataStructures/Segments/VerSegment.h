#pragma once
#ifndef VERSEGMENT_H
#define VERSEGMENT_H

#include <iostream>
#include <glm/glm.hpp>
#include "Linear_Algebra.h"
#include "Geometry.h"

class VerSegment
{
public:
	Geometry* segment;

	// DEFAULT CONSTRUCTOR
	VerSegment();
	// OVERLOAD CONSTRUCTOR (Straight Line)
	VerSegment(const glm::vec3& _p0, const glm::vec3& _p1);
	// OVERLOAD CONSTRUCTOR (Horizontal Circular Arc)
	VerSegment(const glm::vec3& _p0, const glm::vec3& _p1, const glm::vec3& _p2);
	// DESTRUCTOR
	~VerSegment();

	// RETURN (glm::vec3)
	glm::vec3 getStartPoint() const;
	glm::vec3 getMidPoint() const;
	glm::vec3 getEndPoint() const;
	// RETURN (CRAB::Vector4Df)
	CRAB::Vector4Df getStart4DPoint() const;
	CRAB::Vector4Df getMid4DPoint() const;
	CRAB::Vector4Df getEnd4DPoint() const;

	// RETURN THE ELEVATION
	float getY(const float& x) const;
};

#endif // VERSEGMENT_H