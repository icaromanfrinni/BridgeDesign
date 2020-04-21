#include "Alignment.h"

// DEFAULT CONSTRUCTOR
// -------------------
Alignment::Alignment()
{

}
// OVERLOAD CONSTRUCTOR
// --------------------
Alignment::Alignment(const CRAB::Curve& _vertical, const CRAB::Curve& _horizontal)
	: VerticalAlignment(_vertical), HorizontalAlignment(_horizontal)
{
	/*for (int i = 0; i <= STEP; i++)
	{
		float t = float(i) / STEP;
		CRAB::Vector4Df control_point = this->HorizontalAlignment.getPosition(t);
		control_point.y = this->VerticalAlignment.getPosition(t).y;
		this->path3D.AddControlPoint(control_point);
	}*/
	this->path3D.points = this->HorizontalAlignment.points;
}
// DESTRUCTOR
// ----------
Alignment::~Alignment()
{

}

// RETURNS A POINT ON THE CURVE
// ----------------------------
CRAB::Vector4Df Alignment::getPosition(const float& t) const
{
	return path3D.getPosition(t);
}
// RETURNS THE CURVE TANGENT
// -------------------------
CRAB::Vector4Df Alignment::getTangent(const float& t) const
{
	return path3D.getTangent(t);
}
// RETURNS THE CURVE NORMAL
// ------------------------
CRAB::Vector4Df Alignment::getNormal(const float& t) const
{
	return path3D.getNormal(t);
}
// RETURNS THE CURVE NORMAL UP (w/ Superelevation)
// -----------------------------------------------
CRAB::Vector4Df Alignment::getNormalUp(const float& t) const
{
	float tan_alfa = 0.0044f * powf(60.0f, 2.0f) / HorizontalAlignment.getRadius(t);
	if (tan_alfa > SLOPE_MAX)
		tan_alfa = SLOPE_MAX;
	float alfa = atanf(tan_alfa) * 180.0f / M_PI;
	if (this->isClockwise(t))
		alfa = alfa * (-1.0f);
	CRAB::Matrix4 R = CRAB::rotateArbitrary(alfa, getTangent(t));

	return (R * path3D.getNormalUp(t)).to_unitary();
}
// RETURNS THE CURVE BINORMAL
// --------------------------
CRAB::Vector4Df Alignment::getBinormal(const float& t) const
{
	return path3D.getBinormal(t);
}
// RETURNS THE CURVATURE
// ---------------------
float Alignment::getCurvature(const float& t) const
{
	return path3D.getCurvature(t);
}
// RETURNS THE RADIUS OF CURVATURE
// -------------------------------
float Alignment::getRadius(const float& t) const
{
	return path3D.getRadius(t);
}
// RETURNS THE CURVE LENGTH
// ------------------------
float Alignment::getLength() const
{
	return path3D.getLength();
}
// CLOCKWISE CHECK
bool Alignment::isClockwise(const float& t) const
{
	return path3D.isClockwise(t);
}

// *********************************************************************

//// Return Point from Station
//// -------------------------
//CRAB::Vector4Df Alignment::getPointFromStation(float dist)
//{
//	CRAB::Vector4Df vPxz = this->getPosition(1.0f) - this->getPosition(0.0f);
//	vPxz.y = 0.0f;
//	float t = dist / vPxz.length();
//	if (t <= 1.0f)
//		return this->getPosition(t);
//	else dist -= vPxz.length();
//
//	return getPosition(t);
//}
//
//// Return Station from Point
//// -------------------------
//float Alignment::getStationFromPoint(CRAB::Vector4Df p)
//{
//	CRAB::Vector4Df v = p - this->getPosition(0.0f);
//	v.y = 0.0f;
//	return v.length();
//}
//
//// RETURN the closest collision distance of a ray and the segment
//// --------------------------------------------------------------
//CRAB::Vector4Df Alignment::Collision(const Ray& ray) const
//{
//	CRAB::Vector4Df Up = { 0.0f, 1.0f, 0.0f, 0.0f };
//
//	// FIRST TANGENT
//	// -------------
//
//	// normal vector
//	CRAB::Vector4Df Right = CRAB::cross(this->getTan(0.0f), Up);
//	CRAB::Vector4Df normal = CRAB::cross(Right, this->getTan(0.0f));
//
//	// "t" distance
//	float t1 = CRAB::dot(this->getPosition(0.0f) - ray.origin, normal) / CRAB::dot(ray.direction, normal);
//
//	// SECOND TANGENT
//	// --------------
//
//	// normal vector
//	Right = CRAB::cross(this->getTan(1.0f), Up);
//	normal = CRAB::cross(Right, this->getTan(1.0f));
//
//	// "t" distance
//	float t2 = CRAB::dot(this->getPosition(1.0f) - ray.origin, normal) / CRAB::dot(ray.direction, normal);
//
//	// Closest collision Point
//	// -----------------------
//
//	if (t1 < t2)
//		return ray.origin + ray.direction * t1;
//	else return ray.origin + ray.direction * t2;
//}
//// RETURN true if the point P intersect the segment
//// ------------------------------------------------
//bool Alignment::Contains(const CRAB::Vector4Df& p) const
//{
//	/*CRAB::Vector4Df e1 = this->getMidPoint() - this->getStartPoint();
//	CRAB::Vector4Df e2 = this->getEndPoint() - this->getMidPoint();
//
//	if ((p - this->getStartPoint()).length() <= e1.length() &&
//		(p - this->getMidPoint()).length() <= e1.length())
//		return true;
//	else if ((p - this->getMidPoint()).length() <= e2.length() &&
//		(p - this->getEndPoint()).length() <= e2.length())
//		return true;
//	else return false;*/
//	
//	return false;
//}