#pragma once
#ifndef ALIGNMENT_H
#define ALIGNMENT_H

#include <iostream>
#include <vector>
#include <cmath>

#include "Linear_Algebra.h"
#include "Geometry.h"

#define STEP 20

// Bézier Curve
class Alignment
{
	CRAB::Curve VerticalAlignment;
	CRAB::Curve HorizontalAlignment;
public:
	CRAB::Curve path3D;

	// DEFAULT CONSTRUCTOR
	Alignment();
	// OVERLOAD CONSTRUCTOR
	Alignment(const CRAB::Curve& _vertical, const CRAB::Curve& _horizontal);
	// DESTRUCTOR
	~Alignment();

	// RETURNS A POINT ON THE CURVE
	CRAB::Vector4Df getPosition(const float& t) const;
	// RETURNS THE CURVE TANGENT
	CRAB::Vector4Df getTangent(const float& t) const;
	// RETURNS THE CURVE NORMAL
	CRAB::Vector4Df getNormal(const float& t) const;
	// RETURNS THE CURVE NORMAL UP (w/ Superelevation)
	CRAB::Vector4Df getNormalUp(const float& t) const;
	// RETURNS THE CURVE BINORMAL
	CRAB::Vector4Df getBinormal(float t) const;
	// RETURNS THE CURVATURE
	float getCurvature(float t) const;
	// RETURNS THE RADIUS OF CURVATURE
	float getRadius(float t) const;
	// RETURNS THE CURVE LENGTH
	float getLength() const;

	// ************************************************************************* //

	//// Return the closest collision distance of a ray and the segment
	//CRAB::Vector4Df Collision(const Ray& ray) const;
	//// Return true if the point P intersect the segment
	//bool Contains(const CRAB::Vector4Df& p) const;

	//CRAB::Vector4Df getPointFromStation(float dist);
	//float getStationFromPoint(CRAB::Vector4Df p);
};

#endif // ALIGNMENT_H