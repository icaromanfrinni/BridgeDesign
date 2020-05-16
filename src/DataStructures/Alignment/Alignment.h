#pragma once

#ifndef ALIGNMENT_H
#define ALIGNMENT_H

#include "NURBS.h"
#include "HorSegment.h"
#include "VerSegment.h"

// MAX Transverse Slope
const float SLOPE_MAX = 0.10f;

class Alignment
{
private:
	int findSegment(const float& station) const;
	NURBS path2Dh;	// 2D horizontal curve
	NURBS path3D;	// 3D curve
public:
	std::string name;
	std::vector<HorSegment*> plan;		// Horizontal alignment
	std::vector<VerSegment*> profile;	// Vertical alignment

	// DEFAULT CONSTRUCTOR
	Alignment();
	// DEFAULT CONSTRUCTOR
	Alignment(const std::string& _name, const std::vector<HorSegment*>& _plan, const std::vector<VerSegment*>& _profile);
	// DESTRUCTOR
	~Alignment();

	// RETURN
	CRAB::Vector4Df getPosition(const float& t) const;
	CRAB::Vector4Df getTangent(const float& t) const;
	CRAB::Vector4Df getNormalUp(const float& t) const;
	float getSuperelevation(const float& t, const float& V) const;
};

#endif