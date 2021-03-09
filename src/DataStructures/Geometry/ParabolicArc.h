#pragma once
#ifndef PARABOLICARC_H
#define PARABOLICARC_H

#include "Geometry.h"

class ParabolicArc
	: public Geometry
{
	glm::vec3 p1, p2, p3;
	glm::vec3 tan1, tan2;
	float g1, g2;

	// SETUP
	void Setup();

public:
	// DEFAULT CONSTRUCTOR
	ParabolicArc();
	// OVERLOAD CONSTRUCTOR (from glm::vec3)
	ParabolicArc(const glm::vec3 &_p1, const glm::vec3 &_p2, const glm::vec3 &_p3);
	// OVERLOAD CONSTRUCTOR (from CRAB::Vector4Df)
	ParabolicArc(const CRAB::Vector4Df &_p1, const CRAB::Vector4Df &_p2, const CRAB::Vector4Df &_p3);
	// DESTRUCTOR
	~ParabolicArc();

	// RETURN (glm::vec3)
	glm::vec3 getStartPoint() const;
	glm::vec3 getMidPoint() const;
	glm::vec3 getEndPoint() const;
	// RETURN (CRAB::Vector4Df)
	CRAB::Vector4Df getStart4DPoint() const;
	CRAB::Vector4Df getMid4DPoint() const;
	CRAB::Vector4Df getEnd4DPoint() const;
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