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
	// 2D Horizontal Curve CONSTRUCTOR
	// -------------------------------
	std::vector<Geometry*> hor2DSegments;
	for (int i = 0; i < this->plan.size(); i++)
	{
		std::vector<Geometry*> horizontal_curve;
		switch (this->plan[i]->transition)
		{
		case BOTH:
			horizontal_curve = this->plan[i]->HorizontalCurve();
			hor2DSegments.insert(hor2DSegments.end(), horizontal_curve.begin(), horizontal_curve.end());
			break;
		case START:
			horizontal_curve = this->plan[i]->HorizontalCurveStartTransition();
			hor2DSegments.insert(hor2DSegments.end(), horizontal_curve.begin(), horizontal_curve.end());
			break;
		case END:
			horizontal_curve = this->plan[i]->HorizontalCurveEndTransition();
			hor2DSegments.insert(hor2DSegments.end(), horizontal_curve.begin(), horizontal_curve.end());
			break;
		default:
			hor2DSegments.push_back(this->plan[i]->segment);
			break;
		}
	}
	this->path2Dh = NURBS(hor2DSegments);

	// 3D Curve CONSTRUCTOR
	// --------------------
	std::vector<glm::vec3> points3D;
	for (int i = 0; i <= ELEMENTS; i++)
	{
		float t = float(i) / ELEMENTS;
		// coordenadas em planta (UTM)
		points3D.push_back(this->path2Dh.getPosition(t));
		// elevation (m)
		float distance = this->path2Dh.getDistance(t);
		int index = findSegment(distance);
		points3D.back().y = profile[index]->getY(distance);
	}
	this->path3D = NURBS(points3D);
}

// DESTRUCTOR
// ----------
Alignment::~Alignment()
{
}

int Alignment::findSegment(const float& station) const
{
	// special case
	if (station >= profile.back()->getStartPoint().x)
		return profile.size() - 1;

	int low = 0;
	int high = profile.size() - 1;
	int mid = (low + high) / 2;

	while (station < profile[mid]->getStartPoint().x || station >= profile[mid]->getEndPoint().x)
	{
		if (station < profile[mid]->getStartPoint().x)
			high = mid;
		else low = mid;
		mid = (low + high) / 2;
	}
	return mid;
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
CRAB::Vector4Df Alignment::getNormal(const float& t) const
{
	glm::vec3 glm_n = this->path2Dh.getNormal(t);
	CRAB::Vector4Df n = { glm_n.x, glm_n.y, glm_n.z, 0.0f };

	float hor_radius = this->path2Dh.getRadius(t);

	float tan_alfa = 0.0044f * powf(60.0f, 2.0f) / hor_radius;
	if (tan_alfa > SLOPE_MAX)
		tan_alfa = SLOPE_MAX;

	float alfa = atanf(tan_alfa) * 180.0f / M_PI;

	if (this->path2Dh.isClockwise(t))
		alfa = alfa * (-1.0f);
	CRAB::Matrix4 R = CRAB::rotateArbitrary(alfa, this->getTangent(t));
	return (R * n).to_unitary() * hor_radius;
}
CRAB::Vector4Df Alignment::getNormalUp(const float& t) const
{
	// if R = inf
	glm::vec3 glm_n = this->path3D.getNormalUp(t);
	CRAB::Vector4Df n = { glm_n.x, glm_n.y, glm_n.z, 0.0f };

	float hor_radius = this->path2Dh.getRadius(t);
	if (hor_radius == 0.0f)
		return n;

	// else
	float Vp = 40.0f; // velocidade de projeto
	float tan_alfa = 0.0044f * powf(Vp, 2.0f) / hor_radius;
	if (tan_alfa > SLOPE_MAX)
		tan_alfa = SLOPE_MAX;
	float alfa = atanf(tan_alfa) * 180.0f / M_PI;
	
	if (this->path2Dh.isClockwise(t))
		alfa = alfa * (-1.0f);
	CRAB::Matrix4 R = CRAB::rotateArbitrary(alfa, this->getTangent(t));
	return (R * n).to_unitary();
}