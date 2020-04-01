#include "Bridge.h"

//DEFAULT CONSTRUCTOR
Bridge::Bridge()
{
}

//OVERLOAD CONSTRUCTOR
Bridge::Bridge(const std::string& _name, Road* _road, const float& cross_station, const float& vertical_clearance, const float& horizontal_clearance)
	: name(_name), road(_road), CS(cross_station), VC(vertical_clearance), HC(horizontal_clearance)
{
	// Bridge attributes
	this->mainSpan = 35.0f;
	this->B = this->road->width + 2 * GUARD_RAIL;
	this->H = int((100.0f * this->mainSpan / 16.0f) / 5.0f) * 0.05f;

	// CREST VERTICAL CURVE
	// --------------------

	// Length of crest vertical curve
	float L2 = this->HC;

	// Algebraic difference in grades
	float A;
	if (this->road->S < L2)
		A = L2 * 100 * (powf(sqrtf(2 * h1) + sqrtf(2 * h2), 2.0f)) / powf(this->road->S, 2.0f);
	else
		A = (200 * powf(sqrtf(h1) + sqrtf(h2), 2.0f)) / (2.0f * this->road->S - L2);

	float t;
	// VPC
	t = (this->CS - L2 / 2) / this->road->alignment.segments.back()->getLength();
	CRAB::Vector4Df VPC2 = this->road->alignment.segments.back()->getPoint(t);
	VPC2.y += this->VC + this->H;
	// VPI
	t = this->CS / this->road->alignment.segments.back()->getLength();
	CRAB::Vector4Df VPI2 = this->road->alignment.segments.back()->getPoint(t);
	VPI2.y += this->VC + this->H + (A / 200) * L2 / 2;
	// VPT
	t = (this->CS + L2 / 2) / this->road->alignment.segments.back()->getLength();
	CRAB::Vector4Df VPT2 = this->road->alignment.segments.back()->getPoint(t);
	VPT2.y += this->VC + this->H;

	//CircularArc* Crest = new CircularArc(VPC, VPI, VPT);

	// SAG VERTICAL CURVE
	// ------------------

	// Length of vertical curve (S > L)
	float L1 = 2 * this->road->S - ((120 + 3.5 * this->road->S) / (A / 2));
	L1 = int(round(L1 / 5)) * 5;

	// Length of projection
	float x = L1 / (sqrtf(powf(A / 200, 2.0f) + 1) + 1);
	float d = L1 - x;

	// Left
	// ----
	
	// VPI
	t = (this->CS - (L2 / 2 + (this->VC + this->H) / (A / 200))) / this->road->alignment.segments.back()->getLength();
	CRAB::Vector4Df VPI1 = this->road->alignment.segments.back()->getPoint(t);
	// VPC
	CRAB::Vector4Df VPC1 = VPI1 + (VPI1 - this->road->alignment.segments.back()->getPoint(this->CS / this->road->alignment.segments.back()->getLength())).to_unitary() * d;
	// VPT
	CRAB::Vector4Df VPT1 = VPI1 + (VPI2 - VPI1).to_unitary() * d;

	// Right
	// -----

	// VPI
	t = (this->CS + (L2 / 2 + (this->VC + this->H) / (A / 200))) / this->road->alignment.segments.back()->getLength();
	CRAB::Vector4Df VPI3 = this->road->alignment.segments.back()->getPoint(t);
	// VPC
	CRAB::Vector4Df VPC3 = VPI3 + (VPI2 - VPI3).to_unitary() * d;
	// VPT
	CRAB::Vector4Df VPT3 = VPI3 + (VPI3 - this->road->alignment.segments.back()->getPoint(this->CS / this->road->alignment.segments.back()->getLength())).to_unitary() * d;

	// ALIGNMENT
	// ---------

	this->alignment.segments.push_back(new CircularArc(VPC1, VPI1, VPT1));
	this->alignment.segments.push_back(new Line(VPT1, VPC2));
	this->alignment.segments.push_back(new CircularArc(VPC2, VPI2, VPT2));
	this->alignment.segments.push_back(new Line(VPT2, VPC3));
	this->alignment.segments.push_back(new CircularArc(VPC3, VPI3, VPT3));
}

//DESTRUCTOR
Bridge::~Bridge()
{
}