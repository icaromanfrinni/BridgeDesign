#include "CircularArc.h"

// DEFAULT CONSTRUCTOR
// -------------------
CircularArc::CircularArc()
{
}
// OVERLOAD CONSTRUCTOR
// --------------------
CircularArc::CircularArc(const glm::vec3 &_p0, const glm::vec3 &_p1, const glm::vec3 &_p2)
	: p0(_p0), p1(_p1), p2(_p2)
{
}
// OVERLOAD CONSTRUCTOR (from CRAB::Vector4Df)
// -------------------------------------------
CircularArc::CircularArc(const CRAB::Vector4Df& _p0, const CRAB::Vector4Df& _p1, const CRAB::Vector4Df& _p2)
{
	this->p0 = glm::vec3(_p0.x, _p0.y, _p0.z);
	this->p1 = glm::vec3(_p1.x, _p1.y, _p1.z);
	this->p2 = glm::vec3(_p2.x, _p2.y, _p2.z);
}
// DESTRUCTOR
// ----------
CircularArc::~CircularArc()
{
}

// RETURN (glm::vec3)
// ------------------
glm::vec3 CircularArc::getStartPoint() const
{
	return p0;
}
glm::vec3 CircularArc::getMidPoint() const
{
	return p1;
}
glm::vec3 CircularArc::getEndPoint() const
{
	return p2;
}

// RETURN (CRAB::Vector4Df)
// ------------------------
CRAB::Vector4Df CircularArc::getStart4DPoint() const
{
	return { p0.x, p0.y, p0.z, 1.0f };
}
CRAB::Vector4Df CircularArc::getMid4DPoint() const
{
	return { p1.x, p1.y, p1.z, 1.0f };
}
CRAB::Vector4Df CircularArc::getEnd4DPoint() const
{
	return { p2.x, p2.y, p2.z, 1.0f };
}

// RETURNS THE SEGMENT LENGTH
// --------------------------
float CircularArc::getLength() const
{
	glm::vec3 tan1 = p1 - p0;
	glm::vec3 tan2 = p2 - p1;
	float AC = acosf(glm::dot(glm::normalize(tan1), glm::normalize(tan2)));
	float R = glm::length(tan1) / tanf(AC / 2.0f);
	return AC * R;
}

// RETURN THE ELEVATION
// --------------------
float CircularArc::getY(const float& x) const
{
	glm::vec3 tan1 = this->getMidPoint() - this->getStartPoint();
	glm::vec3 tan2 = this->getEndPoint() - this->getMidPoint();
	float AC = acosf(glm::dot(glm::normalize(tan1), glm::normalize(tan2)));
	float R = glm::length(tan1) / tanf(AC / 2.0f);
	glm::vec3 n = glm::normalize(glm::cross(tan2, tan1));
	glm::vec3 r = glm::normalize(glm::cross(tan1, n));
	glm::vec3 center = this->getStartPoint() + (r * R);

	glm::vec3 z = { 0.0f, 0.0f, 1.0f };
	int a = 1; // crest
	if (glm::dot(n, z) < 0)
		a = -1; // sag

	float y = a * sqrtf(powf(R, 2.0f) - powf(x - center.x, 2.0f)) + center.y;
	return y;
}

// RETURN THE WEIGHT OF THE CONTROL POINT
// --------------------------------------
float CircularArc::midPointWeight() const
{
	glm::vec3 p0p1 = p1 - p0;
	glm::vec3 p0p2 = p2 - p0;
	return glm::dot(glm::normalize(p0p1), glm::normalize(p0p2));
}