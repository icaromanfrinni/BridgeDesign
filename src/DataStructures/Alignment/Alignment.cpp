#include "Alignment.h"

// DEFAULT CONSTRUCTOR
// -------------------
Alignment::Alignment()
{

}
// DESTRUCTOR
// ----------
Alignment::~Alignment()
{

}

int Alignment::binomialCoefficient(const int& n, const int& i) const
{
	if (i == 0 || i == n)
		return 1;
	return binomialCoefficient(n - 1, i - 1) + binomialCoefficient(n - 1, i);
}
float Alignment::BernsteinPolynomial(const int& n, const int& i, const float& t) const
{
	if (i < 0 || i > n)
		return 0;
	int C = binomialCoefficient(n, i);
	return C * powf(t, i) * powf(1 - t, n - i);
}

// RETURNS A POINT ON THE CURVE
// ----------------------------
CRAB::Vector4Df Alignment::getPosition(const float& t) const
{
	CRAB::Vector4Df position = { 0.0f, 0.0f, 0.0f, 1.0f };
	int n = points.size() - 1;
	for (int i = 0; i < points.size(); i++)
	{
		position += points[i] * BernsteinPolynomial(n, i, t);
	}
	return position;
}
// RETURNS THE CURVE TANGENT
// -------------------------
CRAB::Vector4Df Alignment::getTan(const float& t) const
{
	CRAB::Vector4Df tan_vector = { 0.0f, 0.0f, 0.0f, 1.0f };
	int n = points.size() - 1;
	for (int i = 0; i < points.size(); i++)
	{
		tan_vector += points[i] * (n * (BernsteinPolynomial(n - 1, i - 1, t) - BernsteinPolynomial(n - 1, i, t)));
	}
	return tan_vector.to_unitary();
}
// RETURNS THE CURVE NORMAL
// ------------------------
CRAB::Vector4Df Alignment::getNormal(const float& t) const
{
	CRAB::Vector4Df up = { 0.0f, 1.0f, 0.0f, 0.0f };
	CRAB::Vector4Df i = cross(up, this->getTan(t)).to_unitary();
	return cross(this->getTan(t), i).to_unitary();
}
// RETURNS THE CURVE LENGTH
// ------------------------
float Alignment::getLength() const
{

}

// Return Point from Station
// -------------------------
CRAB::Vector4Df Alignment::getPointFromStation(float dist)
{
	CRAB::Vector4Df vPxz = this->getPosition(1.0f) - this->getPosition(0.0f);
	vPxz.y = 0.0f;
	float t = dist / vPxz.length();
	if (t <= 1.0f)
		return this->getPosition(t);
	else dist -= vPxz.length();

	return getPosition(t);
}

// Return Station from Point
// -------------------------
float Alignment::getStationFromPoint(CRAB::Vector4Df p)
{
	CRAB::Vector4Df v = p - this->getPosition(0.0f);
	v.y = 0.0f;
	return v.length();
}

// RETURN the closest collision distance of a ray and the segment
// --------------------------------------------------------------
CRAB::Vector4Df Alignment::Collision(const Ray& ray) const
{
	CRAB::Vector4Df Up = { 0.0f, 1.0f, 0.0f, 0.0f };

	// FIRST TANGENT
	// -------------

	// normal vector
	CRAB::Vector4Df Right = CRAB::cross(this->getTan(0.0f), Up);
	CRAB::Vector4Df normal = CRAB::cross(Right, this->getTan(0.0f));

	// "t" distance
	float t1 = CRAB::dot(this->getPosition(0.0f) - ray.origin, normal) / CRAB::dot(ray.direction, normal);

	// SECOND TANGENT
	// --------------

	// normal vector
	Right = CRAB::cross(this->getTan(1.0f), Up);
	normal = CRAB::cross(Right, this->getTan(1.0f));

	// "t" distance
	float t2 = CRAB::dot(this->getPosition(1.0f) - ray.origin, normal) / CRAB::dot(ray.direction, normal);

	// Closest collision Point
	// -----------------------

	if (t1 < t2)
		return ray.origin + ray.direction * t1;
	else return ray.origin + ray.direction * t2;
}
// RETURN true if the point P intersect the segment
// ------------------------------------------------
bool Alignment::Contains(const CRAB::Vector4Df& p) const
{
	/*CRAB::Vector4Df e1 = this->getMidPoint() - this->getStartPoint();
	CRAB::Vector4Df e2 = this->getEndPoint() - this->getMidPoint();

	if ((p - this->getStartPoint()).length() <= e1.length() &&
		(p - this->getMidPoint()).length() <= e1.length())
		return true;
	else if ((p - this->getMidPoint()).length() <= e2.length() &&
		(p - this->getEndPoint()).length() <= e2.length())
		return true;
	else return false;*/
	
	return false;
}