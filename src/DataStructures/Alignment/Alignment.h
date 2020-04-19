#pragma once
#ifndef ALIGNMENT_H
#define ALIGNMENT_H

#include <iostream>
#include <vector>
#include <cmath>

#include "Linear_Algebra.h"

const int STEP = 20;

struct Ray
{
	CRAB::Vector4Df origin, direction;
};

// Bézier Curve
class Alignment
{
	int binomialCoefficient(const int& n, const int& i) const;
	float BernsteinPolynomial(const int& n, const int& i, const float& t) const;

	// CALCULATES THE DERIVATIVE FOR A CURVE OF ORDER 'n'
	CRAB::Vector4Df deriv(float t) const;
	CRAB::Vector4Df deriv2(float t) const;
	// https://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/Bezier/bezier-der.html

public:
	std::vector<CRAB::Vector4Df> points; // control points

	//DEFAULT CONSTRUCTOR
	Alignment();
	//DESTRUCTOR
	~Alignment();

	// RETURNS A POINT ON THE CURVE
	CRAB::Vector4Df getPosition(const float& t) const;
	// RETURNS THE CURVE TANGENT
	CRAB::Vector4Df getTan(const float& t) const;
	// RETURNS THE CURVE NORMAL
	CRAB::Vector4Df getNormal(const float& t) const;
	// RETURNS THE CURVE NORMAL UP (Yaw vector)
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

	// Return the closest collision distance of a ray and the segment
	CRAB::Vector4Df Collision(const Ray& ray) const;
	// Return true if the point P intersect the segment
	bool Contains(const CRAB::Vector4Df& p) const;

	CRAB::Vector4Df getPointFromStation(float dist);
	float getStationFromPoint(CRAB::Vector4Df p);
};

#endif // ALIGNMENT_H