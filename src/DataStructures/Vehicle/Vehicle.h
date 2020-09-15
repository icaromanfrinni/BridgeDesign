#pragma once
#ifndef VEHICLE_H
#define VEHICLE_H

#include <iostream>
#include <string>
#include <vector>

class Vehicle
{
	
public:
	std::string name;
	float A;				// front overhang of inner lane vehicle, m
	std::vector<float> L;	// wheelbase of design vehicle between consecutive axles, m
	//float u;				// track width on tangent (out-to-out of tires), m

	// DEFAULT CONSTRUCTOR
	Vehicle();
	// OVERLOAD CONSTRUCTOR
	Vehicle(const std::string& _name, const float& _front_overhang, const std::vector<float>& _wheelbase);
	// DESTRUCTOR
	~Vehicle();
};

#endif // VEHICLE_H