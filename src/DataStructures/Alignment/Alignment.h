#pragma once
#include <iostream>
#include <vector>

#include "Linear_Algebra.h"
#include "Segment.h"

class Alignment
{
public:
	std::vector<Segment*> segments;

	//DEFAULT CONSTRUCTOR
	Alignment();
	//DESTRUCTOR
	~Alignment();
};