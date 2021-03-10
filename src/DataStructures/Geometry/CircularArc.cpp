#include "CircularArc.h"

// DEFAULT CONSTRUCTOR
// -------------------
CircularArc::CircularArc()
{
}
// OVERLOAD CONSTRUCTOR
// --------------------
CircularArc::CircularArc(const CRAB::Vector4Df& _p1, const CRAB::Vector4Df& _p2, const CRAB::Vector4Df& _p3)
	: p1(_p1), p2(_p2), p3(_p3)
{
}
// DESTRUCTOR
// ----------
CircularArc::~CircularArc()
{
}

// RETURN
// ------
glm::vec3 CircularArc::getStartPoint() const {
	return glm::vec3(p1.x, p1.y, p1.z);
}
glm::vec3 CircularArc::getMidPoint() const {
	return glm::vec3(p2.x, p2.y, p2.z);
}
glm::vec3 CircularArc::getEndPoint() const {
	return glm::vec3(p3.x, p3.y, p3.z);
}
CRAB::Vector4Df CircularArc::getStartPoint4D() const {
	return this->p1;
}
CRAB::Vector4Df CircularArc::getMidPoint4D() const {
	return this->p2;
}
CRAB::Vector4Df CircularArc::getEndPoint4D() const {
	return this->p3;
}

// RETURNS THE SEGMENT LENGTH
// --------------------------
float CircularArc::getLength() const
{
	CRAB::Vector4Df tan1 = this->p2 - this->p1;
	CRAB::Vector4Df tan2 = this->p3 - this->p2;
	float AC = acosf(CRAB::dot(tan1.to_unitary(), tan2.to_unitary()));
	float R = tan1.length() / tanf(AC / 2.0f);
	return AC * R;
}

// RETURN THE ELEVATION
// --------------------
float CircularArc::getY(const float& x) const
{
	CRAB::Vector4Df tan1 = this->p2 - this->p1;
	CRAB::Vector4Df tan2 = this->p3 - this->p2;
	float AC = acosf(CRAB::dot(tan1.to_unitary(), tan2.to_unitary()));
	float R = tan1.length() / tanf(AC / 2.0f);
	CRAB::Vector4Df n = CRAB::cross(tan2, tan1).to_unitary();
	CRAB::Vector4Df r = CRAB::cross(tan1, n).to_unitary();
	CRAB::Vector4Df center = this->p1 + (r * R);

	CRAB::Vector4Df z = { 0.0f, 0.0f, 1.0f };
	int a = 1; // crest
	if (CRAB::dot(n, z) < 0)
		a = -1; // sag

	float y = a * sqrtf(powf(R, 2.0f) - powf(x - center.x, 2.0f)) + center.y;
	return y;
}

// RETURN THE WEIGHT OF THE CONTROL POINT
// --------------------------------------
float CircularArc::midPointWeight() const
{
	CRAB::Vector4Df p1p2 = (this->p2 - this->p1).to_unitary();
	CRAB::Vector4Df p1p3 = (this->p3 - this->p1).to_unitary();
	return CRAB::dot(p1p2, p1p3);
}

// RETURN THE CLOSEST COLLISION DISTANCE OF A RAY AND THE SEGMENT
// --------------------------------------------------------------
CRAB::Vector4Df CircularArc::Collision(const CRAB::Ray& ray) const
{
	CRAB::Vector4Df Up = { 0.0f, 1.0f, 0.0f, 0.0f };

	// FIRST TANGENT
	// -------------
	// tangent vector
	CRAB::Vector4Df tan1 = (this->p2 - this->p1).to_unitary();
	// normal vector
	CRAB::Vector4Df Right = CRAB::cross(tan1, Up);
	CRAB::Vector4Df normal = CRAB::cross(Right, tan1);
	// "t" distance
	float t1 = CRAB::dot(this->p1 - ray.origin, normal) / CRAB::dot(ray.direction, normal);

	// SECOND TANGENT
	// --------------
	// tangent vector
	CRAB::Vector4Df tan2 = (this->p3 - this->p2).to_unitary();
	// normal vector
	Right = CRAB::cross(tan2, Up);
	normal = CRAB::cross(Right, tan2);
	// "t" distance
	float t2 = CRAB::dot(this->p3 - ray.origin, normal) / CRAB::dot(ray.direction, normal);

	// Closest collision Point
	// -----------------------
	if (t1 < t2)
		return ray.origin + ray.direction * t1;
	else return ray.origin + ray.direction * t2;
}

// RETURN TRUE IF THE POINT 'P' INTERSECT THE SEGMENT
// --------------------------------------------------
bool CircularArc::Contains(const CRAB::Vector4Df& p) const
{
	CRAB::Vector4Df e1 = this->p2 - this->p1;
	CRAB::Vector4Df e2 = this->p3 - this->p2;

	if ((p - this->p1).length() <= e1.length() &&
		(p - this->p2).length() <= e1.length())
		return true;
	else if ((p - this->p2).length() <= e2.length() &&
		(p - this->p3).length() <= e2.length())
		return true;
	else return false;
}