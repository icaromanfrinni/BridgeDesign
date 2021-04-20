#include "Alignment.h"


// DEFAULT CONSTRUCTOR
// -------------------
Alignment::Alignment()
{
}

// DEFAULT CONSTRUCTOR
// -------------------
Alignment::Alignment(const std::string& _name, const std::vector<HorSegment*>& _roadplan, const std::vector<VerSegment*>& _profile)
	: name(_name), roadplan(_roadplan), profile(_profile)
{
	std::cout << std::endl;
	std::cout << "\tNEW Alignment ............................ " << name << std::endl;

	// 2D Horizontal Curve CONSTRUCTOR
	// -------------------------------
	std::cout << "\tBuilding the horizontal NURBS curve" << std::endl;
	this->path2D = NURBS(roadplan);

	// VPI list CONSTRUCTOR
	// --------------------
	this->VPI_list.push_back(new CRAB::Vector4Df(this->profile.front()->getStartPoint4D())); // first segment
	for (int i = 0; i < this->profile.size(); i++)
	{
		//if (this->profile[i]->getG1() != this->profile[i]->getG2())
		if (fabs(this->profile[i]->getG1() - this->profile[i]->getG2()) > SMALL_NUMBER)
			this->VPI_list.push_back(new CRAB::Vector4Df(this->profile[i]->getMidPoint4D()));
	}
	this->VPI_list.push_back(new CRAB::Vector4Df(this->profile.back()->getEndPoint4D())); // last segment
}

// DESTRUCTOR
// ----------
Alignment::~Alignment()
{
}

float Alignment::findParameter(const float& s) const
{
	if (s <= 0.0f)
		return 0.0f;
	if (s >= this->path2D.getArcLength())
		return 1.0f;

	int low = 0;
	int high = this->path2D.arcLength_table.size();
	int mid = int(high / 2.0f);

	while (s < this->path2D.arcLength_table[mid].s || s >= this->path2D.arcLength_table[mid].s)
	{
		if (s < this->path2D.arcLength_table[mid].s)
			high = mid;
		else low = mid;
		mid = int((low + high) / 2.0f);
		if (low == mid)
			return CRAB::Linear_Interpolation(this->path2D.arcLength_table[low].s, this->path2D.arcLength_table[low].t, this->path2D.arcLength_table[high].s, this->path2D.arcLength_table[high].t, s);
	}
	return this->path2D.arcLength_table[mid].s;
}

// FIND THE VERTICAL SEGMENT FROM STATION
// --------------------------------------
int Alignment::findSegment(const float& s) const
{
	//std::cout << "profile.size() = " << profile.size() << std::endl;
	for (int i = 0; i < profile.size(); i++)
	{
		/*std::cout << "i = " << i << std::endl;
		std::cout << "Start = " << profile[i]->getStartPoint4D().x << std::endl;
		std::cout << "End = " << profile[i]->getEndPoint4D().x << std::endl;*/
		float A = profile[i]->getStartPoint4D().x;
		float B = profile[i]->getEndPoint4D().x;
		/*if (s >= A && s <= B)
			return i;*/
		if (s > A || fabsf(s - A) < 0.01f) // < 1cm
			if (s < B || fabsf(s - B) < 0.01f)
				return i;
	}
	return -1;
}

// RETURN
// ------
glm::vec3 Alignment::getPosition3D(const float& t) const
{
	glm::vec3 p = this->path2D.getPosition(t);
	float s = this->path2D.getDistance(t);
	/*std::cout << "\n" << std::endl;
	std::cout << "s = " << s << std::endl;*/
	int index = this->findSegment(s);
	/*if (index == -1)
		return p;*/
	//std::cout << "index = " << index << std::endl;
	p.y = this->profile[index]->getY(s);
	//std::cout << "y = " << p.y << std::endl;
	return p;
}
CRAB::Vector4Df Alignment::getPosition(const float& t) const
{
	glm::vec3 p = this->getPosition3D(t);
	return CRAB::Vector4Df{ p.x, p.y, p.z, 1.0f };
}
CRAB::Vector4Df Alignment::getTangent(const float& t) const
{
	// TODO: considerar o perfil longitudinal
	glm::vec3 tan = this->path2D.getTangent(t);
	return CRAB::Vector4Df{ tan.x, tan.y, tan.z, 0.0f };
}
CRAB::Vector4Df Alignment::getNormalUp(const float& t) const
{
	glm::vec3 glm_n = this->path2D.getNormalUp(t);
	return CRAB::Vector4Df{ glm_n.x, glm_n.y, glm_n.z, 0.0f };
}
bool Alignment::isClockwise(const float& t) const {
	return this->path2D.isClockwise(t);
}
float Alignment::getCurvature(const float& t) const {
	return this->path2D.getCurvature(t);
}
float Alignment::getRadius(const float& t) const
{
	float k = this->getCurvature(t);
	if (k < SMALL_NUMBER)
		return INFINITY;
	return 1 / k;
}
float Alignment::getDistance(const float& t) const {
	return this->path2D.getDistance(t);
}

// RETURNS THE LENGTH OF VERTICAL ALIGNMENT
// ----------------------------------------
//float Alignment::getProfileLength() const
//{
//	float L = this->profile.back()->getEndPoint4D().x - this->profile.front()->getStartPoint4D().x;
//	return L;
//}
// RETURNS THE SPACE POSITION FROM STATION
// ---------------------------------------
glm::vec3 Alignment::getPositionFromStation3D(const float& s) const
{
	float t = this->path2D.getParameter(s);
	return this->getPosition3D(t);
}
CRAB::Vector4Df Alignment::getPositionFromStation(const float& s) const
{
	float t = this->path2D.getParameter(s);
	return this->getPosition(t);
}
// RETURNS THE 4D TANGENT VECTOR FROM STATION
// ------------------------------------------
CRAB::Vector4Df Alignment::getTangentFromStation(const float& s) const
{
	float t = this->path2D.getParameter(s);
	return this->getTangent(t);
}