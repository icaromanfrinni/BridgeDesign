#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>

#include "Bridge.h"

class Model
{
	Bridge* bridge;

public:
	std::string name;

	//DEFAULT CONSTRUCTOR
	Model();
	//OVERLOAD CONSTRUCTOR
	Model(const std::string &_name, Bridge* _bridge);

	//DESTRUCTOR
	~Model();

	//RETURN THE NAME OF ALIGNMENT
	std::string getName() const;
	//RETURN THE TYPE OF BRIDGE
	Bridge* getBridge() const;
};

#endif // MODEL_H