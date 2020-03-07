#pragma once
#ifndef BRIDGE_H
#define BRIDGE_H

#include <vector>

#include "HalfEdge.h"
#include "Segment.h"

class Bridge
{
public:
	//DEFAULT CONSTRUCTOR
	Bridge();
	//DESTRUCTOR
	virtual ~Bridge() = 0;

	//MODEL CONSTRUCTOR
	virtual void update() = 0;
	virtual void updateAutoSection() = 0;

	//ALLOCATION
	virtual void setSpan(const float& _span) = 0;
	virtual void setWidth(const float& _B) = 0;
	virtual void setBw(const float& _bw) = 0;
	virtual void setH(const float& _H) = 0;
	
	//return values
	virtual std::vector<HED::solid*> getModel() const = 0;
	virtual std::vector<Segment*> getPath() const = 0;
	virtual float getWidth() const = 0;
	virtual float getSpan() const = 0;
	virtual float getHeight() const = 0;
	virtual float getLb() const = 0;
	virtual float getDeckH() const = 0;
	virtual float getBw() const = 0;
	virtual float getSlabWidth() const = 0;
	virtual float getTH() const = 0;
	virtual float getTV() const = 0;
};

#endif // BRIDGE_H