#pragma once
#ifndef SEGMENT_H
#define SEGMENT_H

#include <iostream>
#include <glm/glm.hpp>
#include "Linear_Algebra.h"

class Segment
{
public:
	// DEFAULT CONSTRUCTOR
	Segment();
	// DESTRUCTOR
	virtual ~Segment() = 0;

	// RETURN
	virtual glm::vec3 getStartPoint() const = 0;
	virtual glm::vec3 getMidPoint() const = 0;
	virtual glm::vec3 getEndPoint() const = 0;
	virtual float getLength() const = 0;
	virtual float getY(const float& x) const = 0;
	virtual float midPointWeight() const = 0;
};

#endif // SEGMENT_H