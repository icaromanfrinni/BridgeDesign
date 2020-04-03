#include "Bridge.h"

//DEFAULT CONSTRUCTOR
Bridge::Bridge()
{
}

//OVERLOAD CONSTRUCTOR
Bridge::Bridge(const std::string& _name, Road* _road, const float& cross_station, const float& vertical_clearance, const float& horizontal_clearance)
	: name(_name), road(_road), CS(cross_station), VC(vertical_clearance), HC(horizontal_clearance)
{
	// -----------------
	// Bridge attributes
	// -----------------

	this->mainSpan = 35.0f;
	this->B = this->road->width + 2 * GUARD_RAIL;
	this->H = int((100.0f * this->mainSpan / 16.0f) / 5.0f) * 0.05f;

	// --------------------
	// CREST VERTICAL CURVE
	// --------------------

	// Length of crest vertical curve
	float Lc = this->HC;

	// Algebraic difference in grades (%)
	float A;
	if (this->road->S < Lc)
		A = Lc * 100 * (powf(sqrtf(2 * h1) + sqrtf(2 * h2), 2.0f)) / powf(this->road->S, 2.0f);
	else
		A = (200 * powf(sqrtf(h1) + sqrtf(h2), 2.0f)) / (2.0f * this->road->S - Lc);

	A = 100.0f;

	// VPC
	CRAB::Vector4Df VPC2 = this->road->alignment.getPointFromStation(this->CS - Lc / 2);
	VPC2.y += this->VC + this->H;
	// VPI
	CRAB::Vector4Df VPI2 = this->road->alignment.getPointFromStation(this->CS);
	VPI2.y += this->VC + this->H + (A / 200) * Lc / 2;
	// VPT
	CRAB::Vector4Df VPT2 = this->road->alignment.getPointFromStation(this->CS + Lc / 2);
	VPT2.y += this->VC + this->H;

	// ------------------
	// SAG VERTICAL CURVE
	// ------------------

	// Length of sag vertical curve (S > L)
	float Ls = 2 * this->road->S - ((120 + 3.5 * this->road->S) / (A / 2));
	Ls = int(round(Ls / 5)) * 5;

	// Grades
	Ray g1, g2;
	g1.origin = VPC2;
	g1.direction = (VPC2 - VPI2).to_unitary();
	g2.origin = VPT2;
	g2.direction = (VPT2 - VPI2).to_unitary();

	// Tangents
	CRAB::Vector4Df tan_L, tan_R;

	// Horizontal Matrix
	CRAB::Matrix4 PlaneXZ = CRAB::Matrix4{
			1, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1 };

	// Length of projection
	float x1, x2;

	// --------
	//   Left
	// --------

	// VPI
	CRAB::Vector4Df VPI1;
	for (int i = 0; i < this->road->alignment.segments.size(); i++)
	{
		// get the intersect point
		CRAB::Vector4Df P = this->road->alignment.segments[i]->Collision(g1);
		// check if its between the extreme points
		if (this->road->alignment.segments[i]->Contains(P))
		{
			VPI1 = P;
			tan_L = this->road->alignment.segments[i]->getTan(0.0f);
			break;
		}
		// if its before all segments
		if (i == 0)
		{
			VPI1 = P;
			tan_L = this->road->alignment.segments[i]->getTan(0.0f);
		}
	}
	// Projections

	// VPC
	CRAB::Vector4Df VPC1;
	// VPT
	CRAB::Vector4Df VPT1;

	// ---------
	//   Right
	// ---------

	// VPI
	CRAB::Vector4Df VPI3;
	for (int i = 0; i < this->road->alignment.segments.size(); i++)
	{
		// get the intersect point
		CRAB::Vector4Df P = this->road->alignment.segments[i]->Collision(g2);
		// check if its between the extreme points
		if (this->road->alignment.segments[i]->Contains(P))
		{
			VPI3 = P;
			tan_R = this->road->alignment.segments[i]->getTan(1.0f);
			break;
		}
		// if its after all segments
		VPI3 = P;
		tan_R = this->road->alignment.segments[i]->getTan(1.0f);
	}
	// Projections

	// VPC
	CRAB::Vector4Df VPC3;
	// VPT
	CRAB::Vector4Df VPT3;

	// ---------
	// ALIGNMENT
	// ---------

	//this->alignment.segments.push_back(new CircularArc(VPC1, VPI1, VPT1));
	//this->alignment.segments.push_back(new Line(VPT1, VPC2));
	this->alignment.segments.push_back(new Line(VPI1, VPC2));
	this->alignment.segments.push_back(new CircularArc(VPC2, VPI2, VPT2));
	this->alignment.segments.push_back(new Line(VPT2, VPI3));
	//this->alignment.segments.push_back(new Line(VPT2, VPC3));
	//this->alignment.segments.push_back(new CircularArc(VPC3, VPI3, VPT3));
}

//DESTRUCTOR
Bridge::~Bridge()
{
}