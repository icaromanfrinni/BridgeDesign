#include "Vehicle.h"

// DEFAULT CONSTRUCTOR
// -------------------
Vehicle::Vehicle()
{
}

// OVERLOAD CONSTRUCTOR
// --------------------
Vehicle::Vehicle(const std::string& _name, const float& _front_overhang, const float& _wheelbase)
	: name(_name), front_overhang(_front_overhang), wheelbase(_wheelbase)
{
}

// DESTRUCTOR
// ----------
Vehicle::~Vehicle()
{
}