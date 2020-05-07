#pragma once

#ifndef ALIGNMENT_H
#define ALIGNMENT_H

#include "NURBS.h"

// MAX Transverse Slope
const float SLOPE_MAX = 0.10f;

class Alignment
{
private:
	int findSegment(const float& station) const;

public:
	std::string name;
	std::vector<Segment*> profile;	// Vertical alignment
	NURBS path2Dh;	// Horizontal alignment
	NURBS path3D;	// 3D alignment

	// DEFAULT CONSTRUCTOR
	Alignment();
	// DEFAULT CONSTRUCTOR
	Alignment(const std::string& _name, const std::vector<Segment*>& horizontal, const std::vector<Segment*>& vertical);
	// DESTRUCTOR
	~Alignment();

	// RETURN
	CRAB::Vector4Df getPosition(const float& t) const;
	CRAB::Vector4Df getTangent(const float& t) const;
	CRAB::Vector4Df getNormal(const float& t) const;
	CRAB::Vector4Df getNormalUp(const float& t) const;
};

#endif