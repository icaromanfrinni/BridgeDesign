#include "Bridge.h"

//DEFAULT CONSTRUCTOR
Bridge::Bridge()
{
}

//OVERLOAD CONSTRUCTOR
Bridge::Bridge(const std::string& _name, Road* _road, const float& _cross_station, const float& _vertical_clearance, const float& _horizontal_clearance)
	: name(_name), road(_road), cross_station(_cross_station), vertical_clearance(_vertical_clearance), horizontal_clearance(_horizontal_clearance)
{
	// Bridge attributes

	this->mainSpan = 35.0f;
	this->B = this->road->width;
	this->H = int((100.0f * this->mainSpan / 16.0f) / 5.0f) * 0.05f;

	// CREST VERTICAL CURVE
	// --------------------

	// Algebraic difference in grades
	float A;
	if (this->road->S < this->horizontal_clearance)
		A = this->horizontal_clearance * 100 * (powf(sqrtf(2 * h1) + sqrtf(2 * h2), 2.0f)) / powf(this->road->S, 2.0f);
	else
		A = (200 * powf(sqrtf(h1) + sqrtf(h2), 2.0f)) / (2.0f * this->road->S - this->horizontal_clearance);

	// control points
	CRAB::Vector4Df VPC, VPI, VPT;
	// station
	VPI = road->alignment.back()->getPosition(this->cross_station);
	VPC = VPI - road->alignment.back()->getTan(this->cross_station) * (this->horizontal_clearance / 2.0f);
	VPT = VPI + road->alignment.back()->getTan(this->cross_station) * (this->horizontal_clearance / 2.0f);
	// elevation
	VPI.y += this->vertical_clearance + (this->horizontal_clearance / 2.0f) * (A / 200) + this->H;
	VPC.y += this->vertical_clearance + this->H;
	VPT.y += this->vertical_clearance + this->H;

	this->alignment.push_back(new CircularArc(VPC, VPI, VPT));

	// SAG VERTICAL CURVE
	// ------------------
}

//DESTRUCTOR
Bridge::~Bridge()
{
}