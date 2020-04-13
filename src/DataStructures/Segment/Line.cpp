#include "Line.h"

// DEFAULT CONSTRUCTOR
Line::Line()
{
}
//OVERLOAD CONSTRUCTOR (from Station)
//Line::Line(const CRAB::Station& _s1, const CRAB::Station& _s2) : s1(_s1), s2(_s2)
//{
//}
//OVERLOAD CONSTRUCTOR (from CRAB::Vector4Df)
Line::Line(const CRAB::Vector4Df& _p1, const CRAB::Vector4Df& _p2) : p1(_p1), p2(_p2)
{
}

// DESTRUCTOR
Line::~Line()
{
}

//ALLOCATION (from Station)
//void Line::setStartStation(const CRAB::Station& s) {
//	s1 = s;
//}
//void Line::setMidStation(const CRAB::Station& s) {
//}
//void Line::setEndStation(const CRAB::Station& s) {
//	s2 = s;
//}
//ALLOCATION (from CRAB::Vector4Df)
void Line::setStartStation(const CRAB::Vector4Df& p) {
	p1 = p;
}
void Line::setMidStation(const CRAB::Vector4Df& p) {
}
void Line::setEndStation(const CRAB::Vector4Df& p) {
	p2 = p;
}

//RETURN START STATION
//CRAB::Station Line::getStartStation() const {
//	return s1;
//}
//RETURN END STATION
//CRAB::Station Line::getEndStation() const {
//	return s2;
//}
//RETURN THE START POINT
CRAB::Vector4Df Line::getStartPoint() const {
	return p1;
}
//RETURN THE END POINT
CRAB::Vector4Df Line::getEndPoint() const {
	return p2;
}
//RETURN THE MIDDLE POINT
CRAB::Vector4Df Line::getMidPoint() const {
	return (p1 + p2) / (p1.length() + p2.length());
}
//RETURN POSITION COORD
CRAB::Vector4Df Line::getPosition(const float &t) const {
	return p1 + (p2 - p1) * t;
}
//RETURN TANGENT VECTOR
CRAB::Vector4Df Line::getTan(const float& t) const {
	return (p2 - p1).to_unitary();
}
//RETURN NORMAL VECTOR
CRAB::Vector4Df Line::getNormal(const float& t) const {
	CRAB::Vector4Df WorldUp = { 0.0f, 1.0f, 0.0f, 0.0f };
	CRAB::Vector4Df Right = cross(getTan(t), WorldUp).to_unitary();
	return cross(Right, getTan(t)).to_unitary();
}
//RETURN BINORMAL VECTOR
CRAB::Vector4Df Line::getBinormal(const float& t) const {
	return cross(getTan(t), getNormal(t)).to_unitary();
}
//RETURN THE LENGTH OF LINE SEGMENT
float Line::getLength() const {
	return (p2 - p1).length();
}

//Return the closest collision distance of a ray and the segment
CRAB::Vector4Df Line::Collision(const Ray& ray) const
{
	// normal vector
	CRAB::Vector4Df Up = { 0.0f, 1.0f, 0.0f, 0.0f };
	CRAB::Vector4Df Right = CRAB::cross(this->getTan(0), Up);
	CRAB::Vector4Df normal = CRAB::cross(Right, this->getTan(0));

	// "t" distance
	float t = CRAB::dot(this->getStartPoint() - ray.origin, normal) / CRAB::dot(ray.direction, normal);

	// Closest collision Point
	return ray.origin + ray.direction * t;
}
//Return true if the point P intersect the segment
bool Line::Contains(const CRAB::Vector4Df& p) const
{
	if ((p - this->getStartPoint()).length() <= this->getLength() &&
		(p - this->getEndPoint()).length() <= this->getLength())
		return true;
	else return false;
}