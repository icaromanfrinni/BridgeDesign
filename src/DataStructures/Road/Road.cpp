#include "Road.h"

//DEFAULT CONSTRUCTOR
Road::Road()
{
}

//OVERLOAD CONSTRUCTOR
Road::Road(const std::string& _name, const float& width, const float& speed) : name(_name), W(width)
{
	// Stopping Sight Distance (S)
	// ---------------------------
	float t = 2.5f;								// brake reaction time
	float d1 = 0.278 * speed * t;				// brake reaction distance
	float a = 3.4f;								// deceleration rate
	float d2 = 0.039 * powf(speed, 2.0f) / a;	// braking distance on level
	this->S = round((d1 + d2) / 10) * 10;
}

//DESTRUCTOR
Road::~Road()
{
}