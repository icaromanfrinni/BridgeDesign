#pragma once
#ifndef LINE_H
#define LINE_H

#include "Segment.h"

// Linear Bézier curves
class Line : public Segment
{
	CRAB::Station s1, s2;

public:
	//DEFAULT CONSTRUCTOR
	Line();
	//OVERLOAD CONSTRUCTOR (from Station)
	Line(const CRAB::Station& _s1, const CRAB::Station& _s2);
	//OVERLOAD CONSTRUCTOR (from CRAB::Vector4Df)
	Line(const CRAB::Vector4Df& _p1, const CRAB::Vector4Df& _p2);

	//DESTRUCTOR
	~Line();

	//ALLOCATION (from Station)
	void setStartStation(const CRAB::Station& s);
	void setMidStation(const CRAB::Station& s);
	void setEndStation(const CRAB::Station& s);
	//ALLOCATION (from CRAB::Vector4Df)
	void setStartStation(const CRAB::Vector4Df& p);
	void setMidStation(const CRAB::Vector4Df& p);
	void setEndStation(const CRAB::Vector4Df& p);

	//Return STATIONS
	CRAB::Station getStartStation() const;
	CRAB::Station getEndStation() const;
	//Return POINTS
	CRAB::Vector4Df getStartPoint() const;
	CRAB::Vector4Df getEndPoint() const;
	CRAB::Vector4Df getMidPoint() const;
	CRAB::Vector4Df getPosition(const float &t) const;
	CRAB::Vector4Df getTan(const float& t) const;
	float getLength() const;
};

#endif // LINE_H