#include "Road.h"

// DEFAULT CONSTRUCTOR
// -------------------
Road::Road()
{
}

// OVERLOAD CONSTRUCTOR
// --------------------
Road::Road(const std::string& _name, const float& _width, const float& _speed) : name(_name), width(_width), speed(_speed)
{
	// Stopping Sight Distance (S)
	// ---------------------------
	float d1 = 0.278 * speed * t;				// brake reaction distance
	float d2 = 0.039 * powf(speed, 2.0f) / a;	// braking distance on level
	S = round((d1 + d2) / 5) * 5;
}

// DESTRUCTOR
// ----------
Road::~Road()
{
}

// ADD NEW CONTROL POINT
// ---------------------
void Road::AddVerPoint(const CRAB::Vector4Df& p)
{
	path2Dv.points.push_back(p);
}
void Road::AddHorPoint(const CRAB::Vector4Df& p)
{
	path2Dh.points.push_back(p);
}