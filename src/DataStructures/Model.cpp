#include "Model.h"

// DEFAULT CONSTRUCTOR
Model::Model()
{

}

// OVERLOAD CONSTRUCTOR
Model::Model(const std::string& _name, Bridge* _bridge) : name(_name), bridge(_bridge)
{

}

// DESTRUCTOR
Model::~Model()
{

}

// RETURN THE NAME
std::string Model::getName() const
{
	return name;
}

//RETURN THE TYPE OF BRIDGE
Bridge* Model::getBridge() const
{
	return bridge;
}