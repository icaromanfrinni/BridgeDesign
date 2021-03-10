#include "Line.h"

// DEFAULT CONSTRUCTOR
// -------------------
Line::Line()
{
	this->p1 = { 0.0f, 0.0f, 0.0f, 1.0f };
	this->p2 = { 0.0f, 0.0f, 0.0f, 1.0f };
}
// OVERLOAD CONSTRUCTOR (from two points)
// --------------------------------------
Line::Line(const CRAB::Vector4Df& _p1, const CRAB::Vector4Df& _p2)
	: p1(_p1), p2(_p2)
{
}
// DESTRUCTOR
// ----------
Line::~Line()
{
}

// RETURN
// ------
glm::vec3 Line::getStartPoint() const {
	return glm::vec3(this->p1.x, this->p1.y, this->p1.z);
}
glm::vec3 Line::getMidPoint() const {
	CRAB::Vector4Df midPoint = (this->p1 + this->p2) / 2.0f;
	return glm::vec3(midPoint.x, midPoint.y, midPoint.z);
}
glm::vec3 Line::getEndPoint() const {
	return glm::vec3(this->p2.x, this->p2.y, this->p2.z);
}
CRAB::Vector4Df Line::getStartPoint4D() const {
	return this->p1;
}
CRAB::Vector4Df Line::getMidPoint4D() const {
	return (this->p1 + this->p2) / 2.0f;
}
CRAB::Vector4Df Line::getEndPoint4D() const {
	return this->p2;
}

// RETURNS THE SEGMENT LENGTH
// --------------------------
float Line::getLength() const
{
	return CRAB::distance(p1, p2);
}

// RETURN THE ELEVATION
// --------------------
float Line::getY(const float& x) const
{
	float m = (this->p2.y - this->p1.y) / (this->p2.x - this->p1.x);
	float y = m * (x - this->p1.x) + this->p1.y;
	return y;
}

// RETURN THE WEIGHT OF THE CONTROL POINT
// --------------------------------------
float Line::midPointWeight() const
{
	return 1.0f;
}

// RETURN THE CLOSEST COLLISION DISTANCE OF A RAY AND THE SEGMENT
// --------------------------------------------------------------
CRAB::Vector4Df Line::Collision(const CRAB::Ray& ray) const
{
	// tangent vector
	CRAB::Vector4Df tan = (p2 - p1).to_unitary();
	// normal vector
	CRAB::Vector4Df Up = { 0.0f, 1.0f, 0.0f, 0.0f };
	CRAB::Vector4Df Right = CRAB::cross(tan, Up);
	CRAB::Vector4Df normal = CRAB::cross(Right, tan);
	// "t" distance
	float t = CRAB::dot(this->p1 - ray.origin, normal) / CRAB::dot(ray.direction, normal);
	// Closest collision Point
	return ray.origin + ray.direction * t;
}

// RETURN TRUE IF THE POINT 'P' INTERSECT THE SEGMENT
// --------------------------------------------------
bool Line::Contains(const CRAB::Vector4Df& p) const
{
	if ((p - this->p1).length() <= this->getLength() &&
		(p - this->p2).length() <= this->getLength())
		return true;
	else return false;
}