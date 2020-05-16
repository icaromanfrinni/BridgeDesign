#pragma once
#ifndef HORSEGMENT_H
#define HORSEGMENT_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "Linear_Algebra.h"
#include "Geometry.h"

class HorSegment
{
public:
	// Straight Line || Circular Arc || Circular Arc with Transition Curve (Clothoid)
	Geometry* segment;

	// DEFAULT CONSTRUCTOR
	HorSegment();
	// OVERLOAD CONSTRUCTOR (Straight Line)
	HorSegment(const glm::vec3& _p0, const glm::vec3& _p1);
	// OVERLOAD CONSTRUCTOR (Horizontal Circular Arc)
	HorSegment(const glm::vec3& _p0, const glm::vec3& _p1, const glm::vec3& _p2);
	// DESTRUCTOR
	~HorSegment();
};

#endif // HORSEGMENT_H