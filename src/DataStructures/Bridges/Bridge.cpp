#include "Bridge.h"

// DEFAULT CONSTRUCTOR
// -------------------
Bridge::Bridge()
{
}

// OVERLOAD CONSTRUCTOR (Viaduct)
// ------------------------------
Bridge::Bridge(const std::string& _name, Road* _road, const float& cross_station, const float& vertical_clearance, const float& horizontal_clearance)
	: name(_name), road(_road), CS(cross_station), VC(vertical_clearance), HC(horizontal_clearance)
{
	std::cout << std::endl;
	std::cout << "\tNEW MODEL (Viaduct) ...................... " << name << std::endl;	
	// Viaduct
	int index = this->road->alignment->findSegment(this->CS);
	this->EL = this->road->alignment->profile[index]->getY(this->CS);
	this->WS = this->EL;

	// Preliminary calculations
	this->SetupBridge();
}
// OVERLOAD CONSTRUCTOR (Overpass)
// -------------------------------
Bridge::Bridge(const std::string& _name, Road* _road, const float& cross_station, const float& vertical_clearance, const float& horizontal_clearance, const float& elevation_level)
	: name(_name), road(_road), CS(cross_station), VC(vertical_clearance), HC(horizontal_clearance), EL(elevation_level)
{
	std::cout << std::endl;
	std::cout << "\tNEW MODEL (Overpass) ..................... " << name << std::endl;
	// Overpass
	this->WS = this->EL;

	// Preliminary calculations
	this->SetupBridge();
}
// OVERLOAD CONSTRUCTOR (Bridge)
// -----------------------------
Bridge::Bridge(const std::string& _name, Road* _road, const float& cross_station, const float& vertical_clearance, const float& horizontal_clearance, const float& elevation_level, const float& water_surface)
	: name(_name), road(_road), CS(cross_station), VC(vertical_clearance), HC(horizontal_clearance), EL(elevation_level), WS(water_surface)
{
	std::cout << std::endl;
	std::cout << "\tNEW MODEL (Bridge) ....................... " << name << std::endl;

	// Preliminary calculations
	this->SetupBridge();
}

// DESTRUCTOR
// ----------
Bridge::~Bridge()
{
}

// INITIALIZES ALL THE PARAMETERS
// ------------------------------
void Bridge::SetupBridge()
{
	// Bridge attributes
	this->mainSpan = 35.0f;
	this->B = this->road->width + 2 * GUARD_RAIL;
	this->H = int((100.0f * this->mainSpan / 16.0f) / 5.0f) * 0.05f;

	// Alignment
	//this->alignment = this->road->alignment;
	this->alignment = new Alignment(this->name, this->Horizontal_Alignment(), this->Vertical_Alignment());
}

// HORIZONTAL ALIGNMENT
// --------------------
std::vector<HorSegment*> Bridge::Horizontal_Alignment()
{
	std::cout << "\tHorizontal alignment" << std::endl;
	return this->road->alignment->plan;
}

// VERTICAL ALIGNMENT
// ------------------
std::vector<VerSegment*> Bridge::Vertical_Alignment()
{
	std::cout << "\tVertical alignment" << std::endl;
	//return this->road->alignment->profile;
	std::vector<VerSegment*> profile;

	// Check type of bridge
	int index = this->road->alignment->findSegment(this->CS);
	float CSy = this->road->alignment->profile[index]->getY(this->CS);
	if (fabsf(CSy - this->EL) >= (this->VC + this->H)
		&& fabsf(CSy - this->WS) >= (this->VC + this->H))
	{
		// Start
		CRAB::Vector4Df VPT = { 0.0f, 0.0f, 0.0f, 1.0f };
		VPT.x = this->CS - this->HC / 2.0f;
		index = this->road->alignment->findSegment(VPT.x);
		VPT.y = this->road->alignment->profile[index]->getY(VPT.x);
		// End
		CRAB::Vector4Df VPC = { 0.0f, 0.0f, 0.0f, 1.0f };
		VPC.x = this->CS + this->HC / 2.0f;
		index = this->road->alignment->findSegment(VPC.x);
		VPC.y = this->road->alignment->profile[index]->getY(VPC.x);

		profile.push_back(new VerSegment(VPT, VPC));
		return profile;
	}

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
	/*CRAB::Vector4Df VPC2 = this->road->path2Dv.getPointFromStation(this->CS - Lc / 2);
	VPC2.y += this->VC + this->H;*/
	CRAB::Vector4Df VPC2 = { 0.0f, 0.0f, 0.0f, 1.0f };
	VPC2.x = this->CS - Lc / 2.0f;														// coordenada horizontal
	//index = this->road->alignment->findSegment(VPC2.x);									// segmento onde está contido
	//VPC2.y = this->road->alignment->profile[index]->getY(VPC2.x) + this->VC + this->H;	// coordenada vertical
	//VPC2.y = this->EL + this->VC + this->H;
	VPC2.y = this->WS + this->VC + this->H;
	// VPI
	/*CRAB::Vector4Df VPI2 = this->road->path2Dv.getPointFromStation(this->CS);
	VPI2.y += this->VC + this->H + (A / 200) * Lc / 2;*/
	CRAB::Vector4Df VPI2 = { 0.0f, 0.0f, 0.0f, 1.0f };
	VPI2.x = this->CS;																								// coordenada horizontal
	//index = this->road->alignment->findSegment(VPI2.x);																// segmento onde está contido
	//VPI2.y = this->road->alignment->profile[index]->getY(VPI2.x) + this->VC + this->H + (A / 200.0f) * Lc / 2.0f;	// coordenada vertical
	//VPI2.y = this->EL + this->VC + this->H + (A / 200.0f) * Lc / 2.0f;
	VPI2.y = this->WS + this->VC + this->H + (A / 200.0f) * Lc / 2.0f;
	// VPT
	/*CRAB::Vector4Df VPT2 = this->road->path2Dv.getPointFromStation(this->CS + Lc / 2);
	VPT2.y += this->VC + this->H;*/
	CRAB::Vector4Df VPT2 = { 0.0f, 0.0f, 0.0f, 1.0f };
	VPT2.x = this->CS + Lc / 2.0f;														// coordenada horizontal
	//index = this->road->alignment->findSegment(VPT2.x);									// segmento onde está contido
	//VPT2.y = this->road->alignment->profile[index]->getY(VPT2.x) + this->VC + this->H;	// coordenada vertical
	//VPT2.y = this->EL + this->VC + this->H;
	VPT2.y = this->WS + this->VC + this->H;

	//// ********************************** SAG VERTICAL CURVE **********************************

	// Length of sag vertical curve (S > L)
	float Ls = 2 * this->road->S - ((120 + 3.5 * this->road->S) / (A / 2));
	Ls = int(round(Ls / 5)) * 5;
	// Minimun length of vertical curve (0.6 Vp)
	float Lmin = 0.60f * this->road->speed;
	// CHECK
	if (Ls < Lmin) Ls = Lmin;

	// Grades
	CRAB::Ray g1, g2;
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
	for (int i = 0; i < this->road->alignment->profile.size(); i++)
	{
		// get the intersect point
		CRAB::Vector4Df P = this->road->alignment->profile[i]->Collision(g1);
		// check if its between the extreme points
		if (this->road->alignment->profile[i]->Contains(P))
		{
			VPI1 = P;
			tan_Segment = (this->road->alignment->profile[i]->getMid4DPoint() - this->road->alignment->profile[i]->getStart4DPoint()).to_unitary();
			break;
		}
		// if its before all segments
		if (i == 0)
		{
			VPI1 = P;
			tan_Segment = (this->road->alignment->profile[i]->getMid4DPoint() - this->road->alignment->profile[i]->getStart4DPoint()).to_unitary();
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
	for (int i = 0; i < this->road->alignment->profile.size(); i++)
	{
		// get the intersect point
		CRAB::Vector4Df P = this->road->alignment->profile[i]->Collision(g2);
		// check if its between the extreme points
		if (this->road->alignment->profile[i]->Contains(P))
		{
			VPI3 = P;
			tan_Segment = (this->road->alignment->profile[i]->getEnd4DPoint() - this->road->alignment->profile[i]->getMid4DPoint()).to_unitary();
			break;
		}
		// if its after all segments
		VPI3 = P;
		tan_Segment = (this->road->alignment->profile[i]->getEnd4DPoint() - this->road->alignment->profile[i]->getMid4DPoint()).to_unitary();
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

	//// ********************************** RETURN **********************************
	
	profile.push_back(new VerSegment(VPC1, VPI1, VPT1));
	profile.push_back(new VerSegment(VPT1, VPC2));
	profile.push_back(new VerSegment(VPC2, VPI2, VPT2));
	profile.push_back(new VerSegment(VPT2, VPC3));
	profile.push_back(new VerSegment(VPC3, VPI3, VPT3));

	// ********************************** DEBUG **********************************

	/*std::cout << "\nCURVE 1" << std::endl;
	std::cout << "VPC1 = [" << VPC1.x << "; " << VPC1.y << "; " << VPC1.z << "]" << std::endl;
	std::cout << "VPI1 = [" << VPI1.x << "; " << VPI1.y << "; " << VPI1.z << "]" << std::endl;
	std::cout << "VPT1 = [" << VPT1.x << "; " << VPT1.y << "; " << VPT1.z << "]" << std::endl;
	std::cout << "\nCURVE 2" << std::endl;
	std::cout << "VPC2 = [" << VPC2.x << "; " << VPC2.y << "; " << VPC2.z << "]" << std::endl;
	std::cout << "VPI2 = [" << VPI2.x << "; " << VPI2.y << "; " << VPI2.z << "]" << std::endl;
	std::cout << "VPT2 = [" << VPT2.x << "; " << VPT2.y << "; " << VPT2.z << "]" << std::endl;
	std::cout << "\nCURVE 3" << std::endl;
	std::cout << "VPC3 = [" << VPC3.x << "; " << VPC3.y << "; " << VPC3.z << "]" << std::endl;
	std::cout << "VPI3 = [" << VPI3.x << "; " << VPI3.y << "; " << VPI3.z << "]" << std::endl;
	std::cout << "VPT3 = [" << VPT3.x << "; " << VPT3.y << "; " << VPT3.z << "]" << std::endl;*/

	return profile;
}
