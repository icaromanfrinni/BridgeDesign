#include "Line.h"

// DEFAULT CONSTRUCTOR
// -------------------
Line::Line()
{
}
// OVERLOAD CONSTRUCTOR (from two points)
// --------------------------------------
Line::Line(const glm::vec3 & _p0, const glm::vec3 & _p1)
	: p0(_p0), p1(_p1)
{
}
// OVERLOAD CONSTRUCTOR (from CRAB::Vector4Df)
// -------------------------------------------
Line::Line(const CRAB::Vector4Df& _p0, const CRAB::Vector4Df& _p1)
{
	this->p0 = glm::vec3(_p0.x, _p0.y, _p0.z);
	this->p1 = glm::vec3(_p1.x, _p1.y, _p1.z);
}
// DESTRUCTOR
// ----------
Line::~Line()
{
}

// RETURN (glm::vec3)
// ------------------
glm::vec3 Line::getStartPoint() const
{
	return p0;
}
glm::vec3 Line::getMidPoint() const
{
	return (p0 + p1) / 2.0f;
}
glm::vec3 Line::getEndPoint() const
{
	return p1;
}

// RETURN (CRAB::Vector4Df)
// ------------------------
CRAB::Vector4Df Line::getStart4DPoint() const
{
	return { p0.x, p0.y, p0.z, 1.0f };
}
CRAB::Vector4Df Line::getMid4DPoint() const
{
	return { this->getMidPoint().x, this->getMidPoint().y, this->getMidPoint().z, 1.0f };
}
CRAB::Vector4Df Line::getEnd4DPoint() const
{
	return { p1.x, p1.y, p1.z, 1.0f };
}

// RETURNS THE SEGMENT LENGTH
// --------------------------
float Line::getLength() const
{
	return glm::distance(p0, p1);
}

// RETURN THE ELEVATION
// --------------------
float Line::getY(const float& x) const
{
	float m = (this->getEndPoint().y - this->getStartPoint().y) / (this->getEndPoint().x - this->getStartPoint().x);
	float y = m * (x - this->getStartPoint().x) + this->getStartPoint().y;
	return y;
}

// RETURN THE WEIGHT OF THE CONTROL POINT
// --------------------------------------
float Line::midPointWeight() const
{
	return 1.0f;
}