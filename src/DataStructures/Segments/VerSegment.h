#pragma once
#ifndef VERSEGMENT_H
#define VERSEGMENT_H

#include <iostream>
#include <glm/glm.hpp>
#include "Linear_Algebra.h"

// Straight Line || Parabolic Arc
class VerSegment
{
	CRAB::Vector4Df p1, p2, p3;
	CRAB::Vector4Df tan1, tan2;
	float g1, g2;

public:
	// DEFAULT CONSTRUCTOR
	VerSegment();
	// OVERLOAD CONSTRUCTOR (Straight Line)
	VerSegment(const CRAB::Vector4Df& _p1, const CRAB::Vector4Df& _p3);
	// OVERLOAD CONSTRUCTOR (Parabolic Arc)
	VerSegment(const CRAB::Vector4Df& _p1, const CRAB::Vector4Df& _p2, const CRAB::Vector4Df& _p3);
	// OVERLOAD CONSTRUCTOR (Parabolic Arc from endpoints with A in percent)
	VerSegment(const CRAB::Vector4Df& _p1, const CRAB::Vector4Df& _p3, const float& A);
	// OVERLOAD CONSTRUCTOR (Parabolic Arc from midpoint)
	VerSegment(const CRAB::Vector4Df& _p2, const CRAB::Vector4Df& _tan1, const CRAB::Vector4Df& _tan2, const float& L);
	// DESTRUCTOR
	~VerSegment();

	// RETURN (glm::vec3)
	glm::vec3 getStartPoint3D() const;
	glm::vec3 getMidPoint3D() const;
	glm::vec3 getEndPoint3D() const;
	// RETURN (CRAB::Vector4Df)
	CRAB::Vector4Df getStartPoint4D() const;
	CRAB::Vector4Df getMidPoint4D() const;
	CRAB::Vector4Df getEndPoint4D() const;
	// RETURN THE ELEVATION
	float getY(const float& x) const;
	// RETURN THE INCLINATION OF A RAMP
	float getG1() const;
	float getG2() const;
};

#endif // VERSEGMENT_H