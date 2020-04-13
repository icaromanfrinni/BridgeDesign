#pragma once
#ifndef CIRCULARARC_H
#define CIRCULARARC_H

#include "Segment.h"

class CircularArc : public Segment
{
	//CRAB::Station s1, s2, s3;
	CRAB::Vector4Df p1, p2, p3;

public:
	//DEFAULT CONSTRUCTOR
	CircularArc();
	//OVERLOAD CONSTRUCTOR (from Station)
	//CircularArc(const CRAB::Station& _s1, const CRAB::Station& _s2, const CRAB::Station& _s3);
	//OVERLOAD CONSTRUCTOR (from CRAB::Vector4Df)
	CircularArc(const CRAB::Vector4Df& _p1, const CRAB::Vector4Df& _p2, const CRAB::Vector4Df& _p3);

	//DESTRUCTOR
	~CircularArc();

	//ALLOCATION (from Station)
	/*void setStartStation(const CRAB::Station& s);
	void setMidStation(const CRAB::Station& s);
	void setEndStation(const CRAB::Station& s);*/
	//ALLOCATION (from CRAB::Vector4Df)
	void setStartStation(const CRAB::Vector4Df& p);
	void setMidStation(const CRAB::Vector4Df& p);
	void setEndStation(const CRAB::Vector4Df& p);

	//Return STATIONS
	/*CRAB::Station getStartStation() const;
	CRAB::Station getEndStation() const;*/
	//Return POINTS
	CRAB::Vector4Df getStartPoint() const;
	CRAB::Vector4Df getEndPoint() const;
	CRAB::Vector4Df getMidPoint() const;
	CRAB::Vector4Df getPosition(const float& t) const;
	CRAB::Vector4Df getTan(const float& t) const;
	CRAB::Vector4Df getNormal(const float& t) const;
	CRAB::Vector4Df getBinormal(const float& t) const;
	float getLength() const;

	CRAB::Vector4Df Collision(const Ray& ray) const;
	bool Contains(const CRAB::Vector4Df& p) const;
};

#endif // CIRCULARARC_H