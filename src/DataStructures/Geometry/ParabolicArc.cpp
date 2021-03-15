#include "ParabolicArc.h"

// DEFAULT CONSTRUCTOR
// -------------------
ParabolicArc::ParabolicArc()
{
}
// OVERLOAD CONSTRUCTOR
// --------------------
ParabolicArc::ParabolicArc(const CRAB::Vector4Df &_p1, const CRAB::Vector4Df &_p2, const CRAB::Vector4Df &_p3)
	: p1(_p1), p2(_p2), p3(_p3)
{
	this->tan1 = (this->p2 - this->p1).to_unitary();
	this->tan2 = (this->p3 - this->p2).to_unitary();
	CRAB::Vector4Df xAxis = { 1.0f, 0.0f, 0.0f, 0.0f };

	this->g1 = tanf(acosf(CRAB::dot(xAxis, this->tan1)));
	if (CRAB::cross(xAxis, this->tan1).z < 0.0f)
		this->g1 *= -1.0f;

	this->g2 = tanf(acosf(CRAB::dot(xAxis, this->tan2)));
	if (CRAB::cross(xAxis, this->tan2).z < 0.0f)
		this->g2 *= -1.0f;
}
// OVERLOAD CONSTRUCTOR (from endpoints)
// -------------------------------------
ParabolicArc::ParabolicArc(const CRAB::Vector4Df& _p1, const CRAB::Vector4Df& _p3, const float& A)
	: p1(_p1), p3(_p3)
{
	float L = this->p3.x - this->p1.x;
	float g3 = (this->p3.y - this->p1.y) / L;
	this->g1 = (A / 200.0f) + g3;
	this->g2 = -(A / 200.0f) + g3;
	
	this->p2 = { 0.0f, 0.0f, 0.0f, 1.0f };
	this->p2.x = this->p1.x + (L / 2.0f);
	this->p2.y = this->p1.y + this->g1 * (L / 2.0f);

	this->tan1 = (this->p2 - this->p1).to_unitary();
	this->tan2 = (this->p3 - this->p2).to_unitary();
}
// OVERLOAD CONSTRUCTOR (from midpoint)
// ------------------------------------
ParabolicArc::ParabolicArc(const CRAB::Vector4Df& _p2, const CRAB::Vector4Df& _tan1, const CRAB::Vector4Df& _tan2, const float& L)
	: p2(_p2), tan1(_tan1), tan2(_tan2)
{
	CRAB::Vector4Df xAxis = { 1.0f, 0.0f, 0.0f, 0.0f };

	this->g1 = tanf(acosf(CRAB::dot(xAxis, this->tan1)));
	if (CRAB::cross(xAxis, this->tan1).z < 0.0f)
		this->g1 *= -1.0f;

	this->g2 = tanf(acosf(CRAB::dot(xAxis, this->tan2)));
	if (CRAB::cross(xAxis, this->tan2).z < 0.0f)
		this->g2 *= -1.0f;

	this->p1 = { 0.0f, 0.0f, 0.0f, 1.0f };
	this->p1.x = this->p2.x - (L / 2.0f);
	this->p1.y = this->p2.y - this->g1 * (L / 2.0f);

	this->p3 = { 0.0f, 0.0f, 0.0f, 1.0f };
	this->p3.x = this->p2.x + (L / 2.0f);
	this->p3.y = this->p2.y + this->g2 * (L / 2.0f);
}
// DESTRUCTOR
// ----------
ParabolicArc::~ParabolicArc()
{
}

// RETURN
// ------
glm::vec3 ParabolicArc::getStartPoint() const {
	return glm::vec3(this->p1.x, this->p1.y, this->p1.z);
}
glm::vec3 ParabolicArc::getMidPoint() const {
	return glm::vec3(this->p2.x, this->p2.y, this->p2.z);
}
glm::vec3 ParabolicArc::getEndPoint() const {
	return glm::vec3(this->p3.x, this->p3.y, this->p3.z);
}
CRAB::Vector4Df ParabolicArc::getStartPoint4D() const {
	return this->p1;
}
CRAB::Vector4Df ParabolicArc::getMidPoint4D() const {
	return this->p2;
}
CRAB::Vector4Df ParabolicArc::getEndPoint4D() const {
	return this->p3;
}

// RETURNS THE SEGMENT LENGTH
// --------------------------
float ParabolicArc::getLength() const
{
	float AC = acosf(CRAB::dot(this->tan1, this->tan2));
	float R = (this->p2 - this->p1).length() / tanf(AC / 2.0f);
	return AC * R;
}

// RETURN THE ELEVATION
// --------------------
float ParabolicArc::getY(const float& x) const
{
	float dX = x - this->p1.x;
	float A = 0.5f * (this->g2 - this->g1) / (this->p3.x - this->p1.x);
	float y = A * powf(dX, 2.0f) + this->g1 * dX + this->p1.y;
	return y;
}

// RETURN THE WEIGHT OF THE CONTROL POINT
// --------------------------------------
float ParabolicArc::midPointWeight() const
{
	return 1.0f;
}

// RETURN THE CLOSEST COLLISION DISTANCE OF A RAY AND THE SEGMENT
// --------------------------------------------------------------
CRAB::Vector4Df ParabolicArc::Collision(const CRAB::Ray& ray) const
{
	CRAB::Vector4Df Up = { 0.0f, 1.0f, 0.0f, 0.0f };

	// FIRST TANGENT
	// -------------
	// tangent vector
	CRAB::Vector4Df tan4D1 = { this->tan1.x, this->tan1.y, this->tan1.z, 0.0f };
	// normal vector
	CRAB::Vector4Df Right = CRAB::cross(tan4D1, Up);
	CRAB::Vector4Df normal = CRAB::cross(Right, tan4D1);
	// "t" distance
	float t1 = CRAB::dot(this->getStartPoint4D() - ray.origin, normal) / CRAB::dot(ray.direction, normal);

	// SECOND TANGENT
	// --------------
	// tangent vector
	CRAB::Vector4Df tan4D2 = { this->tan2.x, this->tan2.y, this->tan2.z, 0.0f };
	// normal vector
	Right = CRAB::cross(tan4D2, Up);
	normal = CRAB::cross(Right, tan4D2);
	// "t" distance
	float t2 = CRAB::dot(this->getEndPoint4D() - ray.origin, normal) / CRAB::dot(ray.direction, normal);

	// Closest collision Point
	// -----------------------
	if (t1 < t2)
		return ray.origin + ray.direction * t1;
	else return ray.origin + ray.direction * t2;
}

// RETURN TRUE IF THE POINT 'P' INTERSECT THE SEGMENT
// --------------------------------------------------
bool ParabolicArc::Contains(const CRAB::Vector4Df& p) const
{
	CRAB::Vector4Df e1 = this->getMidPoint4D() - this->getStartPoint4D();
	CRAB::Vector4Df e2 = this->getEndPoint4D() - this->getMidPoint4D();

	if ((p - this->getStartPoint4D()).length() <= e1.length() &&
		(p - this->getMidPoint4D()).length() <= e1.length())
		return true;
	else if ((p - this->getMidPoint4D()).length() <= e2.length() &&
		(p - this->getEndPoint4D()).length() <= e2.length())
		return true;
	else return false;
}