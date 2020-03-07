#include "Road.h"

//DEFAULT CONSTRUCTOR
Road::Road()
{
}

//OVERLOAD CONSTRUCTOR
Road::Road(const std::string& _name, const float& width) : name(_name), B(width)
{
}

//DESTRUCTOR
Road::~Road()
{
}