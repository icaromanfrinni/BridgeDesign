#pragma once

// BÉZIER CURVE
// Reference:
/*
	Mortenson, Michael E.
	GEOMETRIC MODELING (3.ed)
	Chapters 4 and 12
	Industrial Press Inc., 2006
*/

#include <vector>
#include "Linear_Algebra.h"

class Bezier
{
private:
	// BASIS FUNCTIONS
	int binomialCoefficient(const int& n, const int& i) const;
	float BernsteinPolynomial(const int& n, const int& i, const float& t) const;

	// CALCULATES THE DERIVATIVE FOR A CURVE OF ORDER 'n'
	// Reference: https://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/Bezier/bezier-der.html
	CRAB::Vector4Df deriv(float t) const;
	CRAB::Vector4Df deriv2(float t) const;

public:
	// Control Points
	std::vector<CRAB::Vector4Df> points;
	
	// DEFAULT CONSTRUCTOR
	Bezier();
	// DESTRUCTOR
	~Bezier();

	// RETURNS A POINT ON THE CURVE
	CRAB::Vector4Df getPosition(const float& t) const;
	// RETURNS THE CURVE TANGENT
	CRAB::Vector4Df getTangent(const float& t) const;
	// RETURNS THE CURVE NORMAL
	CRAB::Vector4Df getNormal(const float& t) const;
	// RETURNS THE CURVE NORMAL UP (Yaw vector)
	CRAB::Vector4Df getNormalUp(const float& t) const;
	// RETURNS THE CURVE BINORMAL
	CRAB::Vector4Df getBinormal(const float& t) const;
	// RETURNS THE CURVATURE
	float getCurvature(const float& t) const;
	// RETURNS THE RADIUS OF CURVATURE
	float getRadius(const float& t) const;
	// CLOCKWISE CHECK
	bool isClockwise(const float& t) const;
};