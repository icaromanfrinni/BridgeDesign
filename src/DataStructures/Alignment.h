#pragma once
#ifndef ALIGNMENT_H
#define ALIGNMENT_H

#include <string>
#include <vector>

#include "Road.h"
#include "Segment.h"

class Alignment
{
	Road* road;

public:
	std::string name;
	std::vector<Segment*> segments;

	//DEFAULT CONSTRUCTOR
	Alignment();
	//OVERLOAD CONSTRUCTOR
	Alignment(const std::string& _name, Road* _road);
	Alignment(const std::string& _name, Road* _road, Segment* _segment);

	//DESTRUCTOR
	~Alignment();	

	//ADD NEW SEGMENT
	void AddSegment(Segment* _segment);
	//ALLOCATION
	void SetRoad(Road* _road);

	//RETURN THE CLASSE OF ROAD
	Road* getRoad() const;
};

#endif // ALIGNMENT_H