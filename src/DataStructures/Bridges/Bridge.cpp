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
	// -----------------

	this->mainSpan = 35.0f;
	this->B = this->road->width + 2 * GUARD_RAIL;
	this->H = int((100.0f * this->mainSpan / 16.0f) / 5.0f) * 0.05f;

	// Horizontal Alignment
	// --------------------
	this->path2Dh = this->road->path2Dh;
	
	// Vertical Alignment
	// ------------------
	//Vertical_Alignment();
	this->path2Dv = this->road->path2Dv;

	// 3D Alignment
	// ------------
	//this->path3D = this->road->path3D;
	this->path3D = this->road->path2Dh;
}

//DESTRUCTOR
Bridge::~Bridge()
{
}

// VERTICAL ALIGNMENT
void Bridge::Vertical_Alignment()
{
	// ********************************** CREST VERTICAL CURVE **********************************

	// Length of crest vertical curve
	float Lc = this->HC;

	// Algebraic difference in grades (%)
	float A;
	if (this->road->S < Lc)
		A = Lc * 100 * (powf(sqrtf(2 * h1) + sqrtf(2 * h2), 2.0f)) / powf(this->road->S, 2.0f);
	else
		A = (200 * powf(sqrtf(h1) + sqrtf(h2), 2.0f)) / (2.0f * this->road->S - Lc);
	
	// Round up
	A = ceilf(A);

	// VPC
	CRAB::Vector4Df VPC2 = this->road->path2Dv.getPointFromStation(this->CS - Lc / 2);
	VPC2.y += this->VC + this->H;
	// VPI
	CRAB::Vector4Df VPI2 = this->road->path2Dv.getPointFromStation(this->CS);
	VPI2.y += this->VC + this->H + (A / 200) * Lc / 2;
	// VPT
	CRAB::Vector4Df VPT2 = this->road->path2Dv.getPointFromStation(this->CS + Lc / 2);
	VPT2.y += this->VC + this->H;

	// ********************************** SAG VERTICAL CURVE **********************************

	// Length of sag vertical curve (S > L)
	float Ls = 2 * this->road->S - ((120 + 3.5 * this->road->S) / (A / 2));
	Ls = int(round(Ls / 5)) * 5;
	// Minimun length of vertical curve (0.6 Vp)
	float Lmin = 0.60f * this->road->speed;
	// CHECK
	if (Ls < Lmin) Ls = Lmin;

	// Grades
	Ray g1, g2;
	g1.origin = VPC2;
	g1.direction = (VPC2 - VPI2).to_unitary();
	g2.origin = VPT2;
	g2.direction = (VPT2 - VPI2).to_unitary();

	// Tangent of the collided segment
	CRAB::Vector4Df tan_Segment;

	// Length of tangent segment
	float d;

	// Horizontal Matrix
	CRAB::Matrix4 PlaneXZ = CRAB::Matrix4{
			1, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1 };
	// Horizontal Vector
	CRAB::Vector4Df vPxz;

	// Angle of the tangent
	float cosTeta1, cosTeta2;

	// --------------
	//      LEFT
	// --------------

	// VPI
	CRAB::Vector4Df VPI1;
	for (int i = 0; i < this->road->path2Dv.segments.size(); i++)
	{
		// get the intersect point
		CRAB::Vector4Df P = this->road->path2Dv.segments[i]->Collision(g1);
		// check if its between the extreme points
		if (this->road->path2Dv.segments[i]->Contains(P))
		{
			VPI1 = P;
			tan_Segment = this->road->path2Dv.segments[i]->getTan(0.0f).to_unitary();
			break;
		}
		// if its before all segments
		if (i == 0)
		{
			VPI1 = P;
			tan_Segment = this->road->path2Dv.segments[i]->getTan(0.0f).to_unitary();
		}
	}
	// Projections (hor & vert)
	vPxz = (PlaneXZ * tan_Segment).to_unitary();
	cosTeta1 = fabsf(CRAB::dot(tan_Segment, vPxz));
	cosTeta2 = fabsf(CRAB::dot(g1.direction, vPxz));
	d = Ls / (cosTeta1 + cosTeta2);

	// VPC
	CRAB::Vector4Df VPC1 = VPI1 + (tan_Segment * -1.0f) * d;
	// VPT
	CRAB::Vector4Df VPT1 = VPI1 + (g1.direction * -1.0f) * d;

	// ---------------
	//      RIGHT
	// ---------------

	// VPI
	CRAB::Vector4Df VPI3;
	for (int i = 0; i < this->road->path2Dv.segments.size(); i++)
	{
		// get the intersect point
		CRAB::Vector4Df P = this->road->path2Dv.segments[i]->Collision(g2);
		// check if its between the extreme points
		if (this->road->path2Dv.segments[i]->Contains(P))
		{
			VPI3 = P;
			tan_Segment = this->road->path2Dv.segments[i]->getTan(1.0f);
			break;
		}
		// if its after all segments
		VPI3 = P;
		tan_Segment = this->road->path2Dv.segments[i]->getTan(1.0f);
	}
	// Projections (hor & vert)
	vPxz = (PlaneXZ * tan_Segment).to_unitary();
	cosTeta1 = fabsf(CRAB::dot(g2.direction, vPxz));
	cosTeta2 = fabsf(CRAB::dot(tan_Segment, vPxz));
	d = Ls / (cosTeta1 + cosTeta2);
	// VPC
	CRAB::Vector4Df VPC3 = VPI3 + (g2.direction * -1.0f) * d;
	// VPT
	CRAB::Vector4Df VPT3 = VPI3 + tan_Segment * d;

	// ********************************** RETURN **********************************

	this->path2Dv.segments.push_back(new CircularArc(VPC1, VPI1, VPT1));
	this->path2Dv.segments.push_back(new Line(VPT1, VPC2));
	this->path2Dv.segments.push_back(new CircularArc(VPC2, VPI2, VPT2));
	this->path2Dv.segments.push_back(new Line(VPT2, VPC3));
	this->path2Dv.segments.push_back(new CircularArc(VPC3, VPI3, VPT3));
}