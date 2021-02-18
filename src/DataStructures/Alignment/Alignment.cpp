#include "Alignment.h"


// DEFAULT CONSTRUCTOR
// -------------------
Alignment::Alignment()
{
}

// DEFAULT CONSTRUCTOR
// -------------------
Alignment::Alignment(const std::string& _name, const std::vector<HorSegment*>& _plan, const std::vector<VerSegment*>& _profile)
	: name(_name), plan(_plan), profile(_profile)
{
	std::cout << std::endl;
	std::cout << "\tNEW Alignment ............................ " << name << std::endl;

	// 2D Horizontal Curve CONSTRUCTOR
	// -------------------------------
	std::cout << "\tBuilding the horizontal NURBS curve" << std::endl;
	std::vector<Geometry*> hor2DSegments;
	for (int i = 0; i < this->plan.size(); i++)
	{
		hor2DSegments.push_back(this->plan[i]->segment);
	}
	this->path2Dh = NURBS(hor2DSegments);

	// Arc Length Table CONSTRUCTOR
	// ----------------------------
	/*std::cout << "\tBuilding the Arc Length Table of horizontal NURBS curve" << std::endl;
	float steps = int(this->getRoadLength());
	arcLength_table.push_back(ArcLength(0.0f, 0.0f));
	for (int i = 1; i <= steps; i++)
	{
		float ti = i / steps;
		float G = 0.0f;
		for (int k = 1; k <= 2; k++)
		{
			float w, alfa;
			if (k == 1)
			{
				w = 1.0f;
				alfa = -1.0f / sqrtf(3.0f);
			}
			else
			{
				w = 1.0f;
				alfa = 1.0f / sqrtf(3.0f);
			}
			float tk = 0.5f * ((ti - arcLength_table.back().t) * alfa + (ti + arcLength_table.back().t));
			CRAB::Vector4Df d1 = this->path2Dh.deriv_4D(tk);
			G += w * d1.length();
		}
		float si = arcLength_table.back().s + 0.5f * (ti - arcLength_table.back().t) * G;
		arcLength_table.push_back(ArcLength(ti, si));
	}*/

	//std::cout << "path 2D length = " << this->path2Dh.getLength() << " m" << std::endl;
	//std::cout << "path 2D length (from table) = " << this->arcLength_table.back().s << " m" << std::endl;

	// Start & End Stations
	// --------------------
	//float start_station = this->findParameter(this->profile.front()->getStartPoint().x);
	//float end_station = this->findParameter(this->profile.back()->getEndPoint().x);

	// 3D Curve CONSTRUCTOR
	// --------------------
	std::cout << "\tBuilding the 3D NURBS curve" << std::endl;
	std::vector<glm::vec3> points3D;
	// VERSÃO ANTIGA (CALCULANDO A DISTÂNCIA PERCORRIDA NA NURBS PELO COMPRIMENTO DOS SEGMENTOS DE CORDA)
	//for (int i = 0; i <= ELEMENTS; i++)
	//{
	//	float t = start_station + (end_station - start_station) * i / ELEMENTS;
	//	// coordenadas em planta (UTM)
	//	points3D.push_back(this->path2Dh.getPosition(t));
	//	// elevation (m)
	//	float distance = this->path2Dh.getDistance(t);
	//	int index = findSegment(distance);
	//	points3D.back().y = profile[index]->getY(distance);
	//}
	// VERSÃO NOVA (CRETO)
	for (int i = 0; i <= this->path2Dh.arcLength_table.size(); i++)
	{
		float t = float(i) / this->path2Dh.arcLength_table.size();
		// coordenadas em planta
		points3D.push_back(this->path2Dh.getPosition(t));
		// elevation (m)
		if (i == this->path2Dh.arcLength_table.size()) break;
		//std::cout << "i = " << i << "; distance = " << arcLength_table[i].s << " m" << std::endl;
		int index = findSegment(this->path2Dh.arcLength_table[i].s);
		if (index == -1) continue;
		points3D.back().y = profile[index]->getY(this->path2Dh.arcLength_table[i].s);
		//std::cout << "y = " << points3D.back().y << std::endl;
	}

	this->path3D = NURBS(points3D);
}

// DESTRUCTOR
// ----------
Alignment::~Alignment()
{
}

float Alignment::findParameter(const float& s) const
{
	//std::cout << "Alignment::findParameter(" << s << ")" << std::endl;

	/*if (distance >= this->path2Dh.getArcLength())
		return 1.0f;
	if (distance <= 0.0f)
		return 0.0f;

	float low = 0.0f;
	float high = 1.0f;
	float mid = (low + high) / 2.0f;

	while (distance < this->path2Dh.getDistance(mid) || distance >= this->path2Dh.getDistance(mid))
	{
		if (distance < this->path2Dh.getDistance(mid))
			high = mid;
		else low = mid;
		mid = (low + high) / 2.0f;
		if (fabsf(low - mid) < SMALL_NUMBER)
			return mid;
	}
	return mid;*/

	if (s <= 0.0f)
		return 0.0f;
	if (s >= this->path2Dh.getArcLength())
		return 1.0f;

	int low = 0;
	int high = this->path2Dh.arcLength_table.size();
	int mid = int(high / 2.0f);

	while (s < this->path2Dh.arcLength_table[mid].s || s >= this->path2Dh.arcLength_table[mid].s)
	{
		if (s < this->path2Dh.arcLength_table[mid].s)
			high = mid;
		else low = mid;
		mid = int((low + high) / 2.0f);
		if (low == mid)
			return CRAB::Linear_Interpolation(this->path2Dh.arcLength_table[low].s, this->path2Dh.arcLength_table[low].t, this->path2Dh.arcLength_table[high].s, this->path2Dh.arcLength_table[high].t, s);
		/*if (fabsf(low - mid) < SMALL_NUMBER)
			return this->arcLength_table[mid].s;*/
	}
	return this->path2Dh.arcLength_table[mid].s;
}

// FIND THE VERTICAL SEGMENT FROM STATION
// --------------------------------------
int Alignment::findSegment(const float& station) const
{
	// special case
	/*if (station >= profile.back()->getStartPoint().x)
		return profile.size() - 1;
	if (station < profile.front()->getStartPoint().x)
		return 0;*/
	if (station <= profile.front()->getStartPoint().x || station > profile.back()->getEndPoint().x)
		return -1;

	// BUSCA BINÁRIA
	/*int low = 0;
	int high = profile.size() - 1;
	int mid = (low + high) / 2;
	while (station < profile[mid]->getStartPoint().x || station >= profile[mid]->getEndPoint().x)
	{
		if (station < profile[mid]->getStartPoint().x)
			high = mid;
		else low = mid;
		mid = (low + high) / 2;
	}
	return mid;*/

	for (int i = 0; i < profile.size(); i++)
		if (station > profile[i]->getStartPoint().x && station <= profile[i]->getEndPoint().x)
			return i;
}

// RETURN
// ------
CRAB::Vector4Df Alignment::getPosition(const float& t) const
{
	glm::vec3 p = this->path3D.getPosition(t);
	return CRAB::Vector4Df{ p.x, p.y, p.z, 1.0f };
}
CRAB::Vector4Df Alignment::getTangent(const float& t) const
{
	glm::vec3 tan = this->path3D.getTangent(t);
	return CRAB::Vector4Df{ tan.x, tan.y, tan.z, 0.0f };
}
CRAB::Vector4Df Alignment::getNormalUp(const float& t/*, const float& V*/) const
{
	glm::vec3 glm_n = this->path3D.getNormalUp(t);
	return CRAB::Vector4Df{ glm_n.x, glm_n.y, glm_n.z, 0.0f };
	/*
	// if R = inf
	glm::vec3 glm_n = this->path3D.getNormalUp(t);
	CRAB::Vector4Df n = { glm_n.x, glm_n.y, glm_n.z, 0.0f };
	float k = this->path2Dh.getCurvature(t);
	if (k < SMALL_NUMBER)
		return n;

	// else
	// float slope = 0.0044f * powf(V, 2.0f) * k; // DNIT
	float slope = powf(V, 2.0f) * k / 127.0f; // AASHTO
	if (slope > SLOPE_MAX)
		slope = SLOPE_MAX;
	float alfa = atanf(slope) * 180.0f / M_PI;

	//std::cout << "\t t = " << t  << "; alfa = " << alfa << "; k = " << k << std::endl;
	
	if (this->path2Dh.isClockwise(t))
		alfa = alfa * (-1.0f);
	CRAB::Matrix4 R = CRAB::rotateArbitrary(alfa, this->getTangent(t));
	return (R * n).to_unitary();
	*/
}
bool Alignment::isClockwise(const float& t) const
{
	return this->path2Dh.isClockwise(t);
}
float Alignment::getCurvature(const float& t) const
{
	return this->path2Dh.getCurvature(t);
}
float Alignment::getRadius(const float& t) const
{
	float k = this->getCurvature(t);
	if (k < SMALL_NUMBER)
		return INFINITY;
	return 1 / k;
}
float Alignment::getDistance(const float& t) const
{
	return this->path3D.getDistance(t);
}
float Alignment::getHorDistance(const float& t) const
{
	return this->path3D.getHorDistance(t);
}

// RETURNS THE LENGTH OF VERTICAL ALIGNMENT
// ----------------------------------------
float Alignment::getProfileLength() const
{
	float Lv = this->profile.back()->getEndPoint().x - this->profile.front()->getStartPoint().x;
	return Lv;
}
// RETURNS THE LENGTH OF HORIZONTAL ALIGNMENT
// ------------------------------------------
float Alignment::getRoadLength() const
{
	float Lh = 0.0f;
	for (int i = 0; i < this->plan.size(); i++)
		Lh += this->plan[i]->segment->getLength();
	return Lh;
}
// RETURNS THE 4D POINT FROM STATION
// ---------------------------------
CRAB::Vector4Df Alignment::getPositionFromStation(const float& station) const
{
	float t = this->findParameter(station);
	glm::vec3 p = this->path2Dh.getPosition(t);
	int index = this->findSegment(station);
	p.y = this->profile[index]->getY(station);
	
	return CRAB::Vector4Df{ p.x, p.y, p.z, 1.0f };
}
// RETURNS THE 4D TANGENT VECTOR FROM STATION
// ------------------------------------------
CRAB::Vector4Df Alignment::getTangentFromStation(const float& station) const
{
	float t = this->findParameter(station);
	glm::vec3 tan = this->path2Dh.getTangent(t);

	return CRAB::Vector4Df{ tan.x, tan.y, tan.z, 0.0f };
}