#include "Line.h"

// DEFAULT CONSTRUCTOR
Line::Line()
{
}
//OVERLOAD CONSTRUCTOR (from Station)
Line::Line(const CRAB::Station& _s1, const CRAB::Station& _s2) : s1(_s1), s2(_s2)
{
}
//OVERLOAD CONSTRUCTOR (from CRAB::Vector4Df)
Line::Line(const CRAB::Vector4Df& _p1, const CRAB::Vector4Df& _p2) {
	s1.setStation(_p1);
	s2.setStation(_p2);
}

// DESTRUCTOR
Line::~Line()
{
}

//ALLOCATION (from Station)
void Line::setStartStation(const CRAB::Station& s) {
	s1 = s;
}
void Line::setMidStation(const CRAB::Station& s) {
}
void Line::setEndStation(const CRAB::Station& s) {
	s2 = s;
}
//ALLOCATION (from CRAB::Vector4Df)
void Line::setStartStation(const CRAB::Vector4Df& p) {
	s1.setStation(p);
}
void Line::setMidStation(const CRAB::Vector4Df& p) {
}
void Line::setEndStation(const CRAB::Vector4Df& p) {
	s2.setStation(p);
}

//RETURN START STATION
CRAB::Station Line::getStartStation() const {
	return s1;
}
//RETURN END STATION
CRAB::Station Line::getEndStation() const {
	return s2;
}
//RETURN THE START POINT
CRAB::Vector4Df Line::getStartPoint() const {
	return s1.getPoint();
}
//RETURN THE END POINT
CRAB::Vector4Df Line::getEndPoint() const {
	return s2.getPoint();
}
//RETURN THE MIDDLE POINT
CRAB::Vector4Df Line::getMidPoint() const {
	return (s1.getPoint() + s2.getPoint()) / (s1.getPoint().length() + s2.getPoint().length());
}
//RETURN POSITION COORD
CRAB::Vector4Df Line::getPosition(const float &t) const {
	return s1.getPoint() + (s2.getPoint() - s1.getPoint()) * t;
}
//RETURN TANGENT VECTOR
CRAB::Vector4Df Line::getTan(const float& t) const {
	return (s2.getPoint() - s1.getPoint()).to_unitary();
}
//RETURN THE LENGTH OF LINE SEGMENT
float Line::getLength() const {
	return (s2.getPoint() - s1.getPoint()).length();
}