#pragma once
#ifndef PARABOLICARC_H
#define PARABOLICARC_H

#include "Geometry.h"

class ParabolicArc
	: public Geometry
{
	CRAB::Vector4Df p1, p2, p3;
	CRAB::Vector4Df tan1, tan2;
	float g1, g2;

	// SETUP
	void Setup();

public:
	// DEFAULT CONSTRUCTOR
	ParabolicArc();
	// OVERLOAD CONSTRUCTOR
	ParabolicArc(const CRAB::Vector4Df &_p1, const CRAB::Vector4Df &_p2, const CRAB::Vector4Df &_p3);
	// DESTRUCTOR
	~ParabolicArc();

	// RETURN
	glm::vec3 getStartPoint() const;
	glm::vec3 getMidPoint() const;
	glm::vec3 getEndPoint() const;
	CRAB::Vector4Df getStartPoint4D() const;
	CRAB::Vector4Df getMidPoint4D() const;
	CRAB::Vector4Df getEndPoint4D() const;
	// RETURNS THE SEGMENT LENGTH
	float getLength() const;
	// RETURN THE ELEVATION
	float getY(const float& x) const;
	// RETURN THE WEIGHT OF THE CONTROL POINT
	float midPointWeight() const;

	CRAB::Vector4Df Collision(const CRAB::Ray& ray) const;
	bool Contains(const CRAB::Vector4Df& p) const;
};

#endif // PARABOLICARC_H