#include "CircularArc.h"

// DEFAULT CONSTRUCTOR
CircularArc::CircularArc()
{
}
//OVERLOAD CONSTRUCTOR (from Station)
//CircularArc::CircularArc(const CRAB::Station& _s1, const CRAB::Station& _s2, const CRAB::Station& _s3) : s1(_s1), s2(_s2), s3(_s3)
//{
//}
//OVERLOAD CONSTRUCTOR
CircularArc::CircularArc(const CRAB::Vector4Df& _p1, const CRAB::Vector4Df& _p2, const CRAB::Vector4Df& _p3) : p1(_p1), p2(_p2), p3(_p3)
{
}

// DESTRUCTOR
CircularArc::~CircularArc()
{
}

//ALLOCATION (from Station)
//void CircularArc::setStartStation(const CRAB::Station& s) {
//	s1 = s;
//}
//void CircularArc::setMidStation(const CRAB::Station& s) {
//	s2 = s;
//}
//void CircularArc::setEndStation(const CRAB::Station& s) {
//	s3 = s;
//}
//ALLOCATION (from CRAB::Vector4Df)
void CircularArc::setStartStation(const CRAB::Vector4Df& p) {
	p1 = p;
}
void CircularArc::setMidStation(const CRAB::Vector4Df& p) {
	p2 = p;
}
void CircularArc::setEndStation(const CRAB::Vector4Df& p) {
	p3 = p;
}

//RETURN START STATION
//CRAB::Station CircularArc::getStartStation() const {
//	return s1;
//}
//RETURN END STATION
//CRAB::Station CircularArc::getEndStation() const {
//	return s3;
//}
//RETURN THE START POINT
CRAB::Vector4Df CircularArc::getStartPoint() const {
	return p1;
}
//RETURN THE MIDDLE POINT
CRAB::Vector4Df CircularArc::getMidPoint() const {
	return p2;
}
//RETURN THE END POINT
CRAB::Vector4Df CircularArc::getEndPoint() const {
	return p3;
}
//RETURN POSITION COORD
CRAB::Vector4Df CircularArc::getPoint(const float& t) const
{
	/*CRAB::Vector4Df pos = s1.getPoint() * powf(1.0f - t, 2.0f) +
		s2.getPoint() * 2.0f * (1.0f - t) * t +
		s3.getPoint() * powf(t, 2.0f);*/
	CRAB::Vector4Df pos = p1 * powf(1.0f - t, 2.0f) +
		p2 * 2.0f * (1.0f - t) * t +
		p3 * powf(t, 2.0f);
	return pos;
}
//RETURN TANGENT VECTOR
CRAB::Vector4Df CircularArc::getTan(const float& t) const
{
	/*CRAB::Vector4Df tan = (s2.getPoint() - s1.getPoint()) * 2.0f * (1.0f - t) +
		(s3.getPoint() - s2.getPoint()) * 2.0f * t;*/
	CRAB::Vector4Df tan = (p2 - p1) * 2.0f * (1.0f - t) +
		(p3 - p2) * 2.0f * t;
	return tan.to_unitary();
}
//TODO
float CircularArc::getLength() const
{
	//in Plan
	/*return (s3.getPoint() - s1.getPoint()).length();*/
	return (p3 - p1).length();
}