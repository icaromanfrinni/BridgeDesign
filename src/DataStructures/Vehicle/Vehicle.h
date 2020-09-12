#pragma once
#ifndef VEHICLE_H
#define VEHICLE_H

#include <iostream>
#include <string>

class Vehicle
{
	
public:
	std::string name;
	float front_overhang;	// front overhang of inner lane vehicle, m
	float wheelbase;		// wheelbase of single unit or tractor, m
	//float u;				// track width on tangent (out-to-out of tires), m

	// DEFAULT CONSTRUCTOR
	Vehicle();
	// OVERLOAD CONSTRUCTOR
	Vehicle(const std::string& _name, const float& _front_overhang, const float& _wheelbase);
	// DESTRUCTOR
	~Vehicle();
};

#endif // VEHICLE_H