#pragma once
#ifndef SEGMENT_H
#define SEGMENT_H

#include <iostream>

#include "Linear_Algebra.h"
//#include "Coord_System.h"

const int DIVIDER = 50;

struct Ray
{
	CRAB::Vector4Df origin, direction;
};

class Segment
{
public:
	//DEFAULT CONSTRUCTOR
	Segment();
	//DESTRUCTOR
	virtual ~Segment() = 0;

	//ALLOCATION (from Station)
	/*virtual void setStartStation(const CRAB::Station& s) = 0;
	virtual void setMidStation(const CRAB::Station& s) = 0;
	virtual void setEndStation(const CRAB::Station& s) = 0;*/
	//ALLOCATION (from CRAB::Vector4Df)
	virtual void setStartStation(const CRAB::Vector4Df& p) = 0;
	virtual void setMidStation(const CRAB::Vector4Df& p) = 0;
	virtual void setEndStation(const CRAB::Vector4Df& p) = 0;

	//Return STATIONS
	/*virtual CRAB::Station getStartStation() const = 0;
	virtual CRAB::Station getEndStation() const = 0;*/
	//Return POINTS
	virtual CRAB::Vector4Df getStartPoint() const = 0;
	virtual CRAB::Vector4Df getMidPoint() const = 0;
	virtual CRAB::Vector4Df getEndPoint() const = 0;
	virtual CRAB::Vector4Df getPosition(const float &t) const = 0;
	virtual CRAB::Vector4Df getTan(const float& t) const = 0;
	virtual CRAB::Vector4Df getNormal(const float& t) const = 0;
	virtual CRAB::Vector4Df getBinormal(const float& t) const = 0;
	virtual float getLength() const = 0;
	
	//Return the closest collision distance of a ray and the segment
	virtual CRAB::Vector4Df Collision(const Ray& ray) const = 0;
	//Return true if the point P intersect the segment
	virtual bool Contains(const CRAB::Vector4Df& p) const = 0;
};

#endif // SEGMENT_H