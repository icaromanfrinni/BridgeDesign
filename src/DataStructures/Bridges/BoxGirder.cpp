#include "BoxGirder.h"
#include "EulerOp.h"

// DEFAULT CONSTRUCTOR
// -------------------
BoxGirder::BoxGirder()
{
}

// OVERLOAD CONSTRUCTOR (Viaduct)
// ------------------------------
BoxGirder::BoxGirder(const std::string& _name, Road* _road, const float& cross_station, const float& vertical_clearance, const float& horizontal_clearance, const std::vector<float>& _stations)
	: Bridge(_name, _road, cross_station, vertical_clearance, horizontal_clearance)
{
	// Preliminary calculations
	this->SetupSection();

	if (_stations.empty())
	{
		// Piers
		int n = round(this->alignment->getProfileLength() / this->mainSpan);
		//std::cout << "getProfileLength() = " << this->alignment->getProfileLength() << std::endl;
		//std::cout << "nPiers = " << nPiers << std::endl;
		SetupPiers(n);
	}
	else
		SetupPiers(_stations);
	
	// Model
	this->Update();
}

// OVERLOAD CONSTRUCTOR (Overpass)
// -------------------------------
BoxGirder::BoxGirder(const std::string& _name, Road* _road, const float& cross_station, const float& vertical_clearance, const float& horizontal_clearance, const float& elevation_level)
	: Bridge(_name, _road, cross_station, vertical_clearance, horizontal_clearance, elevation_level)
{
	// Preliminary calculations
	this->SetupSection();
	// Piers
	int n = round(this->alignment->getProfileLength() / this->mainSpan);
	SetupPiers(n);
	// Model
	this->Update();
}

// OVERLOAD CONSTRUCTOR (Bridge)
// -----------------------------
BoxGirder::BoxGirder(const std::string& _name, Road* _road, const float& cross_station, const float& vertical_clearance, const float& horizontal_clearance, const float& elevation_level, const float& water_surface)
	: Bridge(_name, _road, cross_station, vertical_clearance, horizontal_clearance, elevation_level, water_surface)
{
	// Preliminary calculations
	this->SetupSection();
	// Piers
	int n = round(this->alignment->getProfileLength() / this->mainSpan);
	SetupPiers(n);
	// Model
	this->Update();
}

// DESTRUCTOR
// ----------
BoxGirder::~BoxGirder()
{
}

// SETUP ALL THE PARAMETERS
// ------------------------
void BoxGirder::SetupSection()
{
	// Box-Girder Bridge Attributes
	Lb = int((100.0f * B / 4.3f) / 5.0f) * 0.05f;
	h = int((100.0f * Lb / 10.0f) / 5.0f) * 0.05f;
	bw = int((100.0f * H / 5.0f) / 5.0f) * 0.05f;
	if (bw < 0.25f) bw = 0.25f;
	tv = int((100.0f * (H - 2 * h) / 10.0f) / 5.0f) * 0.05f;
	if (tv < 0.10f) tv = 0.10f;
	b = int((100.0f * (B - 2.0f * (Lb + web_inclination * (H - h - tv)))) / 5.0f) * 0.05f;
	th = int((100.0f * (b - 2 * bw) / 5.0f) / 5.0f) * 0.05f;

	/*std::cout << "\nH = " << H << std::endl;
	std::cout << "Lb = " << Lb << std::endl;
	std::cout << "h = " << h << std::endl;
	std::cout << "bw = " << bw << std::endl;
	std::cout << "tv = " << tv << std::endl;
	std::cout << "b = " << b << std::endl;
	std::cout << "th = " << th << std::endl;*/
}
void BoxGirder::SetupSection(const float& t)
{
	if (/*t == 0.0f || */!haunchedGirder)
	{
		this->SetupSection();
		this->dH = H;
		return;
	}

	// ALTURA VARIÁVEL ENTRE APOIOS
	//std::cout << "t = " << t << std::endl;
	// Distância X
	//float x = this->alignment->getDistance(t);
	float x = this->alignment->getHorDistance(t);
	/*std::cout << "\nx = " << x << std::endl;
	std::cout << "x,plano = " << _x << std::endl;*/
	// Intervalo
	float low = 0.0f, high = 0.0f;
	for (int i = 0; i < this->span_vector.size(); i++)
	{
		/*if (x > this->span_vector[i])
			low = this->span_vector[i];
		else
		{
			high = this->span_vector[i];
			break;
		}*/
		if (x >= this->span_vector[i].start && x <= this->span_vector[i].end)
		{
			low = this->span_vector[i].start;
			high = this->span_vector[i].end;
			break;
		}
	}
	if (low == 0.0f && high == 0.0f)
	{
		this->SetupSection();
		this->dH = H;
		return;
	}
	//std::cout << "low = " << low << std::endl;
	//std::cout << "high = " << high << std::endl;
	// Atualiza distância
	x -= low;
	//std::cout << "x,depois = " << x << std::endl;
	// Vão do intervalo atual
	float current_span = high - low;
	// Altura Y
	float C1 = -2.0f * H / powf(current_span, 2.0f);
	float C2 = 2.0f * H / current_span;
	float y = C1 * powf(x, 2.0f) + C2 * x;
	// Atualiza a altura
	this->dH = H - y;

	// Box-Girder Bridge Attributes
	Lb = int((100.0f * B / 4.3f) / 5.0f) * 0.05f;
	h = int((100.0f * Lb / 10.0f) / 5.0f) * 0.05f;
	bw = int((100.0f * this->dH / 5.0f) / 5.0f) * 0.05f;
	if (bw < 0.25f) bw = 0.25f;
	tv = int((100.0f * (this->dH - 2 * h) / 10.0f) / 5.0f) * 0.05f;
	if (tv < 0.10f) tv = 0.10f;	
	if (this->iLocked)
		b = int((100.0f * (B - 2.0f * (Lb + this->iw * (this->dH - h - tv)))) / 5.0f) * 0.05f;
	else
	{
		float ix = (B - (2 * Lb + b)) / 2;
		float iy = this->dH - (h + tv);
		this->iw = ix / iy;
	}
	th = int((100.0f * (b - 2 * bw) / 5.0f) / 5.0f) * 0.05f;

	/*if (this->dH < 1.80f)
	{
		std::cout << "\nH = " << this->dH << std::endl;
		std::cout << "Lb = " << Lb << std::endl;
		std::cout << "h = " << h << std::endl;
		std::cout << "bw = " << bw << std::endl;
		std::cout << "tv = " << tv << std::endl;
		std::cout << "b = " << b << std::endl;
		std::cout << "th = " << th << std::endl;
	}*/
}
void BoxGirder::SetupPiers(const int& _nPiers)
{
	float b_Pier = /*0.9f **/ this->b;
	float h_Pier = 0.6f * b_Pier;
	float span_length = this->alignment->getProfileLength() / _nPiers;
	//std::cout << "span_length = " << span_length << std::endl;
	//float end_length = (total_length - (_nPiers - 1) * span) / 2.0f;
	// first pier
	float station = this->alignment->profile.front()->getStart4DPoint().x + span_length / 2.0f;

	this->piers.clear();
	this->span_vector.clear();
	//this->span_vector.push_back(0.0f);
	//this->span_vector.push_back(-span_length / 2.0f);
	Span span;
	span.start = -span_length / 2.0f;
	for (int i = 0; i < _nPiers; i++)
	{
		//std::cout << "\nPilar P" << i + 1 << std::endl;
		Pier P;
		P.b = b_Pier;
		//std::cout << "B pilar = " << P.b << std::endl;
		P.h = h_Pier;
		P.station = station;
		
		//this->span_vector.push_back(P.station - this->alignment->profile.front()->getStart4DPoint().x);
		span.end = P.station - this->alignment->profile.front()->getStart4DPoint().x - P.h / 2.0f;
		this->span_vector.push_back(span);
		span.start = P.station - this->alignment->profile.front()->getStart4DPoint().x + P.h / 2.0f;

		P.ang = 0.0f;
		P.dir = this->alignment->getTangentFromStation(P.station);
		P.base = this->road->alignment->getPositionFromStation(P.station);
		if (P.base.y > this->EL)
			P.base.y = this->EL;
		P.depth = 0.50f;
		P.base.y -= P.depth; // topo do bloco
		CRAB::Vector4Df top = this->alignment->getPositionFromStation(P.station);
		P.L = (top - P.base).length() - this->H;
		//std::cout << "H pilar = " << P.L << std::endl;
		//P.L = (top - P.base).length() - (this->H + (this->B / 2.0f) * SLOPE + TOP_LAYER);
		piers.push_back(P);
		station += span_length;
	}
	//this->span_vector.push_back(this->alignment->profile.back()->getEnd4DPoint().x - this->alignment->profile.front()->getStart4DPoint().x);
	//float last_span = 2.0f * ((this->alignment->profile.back()->getEnd4DPoint().x - this->alignment->profile.front()->getStart4DPoint().x) - this->span_vector.back());
	float last_span = 2.0f * ((this->alignment->profile.back()->getEnd4DPoint().x - this->alignment->profile.front()->getStart4DPoint().x) - span.start);
	span.end = span.start + last_span;
	//this->span_vector.push_back(this->span_vector.back() + last_span);
	this->span_vector.push_back(span);
}
void BoxGirder::SetupPiers(const std::vector<float>& _stations)
{
	float b_Pier = /*0.9f **/ this->b;
	float h_Pier = 0.6f * b_Pier;
	float span_length = _stations.front() - this->alignment->profile.front()->getStart4DPoint().x;

	this->piers.clear();
	this->span_vector.clear();

	Span span;
	span.start = -span_length;
	for (int i = 0; i < _stations.size(); i++)
	{
		//std::cout << "\nPilar P" << i+1 << std::endl;
		Pier P;
		P.b = b_Pier;
		//std::cout << "B pilar = " << P.b << std::endl;
		P.h = h_Pier;
		P.station = _stations[i];

		span.end = P.station - this->alignment->profile.front()->getStart4DPoint().x - P.h / 2.0f;
		this->span_vector.push_back(span);
		span.start = P.station - this->alignment->profile.front()->getStart4DPoint().x + P.h / 2.0f;

		P.ang = 0.0f;
		P.dir = this->alignment->getTangentFromStation(P.station);
		P.base = this->road->alignment->getPositionFromStation(P.station);
		if (P.base.y > this->EL)
			P.base.y = this->EL;
		P.depth = 0.50f;
		P.base.y -= P.depth; // topo do bloco
		CRAB::Vector4Df top = this->alignment->getPositionFromStation(P.station);
		P.L = (top - P.base).length() - this->H;
		//std::cout << "H pilar = " << P.L << std::endl;
		piers.push_back(P);
	}
	float profile_length = this->alignment->profile.back()->getEnd4DPoint().x - this->alignment->profile.front()->getStart4DPoint().x;
	float last_span = 2.0f * (profile_length - span.start);
	span.end = span.start + last_span;
	this->span_vector.push_back(span);
}
void BoxGirder::AddPier()
{
	Pier P;
	P.b =/* 0.9f **/ this->b;
	P.h = 0.6f * P.b;
	P.station = this->alignment->profile.front()->getStart4DPoint().x + this->alignment->getProfileLength() / 2.0f;
	P.ang = 0.0f;
	P.dir = this->alignment->getTangentFromStation(P.station);
	P.base = this->road->alignment->getPositionFromStation(P.station);
	if (P.base.y > this->EL)
		P.base.y = this->EL;
	P.depth = 0.50f;
	P.base.y -= P.depth; // topo do bloco
	CRAB::Vector4Df top = this->alignment->getPositionFromStation(P.station);
	P.L = (top - P.base).length() - this->H;
	//P.L = (top - P.base).length() - (this->H + (this->B / 2.0f) * SLOPE + TOP_LAYER);
	piers.push_back(P);
}

// RETURNS THE VERTICES OF THE CROSS SECTION
// -----------------------------------------
std::vector<CRAB::Vector4Df> BoxGirder::TopLayer_section(const float& t) const
{
	// Local axis
	CRAB::Vector4Df vUp = { 0.0f, 1.0f, 0.0f, 0.0f };
	CRAB::Vector4Df vRight = { 1.0f, 0.0f, 0.0f, 0.0f };

	// Widening
	float w = 0.0f;
	if (hasWidening)
		w = this->Widening(t);

	// Local View
	std::vector<CRAB::Vector4Df> nodes;
	nodes.push_back(CRAB::Vector4Df{ 0.0f, 0.0f, 0.0f, 1.0f });
	// Left
	nodes.push_back(nodes.back() - (vRight * ((B + w) / 2.0f)) - (vUp * ((B + w) / 2.0f) * SLOPE));
	nodes.push_back(nodes.back() - (vUp * TOP_LAYER));
	// Right
	nodes.push_back(nodes.front() + reflection(nodes[2] - nodes.front(), vUp));
	nodes.push_back(nodes.front() + reflection(nodes[1] - nodes.front(), vUp));

	// Space View
	CRAB::Matrix4 ModelMatrix = toWorld(this->alignment->getPosition(t), this->alignment->getTangent(t), this->getNormal(t));
	for (int i = 0; i < nodes.size(); i++)
		nodes[i] = ModelMatrix * nodes[i];

	return nodes;
}
std::vector<CRAB::Vector4Df> BoxGirder::Deck_section(const float& t)
{
	// Local axis
	CRAB::Vector4Df vUp = { 0.0f, 1.0f, 0.0f, 0.0f };
	CRAB::Vector4Df vRight = { 1.0f, 0.0f, 0.0f, 0.0f };

	// Update Bridge Attributes
	this->SetupSection(t);

	// Widening
	float w = 0.0f;
	if (hasWidening)
		w = this->Widening(t);

	//std::cout << "\tt = " << t << " w = " << w << std::endl;

	// Local View
	float offset = (B / 2.0f) * SLOPE + TOP_LAYER;
	std::vector<CRAB::Vector4Df> nodes;
	nodes.push_back(CRAB::Vector4Df{ 0.0f, 0.0f, 0.0f, 1.0f } - (vUp * offset));
	// Left Guard-Rail
	{
		nodes.push_back(nodes.back() - (vRight * ((B + w) / 2.0f)));
		nodes.push_back(nodes.back() + (vUp * 0.15f));
		nodes.push_back(nodes.back() + (vUp * 0.25f) - (vRight * 0.175f));
		nodes.push_back(nodes.back() + (vUp * 0.47f) - (vRight * 0.05f));
		nodes.push_back(nodes.back() - (vRight * 0.175f));
		nodes.push_back(nodes.back() - (vUp * 1.12f));
		nodes.push_back(nodes.back() - (vUp * 0.05f) + (vRight * 0.05f));
		nodes.push_back(nodes.back() + (vUp * 0.10f) + (vRight * 0.35f));
	}
	nodes.push_back(nodes.back() - (vUp * (h + tv - 0.20f)) + (vRight * Lb));
	float hyp = sqrtf(powf(this->iw * bw, 2.0f) + powf(bw, 2.0f)); // Hypotenuse = top width of beam
	nodes.push_back(nodes.back() + (vRight * hyp));
	nodes.push_back(nodes.back() + (vUp * tv) + (vRight * th));
	// MIRROR
	nodes.push_back(nodes.front() + reflection(nodes[11] - nodes.front(), vUp));
	nodes.push_back(nodes.front() + reflection(nodes[10] - nodes.front(), vUp));
	nodes.push_back(nodes.front() + reflection(nodes[9] - nodes.front(), vUp));
	// Right Guard-Rail
	{
		nodes.push_back(nodes.front() + reflection(nodes[8] - nodes.front(), vUp));
		nodes.push_back(nodes.front() + reflection(nodes[7] - nodes.front(), vUp));
		nodes.push_back(nodes.front() + reflection(nodes[6] - nodes.front(), vUp));
		nodes.push_back(nodes.front() + reflection(nodes[5] - nodes.front(), vUp));
		nodes.push_back(nodes.front() + reflection(nodes[4] - nodes.front(), vUp));
		nodes.push_back(nodes.front() + reflection(nodes[3] - nodes.front(), vUp));
		nodes.push_back(nodes.front() + reflection(nodes[2] - nodes.front(), vUp));
		nodes.push_back(nodes.front() + reflection(nodes[1] - nodes.front(), vUp));
	}

	// Space View
	CRAB::Matrix4 ModelMatrix = toWorld(this->alignment->getPosition(t), this->alignment->getTangent(t), this->getNormal(t));
	for (int i = 0; i < nodes.size(); i++)
		nodes[i] = ModelMatrix * nodes[i];

	return nodes;
}
std::vector<CRAB::Vector4Df> BoxGirder::U_section(const float& t)
{
	// Local axis
	CRAB::Vector4Df vUp = { 0.0f, 1.0f, 0.0f, 0.0f };
	CRAB::Vector4Df vRight = { 1.0f, 0.0f, 0.0f, 0.0f };

	// Update Bridge Attributes
	this->SetupSection(t);

	// Local View
	float offset = (B / 2.0f) * SLOPE + dH + TOP_LAYER;
	std::vector<CRAB::Vector4Df> nodes;
	nodes.push_back(CRAB::Vector4Df{ 0.0f, 0.0f, 0.0f, 1.0f } - (vUp * offset));
	nodes.push_back(nodes.back() + (vRight * (b / 2.0f)));
	nodes.push_back(nodes.back() + (vRight * (B - b - 2 * Lb) / 2.0f) + (vUp * (dH - h - tv)));
	float hyp = sqrtf(powf(this->iw * bw, 2.0f) + powf(bw, 2.0f)); // Hypotenuse = top width of beam
	nodes.push_back(nodes.back() - (vRight * hyp));
	nodes.push_back(nodes.back() - (vUp * (dH - 2 * (h + tv))) - (vRight * (this->iw * ((dH - 2 * (h + tv))))));
	nodes.push_back(nodes.back() - (vUp * tv) - (vRight * th));
	// MIRROR
	{
		nodes.push_back(nodes.front() + reflection(nodes[5] - nodes.front(), vUp));
		nodes.push_back(nodes.front() + reflection(nodes[4] - nodes.front(), vUp));
		nodes.push_back(nodes.front() + reflection(nodes[3] - nodes.front(), vUp));
		nodes.push_back(nodes.front() + reflection(nodes[2] - nodes.front(), vUp));
		nodes.push_back(nodes.front() + reflection(nodes[1] - nodes.front(), vUp));
	}

	// Space View
	CRAB::Matrix4 ModelMatrix = toWorld(this->alignment->getPosition(t), this->alignment->getTangent(t), this->getNormal(t));
	for (int i = 0; i < nodes.size(); i++)
		nodes[i] = ModelMatrix * nodes[i];

	return nodes;
}

// UPDATE THE MODEL
// ----------------
void BoxGirder::UpdatePiers()
{
	for (int i = 0; i < this->piers.size(); i++)
	{
		this->piers[i].dir = CRAB::rotateY(this->piers[i].ang) * this->alignment->getTangentFromStation(this->piers[i].station);
		this->piers[i].base = this->alignment->getPositionFromStation(this->piers[i].station);

		if (this->piers[i].base.y > this->EL)
			this->piers[i].base.y = this->EL;
		this->piers[i].base.y -= this->piers[i].depth; // topo do bloco
		CRAB::Vector4Df top = this->alignment->getPositionFromStation(this->piers[i].station);
		this->piers[i].L = (top - this->piers[i].base).length() - this->H;

		// UPDATE span vector
		//this->span_vector[i + 1] = this->piers[i].station - this->alignment->profile.front()->getStart4DPoint().x;
		this->span_vector[i].end = this->piers[i].station - this->alignment->profile.front()->getStart4DPoint().x - this->piers[i].h / 2.0f;
		this->span_vector[i + 1].start = this->piers[i].station - this->alignment->profile.front()->getStart4DPoint().x + this->piers[i].h / 2.0f;
		
		// configura as extremidades do vertor de vãos com valores fora da extensão total para garantir H / 2 (balanços)
		/*if (i == 0)
			this->span_vector.front() = -this->span_vector[i + 1];
		else if (i == this->piers.size() - 1)
			this->span_vector.back() = -this->span_vector[i + 1];*/
	}
}
//void BoxGirder::OLD_Update()
//{
//	// Initialize
//	model.clear();
//	CRAB::Vector4Df newVertex, next_position, start_point;
//	float offset; // displacement
//	float segment_L; // the length of the line segment
//
//	// POLYGON FACE (GL_TRIANGLE_FAN)
//	// ------------------------------
//
//	// Local axis
//	CRAB::Vector4Df vUp = this->getNormal(0.0f);
//	CRAB::Vector4Df vRight = cross(this->alignment->getTangent(0.0f), vUp).to_unitary();
//
//#pragma region TOP_LAYER
//	// v0
//	start_point = alignment->getPosition(0.0f);// -(vUp * TOP_LAYER);
//	EulerOp::mvfs(model, start_point);
//	model.back()->name = "TOP_LAYER";
//	model.back()->material = { 0.1f, 0.1f, 0.1f, 1.0f };
//	// v1
//	newVertex = model.back()->vertices.back()->point - (vRight * (B / 2.0f - GUARD_RAIL)) - (vUp * (B / 2.0f - GUARD_RAIL) * SLOPE);
//	EulerOp::mev(model.back()->faces[0]->hEdge, NULL, 0, newVertex);
//	// v2
//	newVertex = model.back()->vertices.back()->point - (vUp * TOP_LAYER);
//	EulerOp::mev(model.back()->halfEdges[0], NULL, 1, newVertex);
//	// v3
//	newVertex = model.back()->vertices.back()->point + (vRight * (B - 2.0f * GUARD_RAIL));
//	EulerOp::mev(model.back()->halfEdges[2], NULL, 2, newVertex);
//	// v4
//	newVertex = model.back()->vertices.back()->point + (vUp * TOP_LAYER);
//	EulerOp::mev(model.back()->halfEdges[4], NULL, 3, newVertex);
//	// f1
//	EulerOp::mef(model.back()->halfEdges[0], model.back()->halfEdges[7], 0);
//
//	// SWEEP
//	EulerOp::SWEEP(model.back()->faces.front(), this);
//#pragma endregion TOP_LAYER
//
//#pragma region DECK
//	// OFFSET
//	offset = (B / 2.0f - GUARD_RAIL) * SLOPE + TOP_LAYER;
//	// v0
//	start_point = alignment->getPosition(0.0f) - (vUp * offset);
//	EulerOp::mvfs(model, start_point);
//	model.back()->name = "DECK";
//	model.back()->material = { 0.8f, 0.8f, 0.8f, 1.0f };
//	// v1
//	newVertex = model.back()->vertices.back()->point - (vRight * (B / 2.0f - GUARD_RAIL));
//	EulerOp::mev(model.back()->faces[0]->hEdge, NULL, 0, newVertex);
//
//	// Left Guard-Rail
//	{
//		// V2
//		newVertex = model.back()->vertices.back()->point + (vUp * 0.15f);
//		EulerOp::mev(model.back()->halfEdges[0], NULL, 1, newVertex);
//		// V3
//		newVertex = model.back()->vertices.back()->point + (vUp * 0.25f) - (vRight * 0.175f);
//		EulerOp::mev(model.back()->halfEdges[2], NULL, 2, newVertex);
//		// V4
//		newVertex = model.back()->vertices.back()->point + (vUp * 0.47f) - (vRight * 0.05f);
//		EulerOp::mev(model.back()->halfEdges[4], NULL, 3, newVertex);
//		// V5
//		newVertex = model.back()->vertices.back()->point - (vRight * 0.175f);
//		EulerOp::mev(model.back()->halfEdges[6], NULL, 4, newVertex);
//		// V6
//		newVertex = model.back()->vertices.back()->point - (vUp * 1.12f);
//		EulerOp::mev(model.back()->halfEdges[8], NULL, 5, newVertex);
//		// V7
//		newVertex = model.back()->vertices.back()->point - (vUp * 0.05f) + (vRight * 0.05f);
//		EulerOp::mev(model.back()->halfEdges[10], NULL, 6, newVertex);
//		// V8
//		newVertex = model.back()->vertices.back()->point + (vUp * 0.10f) + (vRight * 0.35f);
//		EulerOp::mev(model.back()->halfEdges[12], NULL, 7, newVertex);
//	}
//	// v9
//	newVertex = model.back()->vertices.back()->point - (vUp * (h + tv - 0.20f)) + (vRight * (Lb - GUARD_RAIL));
//	EulerOp::mev(model.back()->halfEdges[14], NULL, 8, newVertex);
//	// v10
//	float hyp = sqrtf(powf(INCLINATION_RATIO * bw, 2.0f) + powf(bw, 2.0f)); // Hypotenuse = top width of beam
//	newVertex = model.back()->vertices.back()->point + (vRight * hyp);
//	EulerOp::mev(model.back()->halfEdges[16], NULL, 9, newVertex);
//	// v11
//	newVertex = model.back()->vertices.back()->point + (vUp * tv) + (vRight * th);
//	EulerOp::mev(model.back()->halfEdges[18], NULL, 10, newVertex);
//
//	// MIRROR
//	{
//		// v12
//		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[11]->point - model.back()->vertices[0]->point, vUp);
//		EulerOp::mev(model.back()->halfEdges[20], NULL, 11, newVertex);
//		// v13
//		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[10]->point - model.back()->vertices[0]->point, vUp);
//		EulerOp::mev(model.back()->halfEdges[22], NULL, 12, newVertex);
//		// v14
//		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[9]->point - model.back()->vertices[0]->point, vUp);
//		EulerOp::mev(model.back()->halfEdges[24], NULL, 13, newVertex);
//		// v15
//		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[8]->point - model.back()->vertices[0]->point, vUp);
//		EulerOp::mev(model.back()->halfEdges[26], NULL, 14, newVertex);
//		// v16
//		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[7]->point - model.back()->vertices[0]->point, vUp);
//		EulerOp::mev(model.back()->halfEdges[28], NULL, 15, newVertex);
//		// v17
//		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[6]->point - model.back()->vertices[0]->point, vUp);
//		EulerOp::mev(model.back()->halfEdges[30], NULL, 16, newVertex);
//		// v18
//		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[5]->point - model.back()->vertices[0]->point, vUp);
//		EulerOp::mev(model.back()->halfEdges[32], NULL, 17, newVertex);
//		// v19
//		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[4]->point - model.back()->vertices[0]->point, vUp);
//		EulerOp::mev(model.back()->halfEdges[34], NULL, 18, newVertex);
//		// v20
//		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[3]->point - model.back()->vertices[0]->point, vUp);
//		EulerOp::mev(model.back()->halfEdges[36], NULL, 19, newVertex);
//		// v21
//		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[2]->point - model.back()->vertices[0]->point, vUp);
//		EulerOp::mev(model.back()->halfEdges[38], NULL, 20, newVertex);
//		// v22
//		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[1]->point - model.back()->vertices[0]->point, vUp);
//		EulerOp::mev(model.back()->halfEdges[40], NULL, 21, newVertex);
//	}
//
//	// f1
//	EulerOp::mef(model.back()->halfEdges[0], model.back()->halfEdges[43], 0);
//
//	// SWEEP
//	EulerOp::SWEEP(model.back()->faces.front(), this);
//
//#pragma endregion DECK
//
//#pragma region U_SECTION
//	// OFFSET
//	offset = (B / 2.0f - GUARD_RAIL) * SLOPE + H + TOP_LAYER;
//	// v0
//	start_point = alignment->getPosition(0.0f) - (vUp * offset);
//	EulerOp::mvfs(model, start_point);
//	model.back()->name = "U_SECTION";
//	model.back()->material = { 0.8f, 0.8f, 0.8f, 1.0f };
//	// v1
//	newVertex = model.back()->vertices.back()->point + (vRight * (b / 2.0f));
//	EulerOp::mev(model.back()->faces[0]->hEdge, NULL, 0, newVertex);
//	// v2
//	newVertex = model.back()->vertices.back()->point + (vRight * (B - b - 2 * Lb) / 2.0f) + (vUp * (H - h - tv));
//	EulerOp::mev(model.back()->halfEdges[0], NULL, 1, newVertex);
//	// v3
//	newVertex = model.back()->vertices.back()->point - (vRight * hyp);
//	EulerOp::mev(model.back()->halfEdges[2], NULL, 2, newVertex);
//	// v4
//	newVertex = model.back()->vertices.back()->point - (vUp * (H - 2 * (h + tv))) - (vRight * (INCLINATION_RATIO * ((H - 2 * (h + tv)))));
//	EulerOp::mev(model.back()->halfEdges[4], NULL, 3, newVertex);
//	// v5
//	newVertex = model.back()->vertices.back()->point - (vUp * tv) - (vRight * th);
//	EulerOp::mev(model.back()->halfEdges[6], NULL, 4, newVertex);
//	// MIRROR
//	// v6
//	newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[5]->point - model.back()->vertices[0]->point, vUp);
//	EulerOp::mev(model.back()->halfEdges[8], NULL, 5, newVertex);
//	// v7
//	newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[4]->point - model.back()->vertices[0]->point, vUp);
//	EulerOp::mev(model.back()->halfEdges[10], NULL, 6, newVertex);
//	// v8
//	newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[3]->point - model.back()->vertices[0]->point, vUp);
//	EulerOp::mev(model.back()->halfEdges[12], NULL, 7, newVertex);
//	// v9
//	newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[2]->point - model.back()->vertices[0]->point, vUp);
//	EulerOp::mev(model.back()->halfEdges[14], NULL, 8, newVertex);
//	// v10
//	newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[1]->point - model.back()->vertices[0]->point, vUp);
//	EulerOp::mev(model.back()->halfEdges[16], NULL, 9, newVertex);
//	// f1
//	EulerOp::mef(model.back()->halfEdges[0], model.back()->halfEdges[19], 0);
//
//	// SWEEP
//	EulerOp::SWEEP(model.back()->faces.front(), this);
//
//#pragma endregion U_SECTION
//
//#pragma region PIERS
//	
//	for (int i = 0; i < piers.size(); i++)
//	{
//		// UPDATE Local axis
//		CRAB::Vector4Df WorldUp = { 0.0f, 1.0f, 0.0f, 0.0f };
//		vRight = cross(piers[i].dir, WorldUp).to_unitary();
//		// v0
//		start_point = piers[i].base + (piers[i].dir * (piers[i].h / 2.0f)) + (vRight * (piers[i].b / 2.0f));
//		EulerOp::mvfs(model, start_point);
//		model.back()->name = "PIER";
//		model.back()->material = { 0.8f, 0.8f, 0.8f, 1.0f };
//		// v1
//		newVertex = model.back()->vertices.back()->point - (vRight * piers[i].b);
//		EulerOp::mev(model.back()->faces[0]->hEdge, NULL, 0, newVertex);
//		// v2
//		newVertex = model.back()->vertices.back()->point - (piers[i].dir * piers[i].h);
//		EulerOp::mev(model.back()->halfEdges[0], NULL, 1, newVertex);
//		// v3
//		newVertex = model.back()->vertices.back()->point + (vRight * piers[i].b);
//		EulerOp::mev(model.back()->halfEdges[2], NULL, 2, newVertex);
//		// f1
//		EulerOp::mef(model.back()->halfEdges[0], model.back()->halfEdges[5], 0);
//
//		// EXTRUDE
//		EulerOp::EXTRUDE(model.back()->faces.front(), WorldUp, piers[i].L);
//	}
//#pragma endregion 
//
//#pragma region ABUTMENTS
//	/* ================= START ABUTMENT ================= */
//
//	// UPDATE Local axis
//	vUp = this->abutments.front()->getNormalUp(0.0f);
//	vRight = cross(this->abutments.front()->getTangent(0.0f), vUp).to_unitary();
//	// v0
//	start_point = this->abutments.front()->getPosition(0.0f);// -(vUp * TOP_LAYER);
//	EulerOp::mvfs(model, start_point);
//	model.back()->name = "TOP_LAYER";
//	model.back()->material = { 0.1f, 0.1f, 0.1f, 1.0f };
//	// v1
//	newVertex = model.back()->vertices.back()->point - (vRight * (B / 2.0f - GUARD_RAIL)) - (vUp * (B / 2.0f - GUARD_RAIL) * SLOPE);
//	EulerOp::mev(model.back()->faces[0]->hEdge, NULL, 0, newVertex);
//	// v2
//	newVertex = model.back()->vertices.back()->point - (vUp * TOP_LAYER);
//	EulerOp::mev(model.back()->halfEdges[0], NULL, 1, newVertex);
//	// v3
//	newVertex = model.back()->vertices.back()->point + (vRight * (B - 2.0f * GUARD_RAIL));
//	EulerOp::mev(model.back()->halfEdges[2], NULL, 2, newVertex);
//	// v4
//	newVertex = model.back()->vertices.back()->point + (vUp * TOP_LAYER);
//	EulerOp::mev(model.back()->halfEdges[4], NULL, 3, newVertex);
//	// f1
//	EulerOp::mef(model.back()->halfEdges[0], model.back()->halfEdges[7], 0);
//	// SWEEP
//	EulerOp::SWEEP(model.back()->faces.front(), this->abutments.front());
//
//	// ROAD LEVEL
//	float start_station = this->abutments.front()->profile.front()->getStart4DPoint().x;
//	float base_level = this->road->alignment->getPositionFromStation(start_station).y - 0.5f;
//	//std::cout << "base_level = " << base_level << std::endl;
//	// OFFSET
//	offset = (B / 2.0f - GUARD_RAIL) * SLOPE + TOP_LAYER;
//	// v0
//	start_point = this->abutments.front()->getPosition(0.0f) - (vUp * offset);
//	EulerOp::mvfs(model, start_point);
//	model.back()->name = "START_ABUTMENT";
//	model.back()->material = { 0.8f, 0.8f, 0.8f, 1.0f };
//	// v1
//	newVertex = model.back()->vertices.back()->point - (vRight * (B / 2.0f - GUARD_RAIL));
//	EulerOp::mev(model.back()->faces[0]->hEdge, NULL, 0, newVertex);
//	// Left Guard-Rail
//	{
//		// V2
//		newVertex = model.back()->vertices.back()->point + (vUp * 0.15f);
//		EulerOp::mev(model.back()->halfEdges[0], NULL, 1, newVertex);
//		// V3
//		newVertex = model.back()->vertices.back()->point + (vUp * 0.25f) - (vRight * 0.175f);
//		EulerOp::mev(model.back()->halfEdges[2], NULL, 2, newVertex);
//		// V4
//		newVertex = model.back()->vertices.back()->point + (vUp * 0.47f) - (vRight * 0.05f);
//		EulerOp::mev(model.back()->halfEdges[4], NULL, 3, newVertex);
//		// V5
//		newVertex = model.back()->vertices.back()->point - (vRight * 0.175f);
//		EulerOp::mev(model.back()->halfEdges[6], NULL, 4, newVertex);
//		// V6
//		newVertex = model.back()->vertices.back()->point;
//		newVertex.y = base_level;
//		//newVertex -= vUp * 0.5f;
//		EulerOp::mev(model.back()->halfEdges[8], NULL, 5, newVertex);
//	}
//	// MIRROR
//	{
//		// v7
//		newVertex = model.back()->vertices.front()->point + reflection(model.back()->vertices[6]->point - model.back()->vertices.front()->point, vUp);
//		EulerOp::mev(model.back()->halfEdges[10], NULL, 6, newVertex);
//		// v8
//		newVertex = model.back()->vertices.front()->point + reflection(model.back()->vertices[5]->point - model.back()->vertices.front()->point, vUp);
//		EulerOp::mev(model.back()->halfEdges[12], NULL, 7, newVertex);
//		// v9
//		newVertex = model.back()->vertices.front()->point + reflection(model.back()->vertices[4]->point - model.back()->vertices.front()->point, vUp);
//		EulerOp::mev(model.back()->halfEdges[14], NULL, 8, newVertex);
//		// v10
//		newVertex = model.back()->vertices.front()->point + reflection(model.back()->vertices[3]->point - model.back()->vertices.front()->point, vUp);
//		EulerOp::mev(model.back()->halfEdges[16], NULL, 9, newVertex);
//		// v11
//		newVertex = model.back()->vertices.front()->point + reflection(model.back()->vertices[2]->point - model.back()->vertices.front()->point, vUp);
//		EulerOp::mev(model.back()->halfEdges[18], NULL, 10, newVertex);
//		// v12
//		newVertex = model.back()->vertices.front()->point + reflection(model.back()->vertices[1]->point - model.back()->vertices.front()->point, vUp);
//		EulerOp::mev(model.back()->halfEdges[20], NULL, 11, newVertex);
//	}
//	// f1
//	EulerOp::mef(model.back()->halfEdges.front(), model.back()->halfEdges[23], 0);
//	// SWEEP
//	EulerOp::SWEEP_for_Abutments(model.back()->faces.front(), this->abutments.front(), base_level);
//
//	/* ================= END ABUTMENT ================= */
//
//	// UPDATE Local axis
//	vUp = this->abutments.back()->getNormalUp(0.0f);
//	vRight = cross(this->abutments.back()->getTangent(0.0f), vUp).to_unitary();
//	// v0
//	start_point = this->abutments.back()->getPosition(0.0f);// -(vUp * TOP_LAYER);
//	EulerOp::mvfs(model, start_point);
//	model.back()->name = "TOP_LAYER";
//	model.back()->material = { 0.1f, 0.1f, 0.1f, 1.0f };
//	// v1
//	newVertex = model.back()->vertices.back()->point - (vRight * (B / 2.0f - GUARD_RAIL)) - (vUp * (B / 2.0f - GUARD_RAIL) * SLOPE);
//	EulerOp::mev(model.back()->faces[0]->hEdge, NULL, 0, newVertex);
//	// v2
//	newVertex = model.back()->vertices.back()->point - (vUp * TOP_LAYER);
//	EulerOp::mev(model.back()->halfEdges[0], NULL, 1, newVertex);
//	// v3
//	newVertex = model.back()->vertices.back()->point + (vRight * (B - 2.0f * GUARD_RAIL));
//	EulerOp::mev(model.back()->halfEdges[2], NULL, 2, newVertex);
//	// v4
//	newVertex = model.back()->vertices.back()->point + (vUp * TOP_LAYER);
//	EulerOp::mev(model.back()->halfEdges[4], NULL, 3, newVertex);
//	// f1
//	EulerOp::mef(model.back()->halfEdges[0], model.back()->halfEdges[7], 0);
//	// SWEEP
//	EulerOp::SWEEP(model.back()->faces.front(), this->abutments.back());
//
//	// ROAD LEVEL
//	start_station = this->abutments.back()->profile.front()->getStart4DPoint().x;
//	base_level = this->road->alignment->getPositionFromStation(start_station).y - 0.5f;
//	// OFFSET
//	offset = (B / 2.0f - GUARD_RAIL) * SLOPE + TOP_LAYER;
//	// v0
//	start_point = this->abutments.back()->getPosition(0.0f) - (vUp * offset);
//	EulerOp::mvfs(model, start_point);
//	model.back()->name = "END_ABUTMENT";
//	model.back()->material = { 0.8f, 0.8f, 0.8f, 1.0f };
//	// v1
//	newVertex = model.back()->vertices.back()->point - (vRight * (B / 2.0f - GUARD_RAIL));
//	EulerOp::mev(model.back()->faces[0]->hEdge, NULL, 0, newVertex);
//	// Left Guard-Rail
//	{
//		// V2
//		newVertex = model.back()->vertices.back()->point + (vUp * 0.15f);
//		EulerOp::mev(model.back()->halfEdges[0], NULL, 1, newVertex);
//		// V3
//		newVertex = model.back()->vertices.back()->point + (vUp * 0.25f) - (vRight * 0.175f);
//		EulerOp::mev(model.back()->halfEdges[2], NULL, 2, newVertex);
//		// V4
//		newVertex = model.back()->vertices.back()->point + (vUp * 0.47f) - (vRight * 0.05f);
//		EulerOp::mev(model.back()->halfEdges[4], NULL, 3, newVertex);
//		// V5
//		newVertex = model.back()->vertices.back()->point - (vRight * 0.175f);
//		EulerOp::mev(model.back()->halfEdges[6], NULL, 4, newVertex);
//		// V6
//		newVertex = model.back()->vertices.back()->point;
//		newVertex.y = base_level;
//		//newVertex -= vUp * 0.5f;
//		EulerOp::mev(model.back()->halfEdges[8], NULL, 5, newVertex);
//	}
//	// MIRROR
//	{
//		// v7
//		newVertex = model.back()->vertices.front()->point + reflection(model.back()->vertices[6]->point - model.back()->vertices.front()->point, vUp);
//		EulerOp::mev(model.back()->halfEdges[10], NULL, 6, newVertex);
//		// v8
//		newVertex = model.back()->vertices.front()->point + reflection(model.back()->vertices[5]->point - model.back()->vertices.front()->point, vUp);
//		EulerOp::mev(model.back()->halfEdges[12], NULL, 7, newVertex);
//		// v9
//		newVertex = model.back()->vertices.front()->point + reflection(model.back()->vertices[4]->point - model.back()->vertices.front()->point, vUp);
//		EulerOp::mev(model.back()->halfEdges[14], NULL, 8, newVertex);
//		// v10
//		newVertex = model.back()->vertices.front()->point + reflection(model.back()->vertices[3]->point - model.back()->vertices.front()->point, vUp);
//		EulerOp::mev(model.back()->halfEdges[16], NULL, 9, newVertex);
//		// v11
//		newVertex = model.back()->vertices.front()->point + reflection(model.back()->vertices[2]->point - model.back()->vertices.front()->point, vUp);
//		EulerOp::mev(model.back()->halfEdges[18], NULL, 10, newVertex);
//		// v12
//		newVertex = model.back()->vertices.front()->point + reflection(model.back()->vertices[1]->point - model.back()->vertices.front()->point, vUp);
//		EulerOp::mev(model.back()->halfEdges[20], NULL, 11, newVertex);
//	}
//	// f1
//	EulerOp::mef(model.back()->halfEdges.front(), model.back()->halfEdges[23], 0);
//	// SWEEP
//	EulerOp::SWEEP_for_Abutments(model.back()->faces.front(), this->abutments.back(), base_level);
//#pragma endregion ABUTMENTS
//}
void BoxGirder::Update()
{
	// Initialize
	model.clear();

	// TOP_LAYER
	{
		std::vector<CRAB::Vector4Df> cross_section = this->TopLayer_section(0);
		EulerOp::mvfs(model, cross_section.front());
		model.back()->name = "TOP_LAYER";
		model.back()->material = { 0.1f, 0.1f, 0.1f, 1.0f };

		// First face (sentido inverso, pois a normal da primeira face aponta pro sentido contrário à tangente da curva paramétrica)
		std::reverse(cross_section.begin(), cross_section.end());
		int id_HalfEdge = 0;
		for (int i = 0; i < cross_section.size() - 1; i++)
		{
			if (i == 0)
			{
				EulerOp::mev(model.back()->faces.front()->hEdge, NULL, i, cross_section[i]);
				continue;
			}
			EulerOp::mev(model.back()->halfEdges[id_HalfEdge], NULL, i, cross_section[i]);
			id_HalfEdge += 2;
		}
		EulerOp::mef(model.back()->halfEdges.front(), model.back()->halfEdges.back(), 0);

		// Sweep
		for (int i = 1; i <= ELEMENTS; i++)
		{
			// Current position
			float t = float(i) / ELEMENTS;
			// Next section
			std::vector<CRAB::Vector4Df> new_section = this->TopLayer_section(t);
			// Solid
			EulerOp::SWEEP(model.back()->faces.front(), new_section);
		}
	}
	
	// DECK
	{
		std::vector<CRAB::Vector4Df> cross_section = this->Deck_section(0.0f);
		EulerOp::mvfs(model, cross_section.front());
		model.back()->name = "DECK";
		model.back()->material = { 0.8f, 0.8f, 0.8f, 1.0f };

		// First face (sentido inverso, pois a normal da primeira face aponta pro sentido contrário à tangente da curva paramétrica)
		std::reverse(cross_section.begin(), cross_section.end());
		int id_HalfEdge = 0;
		for (int i = 0; i < cross_section.size() - 1; i++)
		{
			if (i == 0)
			{
				EulerOp::mev(model.back()->faces.front()->hEdge, NULL, i, cross_section[i]);
				continue;
			}
			EulerOp::mev(model.back()->halfEdges[id_HalfEdge], NULL, i, cross_section[i]);
			id_HalfEdge += 2;
		}
		EulerOp::mef(model.back()->halfEdges.front(), model.back()->halfEdges.back(), 0);

		// Sweep
		for (int i = 1; i <= /*ELEMENTS*/200; i++)
		{
			// Current position
			float t = float(i) / /*ELEMENTS*/200;
			// Next section
			std::vector<CRAB::Vector4Df> new_section = this->Deck_section(t);
			// Solid
			EulerOp::SWEEP(model.back()->faces.front(), new_section);
		}			
	}

	// U_SECTION
	{
		std::vector<CRAB::Vector4Df> cross_section = this->U_section(0.0f);
		EulerOp::mvfs(model, cross_section.front());
		model.back()->name = "U_SECTION";
		model.back()->material = { 0.8f, 0.8f, 0.8f, 1.0f };

		// First face (sentido inverso, pois a normal da primeira face aponta pro sentido contrário à tangente da curva paramétrica)
		std::reverse(cross_section.begin(), cross_section.end());
		int id_HalfEdge = 0;
		for (int i = 0; i < cross_section.size() - 1; i++)
		{
			if (i == 0)
			{
				EulerOp::mev(model.back()->faces.front()->hEdge, NULL, i, cross_section[i]);
				continue;
			}
			EulerOp::mev(model.back()->halfEdges[id_HalfEdge], NULL, i, cross_section[i]);
			id_HalfEdge += 2;
		}
		EulerOp::mef(model.back()->halfEdges.front(), model.back()->halfEdges.back(), 0);

		// Sweep
		for (int i = 1; i <= /*ELEMENTS*/200; i++)
		{
			// Current position
			float t = float(i) / /*ELEMENTS*/200;
			// Next section
			std::vector<CRAB::Vector4Df> new_section = this->U_section(t);
			// Solid
			EulerOp::SWEEP(model.back()->faces.front(), new_section);
		}
	}

	// PIERS
	{
		for (int i = 0; i < piers.size(); i++)
		{
			// UPDATE Local axis
			CRAB::Vector4Df WorldUp = { 0.0f, 1.0f, 0.0f, 0.0f };
			CRAB::Vector4Df vRight = cross(piers[i].dir, WorldUp).to_unitary();
			// v0
			CRAB::Vector4Df start_point = piers[i].base + (piers[i].dir * (piers[i].h / 2.0f)) + (vRight * (piers[i].b / 2.0f));
			EulerOp::mvfs(model, start_point);
			model.back()->name = "PIER";
			model.back()->material = { 0.8f, 0.8f, 0.8f, 1.0f };
			// v1
			CRAB::Vector4Df newVertex = model.back()->vertices.back()->point - (vRight * piers[i].b);
			EulerOp::mev(model.back()->faces.front()->hEdge, NULL, 0, newVertex);
			// v2
			newVertex = model.back()->vertices.back()->point - (piers[i].dir * piers[i].h);
			EulerOp::mev(model.back()->halfEdges[0], NULL, 1, newVertex);
			// v3
			newVertex = model.back()->vertices.back()->point + (vRight * piers[i].b);
			EulerOp::mev(model.back()->halfEdges[2], NULL, 2, newVertex);
			// f1
			EulerOp::mef(model.back()->halfEdges.front(), model.back()->halfEdges[5], 0);

			// EXTRUDE
			EulerOp::EXTRUDE(model.back()->faces.front(), WorldUp, piers[i].L);
		}
	}

#pragma region ABUTMENTS
	{
		///* ================= START ABUTMENT ================= */

		//// UPDATE Local axis
		//CRAB::Vector4Df vUp = this->abutments.front()->getNormalUp(0.0f);
		//CRAB::Vector4Df vRight = cross(this->abutments.front()->getTangent(0.0f), vUp).to_unitary();
		//// v0
		//CRAB::Vector4Df start_point = this->abutments.front()->getPosition(0.0f);// -(vUp * TOP_LAYER);
		//EulerOp::mvfs(model, start_point);
		//model.back()->name = "TOP_LAYER";
		//model.back()->material = { 0.1f, 0.1f, 0.1f, 1.0f };
		//// v1
		//CRAB::Vector4Df newVertex = model.back()->vertices.back()->point - (vRight * (B / 2.0f)) - (vUp * (B / 2.0f) * SLOPE);
		//EulerOp::mev(model.back()->faces.front()->hEdge, NULL, 0, newVertex);
		//// v2
		//newVertex = model.back()->vertices.back()->point - (vUp * TOP_LAYER);
		//EulerOp::mev(model.back()->halfEdges[0], NULL, 1, newVertex);
		//// v3
		//newVertex = model.back()->vertices.back()->point + (vRight * B);
		//EulerOp::mev(model.back()->halfEdges[2], NULL, 2, newVertex);
		//// v4
		//newVertex = model.back()->vertices.back()->point + (vUp * TOP_LAYER);
		//EulerOp::mev(model.back()->halfEdges[4], NULL, 3, newVertex);
		//// f1
		//EulerOp::mef(model.back()->halfEdges.front(), model.back()->halfEdges[7], 0);
		//// SWEEP
		//EulerOp::SWEEP(model.back()->faces.front(), this->abutments.front());

		//// ROAD LEVEL
		//float start_station = this->abutments.front()->profile.front()->getStart4DPoint().x;
		//float base_level = this->road->alignment->getPositionFromStation(start_station).y - 0.5f;
		////std::cout << "base_level = " << base_level << std::endl;
		//// OFFSET
		//float offset = (B / 2.0f) * SLOPE + TOP_LAYER;
		//// v0
		//start_point = this->abutments.front()->getPosition(0.0f) - (vUp * offset);
		//EulerOp::mvfs(model, start_point);
		//model.back()->name = "START_ABUTMENT";
		//model.back()->material = { 0.8f, 0.8f, 0.8f, 1.0f };
		//// v1
		//newVertex = model.back()->vertices.back()->point - (vRight * (B / 2.0f));
		//EulerOp::mev(model.back()->faces[0]->hEdge, NULL, 0, newVertex);
		//// Left Guard-Rail
		//{
		//	// V2
		//	newVertex = model.back()->vertices.back()->point + (vUp * 0.15f);
		//	EulerOp::mev(model.back()->halfEdges[0], NULL, 1, newVertex);
		//	// V3
		//	newVertex = model.back()->vertices.back()->point + (vUp * 0.25f) - (vRight * 0.175f);
		//	EulerOp::mev(model.back()->halfEdges[2], NULL, 2, newVertex);
		//	// V4
		//	newVertex = model.back()->vertices.back()->point + (vUp * 0.47f) - (vRight * 0.05f);
		//	EulerOp::mev(model.back()->halfEdges[4], NULL, 3, newVertex);
		//	// V5
		//	newVertex = model.back()->vertices.back()->point - (vRight * 0.175f);
		//	EulerOp::mev(model.back()->halfEdges[6], NULL, 4, newVertex);
		//	// V6
		//	newVertex = model.back()->vertices.back()->point;
		//	newVertex.y = base_level;
		//	//newVertex -= vUp * 0.5f;
		//	EulerOp::mev(model.back()->halfEdges[8], NULL, 5, newVertex);
		//}
		//// MIRROR
		//{
		//	// v7
		//	newVertex = model.back()->vertices.front()->point + reflection(model.back()->vertices[6]->point - model.back()->vertices.front()->point, vUp);
		//	EulerOp::mev(model.back()->halfEdges[10], NULL, 6, newVertex);
		//	// v8
		//	newVertex = model.back()->vertices.front()->point + reflection(model.back()->vertices[5]->point - model.back()->vertices.front()->point, vUp);
		//	EulerOp::mev(model.back()->halfEdges[12], NULL, 7, newVertex);
		//	// v9
		//	newVertex = model.back()->vertices.front()->point + reflection(model.back()->vertices[4]->point - model.back()->vertices.front()->point, vUp);
		//	EulerOp::mev(model.back()->halfEdges[14], NULL, 8, newVertex);
		//	// v10
		//	newVertex = model.back()->vertices.front()->point + reflection(model.back()->vertices[3]->point - model.back()->vertices.front()->point, vUp);
		//	EulerOp::mev(model.back()->halfEdges[16], NULL, 9, newVertex);
		//	// v11
		//	newVertex = model.back()->vertices.front()->point + reflection(model.back()->vertices[2]->point - model.back()->vertices.front()->point, vUp);
		//	EulerOp::mev(model.back()->halfEdges[18], NULL, 10, newVertex);
		//	// v12
		//	newVertex = model.back()->vertices.front()->point + reflection(model.back()->vertices[1]->point - model.back()->vertices.front()->point, vUp);
		//	EulerOp::mev(model.back()->halfEdges[20], NULL, 11, newVertex);
		//}
		//// f1
		//EulerOp::mef(model.back()->halfEdges.front(), model.back()->halfEdges[23], 0);
		//// SWEEP
		//EulerOp::SWEEP_for_Abutments(model.back()->faces.front(), this->abutments.front(), base_level);

		///* ================= END ABUTMENT ================= */

		//// UPDATE Local axis
		//vUp = this->abutments.back()->getNormalUp(0.0f);
		//vRight = cross(this->abutments.back()->getTangent(0.0f), vUp).to_unitary();
		//// v0
		//start_point = this->abutments.back()->getPosition(0.0f);// -(vUp * TOP_LAYER);
		//EulerOp::mvfs(model, start_point);
		//model.back()->name = "TOP_LAYER";
		//model.back()->material = { 0.1f, 0.1f, 0.1f, 1.0f };
		//// v1
		//newVertex = model.back()->vertices.back()->point - (vRight * (B / 2.0f)) - (vUp * (B / 2.0f) * SLOPE);
		//EulerOp::mev(model.back()->faces.front()->hEdge, NULL, 0, newVertex);
		//// v2
		//newVertex = model.back()->vertices.back()->point - (vUp * TOP_LAYER);
		//EulerOp::mev(model.back()->halfEdges[0], NULL, 1, newVertex);
		//// v3
		//newVertex = model.back()->vertices.back()->point + (vRight * B);
		//EulerOp::mev(model.back()->halfEdges[2], NULL, 2, newVertex);
		//// v4
		//newVertex = model.back()->vertices.back()->point + (vUp * TOP_LAYER);
		//EulerOp::mev(model.back()->halfEdges[4], NULL, 3, newVertex);
		//// f1
		//EulerOp::mef(model.back()->halfEdges.front(), model.back()->halfEdges[7], 0);
		//// SWEEP
		//EulerOp::SWEEP(model.back()->faces.front(), this->abutments.back());

		//// ROAD LEVEL
		//start_station = this->abutments.back()->profile.front()->getStart4DPoint().x;
		//base_level = this->road->alignment->getPositionFromStation(start_station).y - 0.5f;
		//// OFFSET
		//offset = (B / 2.0f) * SLOPE + TOP_LAYER;
		//// v0
		//start_point = this->abutments.back()->getPosition(0.0f) - (vUp * offset);
		//EulerOp::mvfs(model, start_point);
		//model.back()->name = "END_ABUTMENT";
		//model.back()->material = { 0.8f, 0.8f, 0.8f, 1.0f };
		//// v1
		//newVertex = model.back()->vertices.back()->point - (vRight * (B / 2.0f));
		//EulerOp::mev(model.back()->faces.front()->hEdge, NULL, 0, newVertex);
		//// Left Guard-Rail
		//{
		//	// V2
		//	newVertex = model.back()->vertices.back()->point + (vUp * 0.15f);
		//	EulerOp::mev(model.back()->halfEdges[0], NULL, 1, newVertex);
		//	// V3
		//	newVertex = model.back()->vertices.back()->point + (vUp * 0.25f) - (vRight * 0.175f);
		//	EulerOp::mev(model.back()->halfEdges[2], NULL, 2, newVertex);
		//	// V4
		//	newVertex = model.back()->vertices.back()->point + (vUp * 0.47f) - (vRight * 0.05f);
		//	EulerOp::mev(model.back()->halfEdges[4], NULL, 3, newVertex);
		//	// V5
		//	newVertex = model.back()->vertices.back()->point - (vRight * 0.175f);
		//	EulerOp::mev(model.back()->halfEdges[6], NULL, 4, newVertex);
		//	// V6
		//	newVertex = model.back()->vertices.back()->point;
		//	newVertex.y = base_level;
		//	//newVertex -= vUp * 0.5f;
		//	EulerOp::mev(model.back()->halfEdges[8], NULL, 5, newVertex);
		//}
		//// MIRROR
		//{
		//	// v7
		//	newVertex = model.back()->vertices.front()->point + reflection(model.back()->vertices[6]->point - model.back()->vertices.front()->point, vUp);
		//	EulerOp::mev(model.back()->halfEdges[10], NULL, 6, newVertex);
		//	// v8
		//	newVertex = model.back()->vertices.front()->point + reflection(model.back()->vertices[5]->point - model.back()->vertices.front()->point, vUp);
		//	EulerOp::mev(model.back()->halfEdges[12], NULL, 7, newVertex);
		//	// v9
		//	newVertex = model.back()->vertices.front()->point + reflection(model.back()->vertices[4]->point - model.back()->vertices.front()->point, vUp);
		//	EulerOp::mev(model.back()->halfEdges[14], NULL, 8, newVertex);
		//	// v10
		//	newVertex = model.back()->vertices.front()->point + reflection(model.back()->vertices[3]->point - model.back()->vertices.front()->point, vUp);
		//	EulerOp::mev(model.back()->halfEdges[16], NULL, 9, newVertex);
		//	// v11
		//	newVertex = model.back()->vertices.front()->point + reflection(model.back()->vertices[2]->point - model.back()->vertices.front()->point, vUp);
		//	EulerOp::mev(model.back()->halfEdges[18], NULL, 10, newVertex);
		//	// v12
		//	newVertex = model.back()->vertices.front()->point + reflection(model.back()->vertices[1]->point - model.back()->vertices.front()->point, vUp);
		//	EulerOp::mev(model.back()->halfEdges[20], NULL, 11, newVertex);
		//}
		//// f1
		//EulerOp::mef(model.back()->halfEdges.front(), model.back()->halfEdges[23], 0);
		//// SWEEP
		//EulerOp::SWEEP_for_Abutments(model.back()->faces.front(), this->abutments.back(), base_level);
	}
#pragma endregion
}

// RETURN SECTION PARAMETERS
// -------------------------
float BoxGirder::get_CantileverLength() const
{
	return Lb;
}
float BoxGirder::get_DeckThickness() const
{
	return h;
}
float BoxGirder::get_WebThickness() const
{
	return bw;
}
float BoxGirder::get_HaunchHeight() const
{
	return tv;
}
float BoxGirder::get_BottomWidth() const
{
	return b;
}
float BoxGirder::get_HaunchWidth() const
{
	return th;
}

// ALLOCATION SECTION PARAMETERS
// -----------------------------
void BoxGirder::set_CantileverLength(const float& _Lb)
{
	Lb = _Lb;
}
void BoxGirder::set_DeckThickness(const float& _h)
{
	h = _h;
}
void BoxGirder::set_WebThickness(const float& _tw)
{
	bw = _tw;
}
void BoxGirder::set_HaunchHeight(const float& _hc)
{
	tv = _hc;
}
void BoxGirder::set_BottomWidth(const float& _b)
{
	b = _b;
}
void BoxGirder::set_HaunchWidth(const float& _bc)
{
	th = _bc;
}