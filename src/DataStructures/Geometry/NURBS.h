#pragma once

// RATIONAL B-SPLINE CURVES
// Reference:
/*
	Piegl, Les. Tiller, Wayne.
	The NURBS Book
	Chapter 4
	Springer, 1997
*/

#include <vector>
#include <glm/glm.hpp>

#include "Linear_Algebra.h"
#include "HorSegment.h"

#define ELEMENTS 100

struct ArcLength
{
	float t, s;
	// CONSTRUCTOR
	ArcLength(const float& _t, const float& _s)
		: t(_t), s(_s)
	{	}
};

class NURBS
{
private:
	// B-SPLINE BASIS FUNCTIONS
	float N(const int& i, const int& p, const float& t) const;
	float dN(const int& i, const int& p, const float& t) const;
	float dN2(const int& i, const int& p, const float& t) const;
	// DERIVATIVES
	glm::vec3 deriv(const float& t) const;
	glm::vec3 deriv2(const float& t) const;
	CRAB::Vector4Df deriv_4D(const float& t) const;
	// FIND THE ith KNOT SPAN
	int FindSpan(const float& t) const;
	// SETUP THE ARC LENGTH TABLE
	void SetupArcLengthTable();

public:
	// Control Points
	std::vector<glm::vec3> points;
	// Weights
	std::vector<float> w;
	// Degree of the basis function
	int P;
	// Knot vector
	std::vector<float> T;
	// Arc Length table
	std::vector<ArcLength> arcLength_table;

	// DEFAULT CONSTRUCTOR
	NURBS();
	// OVERLOAD CONSTRUCTOR (from Points)
	NURBS(const std::vector<glm::vec3>& _points);
	// OVERLOAD CONSTRUCTOR (from HorSegments)
	NURBS(const std::vector<HorSegment*>& segments);
	// DESTRUCTOR
	~NURBS();

	// RETURNS A POINT ON THE CURVE
	glm::vec3 getPosition(const float& t) const;
	// RETURNS THE CURVE TANGENT
	glm::vec3 getTangent(const float& t) const;
	// RETURNS THE CURVE NORMAL
	glm::vec3 getNormal(const float& t) const;
	// RETURNS THE CURVE NORMAL UP (Yaw vector)
	glm::vec3 getNormalUp(const float& t) const;
	// RETURNS THE CURVE BINORMAL
	glm::vec3 getBinormal(const float& t) const;
	// RETURNS THE CURVATURE
	float getCurvature(const float& t) const;
	// RETURNS THE RADIUS OF CURVATURE
	float getRadius(const float& t) const;
	// RETURNS THE CHORD LENGTH OF CURVE
	float getChordLength() const;
	// RETURNS THE ARC LENGTH OF CURVE
	float getArcLength() const;
	// RETURNS THE DISTANCE ALONG THE CURVE (from Arclength Table)
	float getDistance(const float& t) const;
	// RETURNS THE PARAMETER FROM ARC LENGTH
	float getParameter(const float& length) const;
	// RETURNS THE HORIZONTAL DISTANCE FROM START
	float getHorDistance(const float& t) const;
	// CLOCKWISE CHECK
	bool isClockwise(const float& t) const;
};