#include "Bridge.h"

// DEFAULT CONSTRUCTOR
// -------------------
Bridge::Bridge()
{
}

// OVERLOAD CONSTRUCTOR (Viaduct)
// ------------------------------
Bridge::Bridge(const std::string& _name, Road* _road, const float& cross_station, const float& vertical_clearance, const float& horizontal_clearance, const float& start_station, const float& end_station)
	: name(_name), road(_road), CS(cross_station), VC(vertical_clearance), HC(horizontal_clearance), start_S(start_station), end_S(end_station)
{
	// General attributes
	this->mainSpan = main_span;
	//this->mainSpan = this->HC;
	//this->VC = v_clearance;

	// Viaduct
	int index = this->road->alignment->findSegment(this->CS);
	this->EL = this->road->alignment->profile[index]->getY(this->CS);
	this->WS = this->EL;

	// Preliminary calculations
	this->SetupBridge();

	std::cout << "\n\tNEW MODEL (Viaduct) ...................... " << name << std::endl;
}
// OVERLOAD CONSTRUCTOR (Overpass)
// -------------------------------
Bridge::Bridge(const std::string& _name, Road* _road, const float& cross_station, const float& vertical_clearance, const float& horizontal_clearance, const float& elevation_level, const float& start_station, const float& end_station)
	: name(_name), road(_road), CS(cross_station), VC(vertical_clearance), HC(horizontal_clearance), EL(elevation_level), start_S(start_station), end_S(end_station)
{
	// General attributes
	this->mainSpan = main_span;
	//this->mainSpan = this->HC;
	//this->VC = v_clearance;

	// Overpass
	this->WS = this->EL;

	// Preliminary calculations
	this->SetupBridge();

	std::cout << "\n\tNEW MODEL (Overpass) ..................... " << name << std::endl;
}
// OVERLOAD CONSTRUCTOR (Bridge)
// -----------------------------
Bridge::Bridge(const std::string& _name, Road* _road, const float& cross_station, const float& vertical_clearance, const float& horizontal_clearance, const float& elevation_level, const float& water_surface, const float& start_station, const float& end_station)
	: name(_name), road(_road), CS(cross_station), VC(vertical_clearance), HC(horizontal_clearance), EL(elevation_level), WS(water_surface), start_S(start_station), end_S(end_station)
{
	// General attributes
	this->mainSpan = main_span;
	//this->mainSpan = this->HC;
	//this->VC = v_clearance;

	// Preliminary calculations
	this->SetupBridge();

	std::cout << "\n\tNEW MODEL (Bridge) ....................... " << name << std::endl;
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
	this->B = this->road->width;
	// nos apoios
	//this->H = int((100.0f * this->mainSpan / 16.0f) / 5.0f) * 0.05f;
	this->H = int((100.0f * this->mainSpan / 20.0f) / 5.0f) * 0.05f;

	// Alignment
	////this->alignment = this->road->alignment;
	//std::vector<VerSegment*> total_Profile, start_Abutment, end_Abutment, bridge_Profile;
	//total_Profile = this->Vertical_Alignment();
	//this->abutments.clear();
	//// encontro inicial
	//start_Abutment.push_back(total_Profile.front());
	//this->abutments.push_back(new Alignment("Start_Abutment", this->Horizontal_Alignment(), start_Abutment));
	//// encontro final
	//end_Abutment.push_back(total_Profile.back());
	//this->abutments.push_back(new Alignment("End_Abutment", this->Horizontal_Alignment(), end_Abutment));
	//// alinhamento da superestrutura (sem os encontros)
	//for (int i = 1; i < 4; i++)
	//	bridge_Profile.push_back(total_Profile[i]);
	//this->alignment = new Alignment(this->name, this->Horizontal_Alignment(), bridge_Profile);
	this->alignment = new Alignment(this->name, this->Horizontal_Alignment(), this->Vertical_Alignment());
}

// SUPERELEVATION
// --------------
float Bridge::Superelevation(const float& t) const
{
	float r = this->alignment->getRadius(t);
	if (isinf(r))
		return 0.0f;
	float fMax;
	if (this->road->speed >= 80.0f)
		fMax = 0.24f - this->road->speed / 800.0f;
	else
		fMax = 0.188f - this->road->speed / 1667.0f;
	float slope = (SLOPE_MAX / (SLOPE_MAX + fMax)) * powf(this->road->speed, 2.0f) / (r * 127.0f); // AASHTO
	if (slope > SLOPE_MAX)
		slope = SLOPE_MAX;
	float alpha = atanf(slope) * 180.0f / M_PI;
	if (this->alignment->isClockwise(t))
		alpha = alpha * (-1.0f);

	//std::cout << "t = " << t << " slope = " << alpha << " curvature = " << this->alignment->getCurvature(t) << " radius = " << r << std::endl;
	return alpha;
}
// GET NORMAL VECTOR WITH SUPERELEVATION
// -------------------------------------
CRAB::Vector4Df Bridge::getNormal(const float& t) const
{
	CRAB::Matrix4 R = CRAB::rotateArbitrary(this->Superelevation(t), this->alignment->getTangent(t));
	return (R * this->alignment->getNormalUp(t)).to_unitary();
}
// WIDENING
// --------
float Bridge::Widening(const float& t) const
{
	float radius = this->alignment->getRadius(t);
	//std::cout << "R = " << radius << std::endl;
	/* TRAVELED WAY ON TANGENT */
	if (isinf(radius))
		return 0.0f;
	/* TRAVELED WAY ON CURVE */
	float sumL = 0.0f;
	for (int i = 0; i < this->road->vehicle->L.size(); i++)
		//sumL += this->road->vehicle->L[i]; /* VERIFICAR: acumular valores de Li ao quadrado */
		sumL += powf(this->road->vehicle->L[i], 2.0f);
	// Track width on curve (U)
	//float U = radius - sqrtf(powf(radius, 2.0f) - powf(sumL, 2.0f)); /* VERIFICAR: somat�rio do quadrado, e n�o o quadrado do somat�rio */
	float U = this->road->vehicle->u + radius - sqrtf(powf(radius, 2.0f) - sumL);
	// Width of the front overhang (Fa)
	float Fa = sqrtf(powf(radius, 2.0f) + this->road->vehicle->A * (2.0f * this->road->vehicle->L.front() + this->road->vehicle->A)) - radius;
	// Extra width (Z)
	float Z = 0.1f * this->road->speed / sqrtf(radius);
	// Lateral clearance (C)
	float C = this->road->width / nLanes - U;
	if (C < 0.9f)
		C = 0.9f;
	//std::cout << "C = " << C << std::endl;
	// Width needed on a curve (Wc)
	float Wc = nLanes * (U + C) + (nLanes - 1) * Fa + Z;
	// Widening of traveled way on curve (w)
	return Wc - this->road->width;
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

	/* ================================== CREST VERTICAL CURVE ================================== */

	// Length of crest vertical curve
	float Lc = this->HC;

	// Algebraic difference in grades (%)
	float A;
	if (this->road->StoppingSightDistance() < Lc)
		A = Lc * 100 * (powf(sqrtf(2 * h1) + sqrtf(2 * h2), 2.0f)) / powf(this->road->StoppingSightDistance(), 2.0f);
	else
		A = (200 * powf(sqrtf(h1) + sqrtf(h2), 2.0f)) / (2.0f * this->road->StoppingSightDistance() - Lc);
	
	// Round up
	A = ceilf(A);
	//std::cout << "A = " << A << std::endl;
	//std::cout << "S = " << this->road->StoppingSightDistance() << std::endl;

	// VPC
	CRAB::Vector4Df VPC = { 0.0f, 0.0f, 0.0f, 1.0f };
	VPC.x = this->CS - Lc / 2.0f;
	if (this->WS == this->EL) // verifica se � viaduto
	{
		index = this->road->alignment->findSegment(VPC.x);
		VPC.y = this->road->alignment->profile[index]->getY(VPC.x) + this->VC + this->H;
	}
	else
		VPC.y = this->WS + this->VC + this->H;

	// VPT
	CRAB::Vector4Df VPT = { 0.0f, 0.0f, 0.0f, 1.0f };
	VPT.x = this->CS + Lc / 2.0f;
	if (this->WS == this->EL) // verifica se � viaduto
	{
		index = this->road->alignment->findSegment(VPT.x);
		VPT.y = this->road->alignment->profile[index]->getY(VPT.x) + this->VC + this->H;
	}
	else
		VPT.y = this->WS + this->VC + this->H;

	VerSegment* CrestCurve = new VerSegment(VPC, VPT, A);

	/* ================================== SAG VERTICAL CURVE ================================== */

	// LEFT
	VerSegment* SagCurve_LEFT;
	{
		CRAB::Vector4Df VPI;
		CRAB::Vector4Df grade;

		// Collision Test
		CRAB::Ray r;
		r.origin = CrestCurve->getStartPoint4D();
		r.direction = (CrestCurve->getStartPoint4D() - CrestCurve->getMidPoint4D()).to_unitary();
		for (int i = 0; i < this->road->alignment->profile.size(); i++)
		{
			CRAB::Vector4Df P = this->road->alignment->profile[i]->Collision(r);
			// check if its between the extreme points
			if (this->road->alignment->profile[i]->Contains(P))
			{
				VPI = P;
				grade = (this->road->alignment->profile[i]->getMidPoint4D() - this->road->alignment->profile[i]->getStartPoint4D()).to_unitary();
				break;
			}
			// if its before all segments
			if (i == 0)
			{
				VPI = P;
				grade = (this->road->alignment->profile[i]->getMidPoint4D() - this->road->alignment->profile[i]->getStartPoint4D()).to_unitary();
			}
		}

		// Algebraic difference in grades (%)
		float As = 100.0f * tanf(acosf(CRAB::dot(grade, r.direction * (-1.0f))));
		// Length of sag vertical curve (S < L)
		float Ls = (As / 2) * powf(this->road->StoppingSightDistance(), 2.0f) / (120.0f + 3.5f * this->road->StoppingSightDistance());

		SagCurve_LEFT = new VerSegment(VPI, grade, r.direction * (-1.0f), Ls);

		/*std::cout << "\n" << std::endl;
		std::cout << "SagCurve_LEFT" << std::endl;
		std::cout << "A = " << As << " %" << std::endl;
		std::cout << "L = " << Ls << " m" << std::endl;
		std::cout << "\n" << std::endl;*/
	}

	// RIGHT
	VerSegment* SagCurve_RIGHT;
	{
		CRAB::Vector4Df VPI;
		CRAB::Vector4Df grade;

		// Collision Test
		CRAB::Ray r;
		r.origin = CrestCurve->getEndPoint4D();
		r.direction = (CrestCurve->getEndPoint4D() - CrestCurve->getMidPoint4D()).to_unitary();
		for (int i = 0; i < this->road->alignment->profile.size(); i++)
		{
			CRAB::Vector4Df P = this->road->alignment->profile[i]->Collision(r);
			// check if its between the extreme points
			if (this->road->alignment->profile[i]->Contains(P))
			{
				VPI = P;
				grade = (this->road->alignment->profile[i]->getEndPoint4D() - this->road->alignment->profile[i]->getMidPoint4D()).to_unitary();
				break;
			}
			// if its before all segments
			if (i == 0)
			{
				VPI = P;
				grade = (this->road->alignment->profile[i]->getEndPoint4D() - this->road->alignment->profile[i]->getMidPoint4D()).to_unitary();
			}
		}

		// Algebraic difference in grades (%)
		float As = 100.0f * tanf(acosf(CRAB::dot(grade, r.direction)));
		// Length of sag vertical curve (S < L)
		float Ls = (As / 2) * powf(this->road->StoppingSightDistance(), 2.0f) / (120.0f + 3.5f * this->road->StoppingSightDistance());

		SagCurve_RIGHT = new VerSegment(VPI, r.direction, grade, Ls);

		/*std::cout << "\n" << std::endl;
		std::cout << "SagCurve_RIGHT" << std::endl;
		std::cout << "A = " << As << " %" << std::endl;
		std::cout << "L = " << Ls << " m" << std::endl;
		std::cout << "\n" << std::endl;*/
	}

	//// ********************************** RETURN **********************************

	profile.push_back(SagCurve_LEFT);
	profile.push_back(new VerSegment(SagCurve_LEFT->getEndPoint4D(), CrestCurve->getStartPoint4D()));
	profile.push_back(CrestCurve);
	profile.push_back(new VerSegment(CrestCurve->getEndPoint4D(), SagCurve_RIGHT->getStartPoint4D()));
	profile.push_back(SagCurve_RIGHT);

	// ********************************** DEBUG **********************************

	/*std::cout << "\nCURVE 1" << std::endl;
	CRAB::Vector4Df VPC1 = SagCurve_LEFT->getStartPoint4D();
	CRAB::Vector4Df VPI1 = SagCurve_LEFT->getMidPoint4D();
	CRAB::Vector4Df VPT1 = SagCurve_LEFT->getEndPoint4D();
	std::cout << "VPC1 = [" << VPC1.x << "; " << VPC1.y << "; " << VPC1.z << "]" << std::endl;
	std::cout << "VPI1 = [" << VPI1.x << "; " << VPI1.y << "; " << VPI1.z << "]" << std::endl;
	std::cout << "VPT1 = [" << VPT1.x << "; " << VPT1.y << "; " << VPT1.z << "]" << std::endl;
	std::cout << "\nCURVE 2" << std::endl;
	CRAB::Vector4Df VPI2 = CrestCurve->getMidPoint4D();
	std::cout << "VPC2 = [" << VPC.x << "; " << VPC.y << "; " << VPC.z << "]" << std::endl;
	std::cout << "VPI2 = [" << VPI2.x << "; " << VPI2.y << "; " << VPI2.z << "]" << std::endl;
	std::cout << "VPT2 = [" << VPT.x << "; " << VPT.y << "; " << VPT.z << "]" << std::endl;
	std::cout << "\nCURVE 3" << std::endl;
	CRAB::Vector4Df VPC3 = SagCurve_RIGHT->getStartPoint4D();
	CRAB::Vector4Df VPI3 = SagCurve_RIGHT->getMidPoint4D();
	CRAB::Vector4Df VPT3 = SagCurve_RIGHT->getEndPoint4D();
	std::cout << "VPC3 = [" << VPC3.x << "; " << VPC3.y << "; " << VPC3.z << "]" << std::endl;
	std::cout << "VPI3 = [" << VPI3.x << "; " << VPI3.y << "; " << VPI3.z << "]" << std::endl;
	std::cout << "VPT3 = [" << VPT3.x << "; " << VPT3.y << "; " << VPT3.z << "]" << std::endl;*/

	return profile;
}
