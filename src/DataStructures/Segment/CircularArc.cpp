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
CRAB::Vector4Df CircularArc::getPosition(const float& t) const
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
//RETURN NORMAL VECTOR
CRAB::Vector4Df CircularArc::getNormal(const float& t) const
{
	return cross(getBinormal(t), getTan(t)).to_unitary();
}
//RETURN BINORMAL VECTOR
CRAB::Vector4Df CircularArc::getBinormal(const float& t) const {
	//CRAB::Vector4Df d2 = (s3.getPoint() - (s2.getPoint() * 2.0f) + s1.getPoint()) * 2.0f;
	CRAB::Vector4Df d2 = (p3 - (p2 * 2.0f) + p1) * 2.0f;
	return cross(getTan(t), d2).to_unitary();
}
//TODO better
float CircularArc::getLength() const
{
	float arcLength = 0.0f;
	float t = 0.0f;

	CRAB::Vector4Df A = p1;
	for (int i = 0; i < DIVIDER; i++)
	{
		t += 1.0f / DIVIDER;
		CRAB::Vector4Df B = getPosition(t);
		arcLength += (B - A).length();
		A = B;
	}

	return arcLength;
}

//RETURN the closest collision distance of a ray and the segment
CRAB::Vector4Df CircularArc::Collision(const Ray& ray) const
{
	CRAB::Vector4Df Up = { 0.0f, 1.0f, 0.0f, 0.0f };

	// FIRST TANGENT
	// -------------

	// normal vector
	CRAB::Vector4Df Right = CRAB::cross(this->getTan(0.0f), Up);
	CRAB::Vector4Df normal = CRAB::cross(Right, this->getTan(0.0f));

	// "t" distance
	float t1 = CRAB::dot(this->getStartPoint() - ray.origin, normal) / CRAB::dot(ray.direction, normal);

	// SECOND TANGENT
	// --------------

	// normal vector
	Right = CRAB::cross(this->getTan(1.0f), Up);
	normal = CRAB::cross(Right, this->getTan(1.0f));

	// "t" distance
	float t2 = CRAB::dot(this->getEndPoint() - ray.origin, normal) / CRAB::dot(ray.direction, normal);

	// Closest collision Point
	// -----------------------

	if (t1 < t2)
		return ray.origin + ray.direction * t1;
	else return ray.origin + ray.direction * t2;
}
//RETURN true if the point P intersect the segment
bool CircularArc::Contains(const CRAB::Vector4Df& p) const
{
	CRAB::Vector4Df e1 = this->getMidPoint() - this->getStartPoint();
	CRAB::Vector4Df e2 = this->getEndPoint() - this->getMidPoint();

	if ((p - this->getStartPoint()).length() <= e1.length() &&
		(p - this->getMidPoint()).length() <= e1.length())
		return true;
	else if ((p - this->getMidPoint()).length() <= e2.length() &&
		(p - this->getEndPoint()).length() <= e2.length())
		return true;
	else return false;
}