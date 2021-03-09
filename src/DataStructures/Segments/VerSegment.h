#pragma once
#ifndef VERSEGMENT_H
#define VERSEGMENT_H

#include <iostream>
#include <glm/glm.hpp>
#include "Linear_Algebra.h"
#include "Geometry.h"

class VerSegment
{
public:
	Geometry* segment;

	// DEFAULT CONSTRUCTOR
	VerSegment();
	// OVERLOAD CONSTRUCTOR (Straight Line)
	VerSegment(const glm::vec3& _p0, const glm::vec3& _p1);
	// OVERLOAD CONSTRUCTOR (Straight Line from CRAB::Vector)
	VerSegment(const CRAB::Vector4Df& _p0, const CRAB::Vector4Df& _p1);
	// OVERLOAD CONSTRUCTOR (Parabolic Arc)
	VerSegment(const glm::vec3& _p0, const glm::vec3& _p1, const glm::vec3& _p2);
	// OVERLOAD CONSTRUCTOR (Parabolic Arc from CRAB::Vector)
	VerSegment(const CRAB::Vector4Df& _p0, const CRAB::Vector4Df& _p1, const CRAB::Vector4Df& _p2);
	// DESTRUCTOR
	~VerSegment();

	// RETURN (glm::vec3)
	glm::vec3 getStartPoint() const;
	glm::vec3 getMidPoint() const;
	glm::vec3 getEndPoint() const;
	// RETURN (CRAB::Vector4Df)
	CRAB::Vector4Df getStart4DPoint() const;
	CRAB::Vector4Df getMid4DPoint() const;
	CRAB::Vector4Df getEnd4DPoint() const;

	// RETURN THE ELEVATION
	float getY(const float& x) const;
	// RETURN THE CLOSEST COLLISION DISTANCE
	CRAB::Vector4Df Collision(const CRAB::Ray& ray) const;

	bool Contains(const CRAB::Vector4Df& p) const;
};

#endif // VERSEGMENT_H