#pragma once
#ifndef HORSEGMENT_H
#define HORSEGMENT_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "Linear_Algebra.h"
#include "Geometry.h"

/* Meek and Walton (2004) */
struct Clothoid
{
	int n = 10;	// n + 1 arcs
	float kA;	// Start Curvature
	float kB;	// End Curvature
	float S;	// Total arc length
};

class HorSegment
{
	std::vector<Geometry*> DiscretizedClothoid(const CRAB::Vector4Df& A, const CRAB::Vector4Df& B, const CRAB::Vector4Df& V) const;
	std::vector<Geometry*> DiscretizedClothoidInverted(const CRAB::Vector4Df& B, const CRAB::Vector4Df& A, const CRAB::Vector4Df& V) const;

public:
	// Straight Line || Circular Arc || Circular Arc with Transition Curve (Clothoid)
	Geometry* segment;
	// Transition curve
	bool transition;
	Clothoid spiral;

	// DEFAULT CONSTRUCTOR
	HorSegment();
	// OVERLOAD CONSTRUCTOR (Straight Line)
	HorSegment(const glm::vec3& _p0, const glm::vec3& _p1);
	// OVERLOAD CONSTRUCTOR (Horizontal Circular Arc)
	HorSegment(const glm::vec3& _p0, const glm::vec3& _p1, const glm::vec3& _p2);
	// OVERLOAD CONSTRUCTOR (Circular Arc with Transition Curve)
	HorSegment(const glm::vec3& _p0, const glm::vec3& _p1, const glm::vec3& _p2, const float& _S, const float& _kB);
	// DESTRUCTOR
	~HorSegment();

	// RETURN DISCRETIZED HORIZONTAL CURVE WITH SPIRAL TRANSITION SYMMETRIC
	std::vector<Geometry*> HorizontalCurve() const;
};

#endif // HORSEGMENT_H