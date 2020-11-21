#include "Vehicle.h"

// DEFAULT CONSTRUCTOR
// -------------------
Vehicle::Vehicle()
{
}

// OVERLOAD CONSTRUCTOR
// --------------------
Vehicle::Vehicle(const std::string& _name, const float& _width, const float& _front_overhang, const std::vector<float>& _wheelbase)
	: name(_name), u(_width), A(_front_overhang), L(_wheelbase)
{
}

// DESTRUCTOR
// ----------
Vehicle::~Vehicle()
{
}