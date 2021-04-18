#pragma once
#ifndef HORSEGMENT_H
#define HORSEGMENT_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "Linear_Algebra.h"

// Straight Line || Circular Arc
class HorSegment
{
	CRAB::Vector4Df p1, p2, p3;

public:
	// DEFAULT CONSTRUCTOR
	HorSegment();
	// OVERLOAD CONSTRUCTOR (Straight Line)
	HorSegment(const CRAB::Vector4Df& _p1, const CRAB::Vector4Df& _p3);
	// OVERLOAD CONSTRUCTOR (Circular Arc)
	HorSegment(const CRAB::Vector4Df& _p1, const CRAB::Vector4Df& _p2, const CRAB::Vector4Df& _p3);
	// DESTRUCTOR
	~HorSegment();

	// RETURN (glm::vec3)
	glm::vec3 getStartPoint3D() const;
	glm::vec3 getMidPoint3D() const;
	glm::vec3 getEndPoint3D() const;
	// RETURN (CRAB::Vector4Df)
	CRAB::Vector4Df getStartPoint4D() const;
	CRAB::Vector4Df getMidPoint4D() const;
	CRAB::Vector4Df getEndPoint4D() const;
	// RETURN THE WEIGHT OF THE CONTROL POINT
	float midPointWeight() const;
};

#endif // HORSEGMENT_H