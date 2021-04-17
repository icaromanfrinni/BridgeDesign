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
	// Straight Line || Circular Arc
	Geometry* segment;

public:

	// DEFAULT CONSTRUCTOR
	HorSegment();
	// OVERLOAD CONSTRUCTOR (Straight Line)
	//HorSegment(const glm::vec3& _p0, const glm::vec3& _p1);
	HorSegment(const CRAB::Vector4Df& _p1, const CRAB::Vector4Df& _p2);
	// OVERLOAD CONSTRUCTOR (Horizontal Circular Arc)
	//HorSegment(const glm::vec3& _p0, const glm::vec3& _p1, const glm::vec3& _p2);
	HorSegment(const CRAB::Vector4Df& _p1, const CRAB::Vector4Df& _p2, const CRAB::Vector4Df& _p3);
	// DESTRUCTOR
	~HorSegment();

	Geometry* getSegment() const;
};

#endif // HORSEGMENT_H