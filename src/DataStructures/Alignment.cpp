#include "Alignment.h"

//DEFAULT CONSTRUCTOR
Alignment::Alignment()
{
}

//OVERLOAD CONSTRUCTOR
Alignment::Alignment(const std::string& _name, Road* _road) : name(_name), road(_road)
{
}

//OVERLOAD CONSTRUCTOR
Alignment::Alignment(const std::string& _name, Road* _road, Segment* _segment) : name(_name), road(_road)
{
	segments.push_back(_segment);
}

//DESTRUCTOR
Alignment::~Alignment()
{
}

//ADD NEW SEGMENT
void Alignment::AddSegment(Segment* _segment)
{
	segments.push_back(_segment);
}

//ALLOCATION
void Alignment::SetRoad(Road* _road)
{
	road = _road;
}

//RETURN THE CLASSE OF ROAD
Road* Alignment::getRoad() const
{
	return road;
}
