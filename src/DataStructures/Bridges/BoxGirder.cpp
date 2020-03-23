#include "BoxGirder.h"
#include "EulerOp.h"

// DEFAULT CONSTRUCTOR
BoxGirder::BoxGirder()
{
}

// OVERLOAD CONSTRUCTOR
BoxGirder::BoxGirder(const Alignment &roadway, const CRAB::Station& _start, const CRAB::Station& _end)
{
	span = 35.0f;

	// Bridge Attributes
	B = roadway.getRoad()->B;
	H = int((100.0f * span / 16.0f) / 5.0f) * 0.05f;
	Lb = int((100.0f * B / 4.3f) / 5.0f) * 0.05f;
	h = int((100.0f * Lb / 10.0f) / 5.0f) * 0.05f;
	bw = int((100.0f * H / 5.0f) / 5.0f) * 0.05f;
	if (bw < 0.25f) bw = 0.25f;
	tv = int((100.0f * (H - 2 * h) / 10.0f) / 5.0f) * 0.05f;
	if (tv < 0.10f) tv = 0.10f;
	b = int((100.0f * (B - 2.0f * (Lb + INCLINATION_RATIO * (H - h - tv)))) / 5.0f) * 0.05f;
	th = int((100.0f * (b - 2 * bw) / 5.0f) / 5.0f) * 0.05f;

	//// Alignment
	//// TODO: copiar alinhamento se a ponte estiver inserida em v�rios segmentos
	//for (int i = 0; i < roadway.segments.size(); i++)
	//{
	//	if (_start.c_Meters() <= roadway.segments[i]->getStartStation().c_Meters() && _end.c_Meters() >= roadway.segments[i]->getEndStation().c_Meters())
	//	{// verifica se o segmento corrente est� dentro das esta��es limites da obra
	//		path.push_back(roadway.segments[i]);
	//	}
	//	else if (_start.c_Meters() > roadway.segments[i]->getStartStation().c_Meters() && _start.c_Meters() < roadway.segments[i]->getEndStation().c_Meters())
	//	{// verifica se a ponte come�a dentro do segmento corrente (com fim em outro segmento)
	//		std::string className = typeid(*roadway.segments[i]).name();
	//		if (className == "class Line")
	//		{
	//			float x1 = _start.c_Meters() - roadway.segments[i]->getStartStation().c_Meters();
	//			float x2 = roadway.segments[i]->getStartStation().getDistance(roadway.segments[i]->getEndStation());
	//			float t = x1 / x2;
	//			CRAB::Vector4Df start_point = roadway.segments[i]->getPosition(t);
	//			path.push_back(new Line(CRAB::Station{ start_point }, roadway.segments[i]->getEndStation()));
	//		}
	//		else if (className == "class CircularArc")
	//		{
	//			//TODO
	//			path.push_back(new CircularArc(CRAB::Station{ _start.E, _start.meters }, CRAB::Station{ roadway.segments[i]->getMidPoint() }, roadway.segments[i]->getEndStation()));
	//		}
	//	}
	//	else if (_end.c_Meters() > roadway.segments[i]->getStartStation().c_Meters() && _end.c_Meters() < roadway.segments[i]->getEndStation().c_Meters())
	//	{// verifica se a ponte termina dentro do segmento corrente (com in�cio em outro segmento)
	//		std::string className = typeid(*roadway.segments[i]).name();
	//		if (className == "class Line")
	//		{
	//			float x1 = _end.c_Meters() - roadway.segments[i]->getStartStation().c_Meters();
	//			float x2 = roadway.segments[i]->getStartStation().getDistance(roadway.segments[i]->getEndStation());
	//			float t = x1 / x2;
	//			CRAB::Vector4Df end_point = roadway.segments[i]->getPosition(t);
	//			path.push_back(new Line(roadway.segments[i]->getStartStation(), CRAB::Station{ end_point }));
	//		}
	//		else if (className == "class CircularArc")
	//		{
	//			//TODO
	//			path.push_back(new CircularArc(roadway.segments[i]->getStartStation(), CRAB::Station{ roadway.segments[i]->getMidPoint() }, CRAB::Station{ _start.E, _start.meters }));
	//		}
	//	}
	//	//TODO: in�cio e fim da ponte dentro do segmento corrente
	//}
	path = roadway.segments;

	// Model
	update();
}

// DESTRUCTOR
BoxGirder::~BoxGirder()
{
}

// ALLOCATION
void BoxGirder::setSpan(const float& _span)
{
	span = _span;
}
void BoxGirder::setWidth(const float& _B)
{
	B = _B;
}
void BoxGirder::setBw(const float& _bw)
{
	bw = _bw;
}
void BoxGirder::setH(const float& _H)
{
	H = _H;
}

// RETURN
std::vector<HED::solid*> BoxGirder::getModel() const
{
	return model;
}
std::vector<Segment*> BoxGirder::getPath() const
{
	return path;
}
float BoxGirder::getWidth() const
{
	return B;
}
float BoxGirder::getSpan() const
{
	return span;
}
float BoxGirder::getHeight() const
{
	return H;
}
float BoxGirder::getLb() const
{
	return Lb;
}
float BoxGirder::getDeckH() const
{
	return h;
}
float BoxGirder::getBw() const
{
	return bw;
}
float BoxGirder::getSlabWidth() const
{
	return b;
}
float BoxGirder::getTH() const
{
	return th;
}
float BoxGirder::getTV() const
{
	return tv;
}

// UPDATE AUTO SECTION DATA
void BoxGirder::updateAutoSection()
{
	H = int((100.0f * span / 16.0f) / 5.0f) * 0.05f;
	Lb = int((100.0f * B / 4.3f) / 5.0f) * 0.05f;
	h = int((100.0f * Lb / 10.0f) / 5.0f) * 0.05f;
	bw = int((100.0f * H / 5.0f) / 5.0f) * 0.05f;
	if (bw < 0.25f) bw = 0.25f;
	tv = int((100.0f * (H - 2 * h) / 10.0f) / 5.0f) * 0.05f;
	if (tv < 0.10f) tv = 0.10f;
	b = int((100.0f * (B - 2.0f * (Lb + INCLINATION_RATIO * (H - h - tv)))) / 5.0f) * 0.05f;
	th = int((100.0f * (b - 2 * bw) / 5.0f) / 5.0f) * 0.05f;
}

// UPDATE THE MODEL
void BoxGirder::update()
{
	//Initialize
	model.clear();
	CRAB::Vector4Df newVertex, next_position, start_point;
	float offset; // displacement
	float segment_L; // the length of the line segment
	float t = 0.0f; // [0; 1]

	// POLYGON FACE (GL_TRIANGLE_FAN)
	// ------------------------------

	// Local axis
	CRAB::Vector4Df vRight = cross(path.front()->getTan(t), { 0.0f, 1.0f, 0.0f, 0.0f }).to_unitary();
	CRAB::Vector4Df vUp = cross(vRight, path.front()->getTan(t)).to_unitary();

#pragma region TOP_LAYER
	// v0
	start_point = path.front()->getStartPoint();// -(vUp * TOP_LAYER);
	EulerOp::mvfs(model, start_point);
	model.back()->name = "TOP_LAYER";
	model.back()->material = { 0.1f, 0.1f, 0.1f, 1.0f };
	// v1
	newVertex = model.back()->vertices.back()->point - (vRight * (B / 2.0f - GUARD_RAIL)) - (vUp * (B / 2.0f - GUARD_RAIL) * SLOPE);
	EulerOp::mev(model.back()->faces[0]->hEdge, NULL, 0, newVertex);
	// v2
	newVertex = model.back()->vertices.back()->point + (vRight * (B - 2.0f * GUARD_RAIL));
	EulerOp::mev(model.back()->halfEdges[0], NULL, 1, newVertex);
	// f1
	EulerOp::mef(model.back()->halfEdges[0], model.back()->halfEdges[3], 0);

	// SWEEP
	for (int i = 0; i < path.size(); i++)
	{
		if (i == 0) // First segment
		{
			//std::cout << "path[i]->getTan(0) = [" << path[i]->getTan(0).x << "; " << path[i]->getTan(0).y << "; " << path[i]->getTan(0).z << "]" << std::endl;

			// First Sweep
			t = 1.0f / DIVIDER;
			next_position = path[i]->getPosition(t);
			segment_L = (next_position - path[i]->getStartPoint()).length();
			EulerOp::SWEEP(model.back()->faces.back(), path[i]->getTan(t), segment_L);
			start_point = next_position;

			for (int j = 1; j < DIVIDER; j++)
			{
				t += 1.0f / DIVIDER;
				next_position = path[i]->getPosition(t);
				segment_L = (next_position - start_point).length();
				EulerOp::SWEEP(model.back()->faces[0], path[i]->getTan(t), segment_L);
				start_point = next_position;
			}
		}
		else // Others segments
		{
			t = 0.0f;
			for (int j = 0; j < DIVIDER; j++)
			{
				t += 1.0f / DIVIDER;
				next_position = path[i]->getPosition(t);
				segment_L = (next_position - start_point).length();
				EulerOp::SWEEP(model.back()->faces[0], path[i]->getTan(t), segment_L);
				start_point = next_position;
			}
		}
	}
#pragma endregion TOP_LAYER

#pragma region DECK
	// OFFSET
	offset = (B / 2.0f - GUARD_RAIL) * SLOPE;
	// v0
	start_point = path.front()->getStartPoint() - (vUp * offset);
	EulerOp::mvfs(model, start_point);
	model.back()->name = "DECK";
	model.back()->material = { 0.8f, 0.8f, 0.8f, 1.0f };
	// v1
	newVertex = model.back()->vertices.back()->point - (vRight * (B / 2.0f - GUARD_RAIL));
	EulerOp::mev(model.back()->faces[0]->hEdge, NULL, 0, newVertex);

	// Left Guard-Rail
	{
		// V2
		newVertex = model.back()->vertices.back()->point + (vUp * 0.15f);
		EulerOp::mev(model.back()->halfEdges[0], NULL, 1, newVertex);
		// V3
		newVertex = model.back()->vertices.back()->point + (vUp * 0.25f) - (vRight * 0.175f);
		EulerOp::mev(model.back()->halfEdges[2], NULL, 2, newVertex);
		// V4
		newVertex = model.back()->vertices.back()->point + (vUp * 0.47f) - (vRight * 0.05f);
		EulerOp::mev(model.back()->halfEdges[4], NULL, 3, newVertex);
		// V5
		newVertex = model.back()->vertices.back()->point - (vRight * 0.175f);
		EulerOp::mev(model.back()->halfEdges[6], NULL, 4, newVertex);
		// V6
		newVertex = model.back()->vertices.back()->point - (vUp * 1.12f);
		EulerOp::mev(model.back()->halfEdges[8], NULL, 5, newVertex);
		// V7
		newVertex = model.back()->vertices.back()->point - (vUp * 0.05f) + (vRight * 0.05f);
		EulerOp::mev(model.back()->halfEdges[10], NULL, 6, newVertex);
		// V8
		newVertex = model.back()->vertices.back()->point + (vUp * 0.10f) + (vRight * 0.35f);
		EulerOp::mev(model.back()->halfEdges[12], NULL, 7, newVertex);
	}
	// v9
	newVertex = model.back()->vertices.back()->point - (vUp * (h + tv - 0.20f)) + (vRight * (Lb - GUARD_RAIL));
	EulerOp::mev(model.back()->halfEdges[14], NULL, 8, newVertex);
	// v10
	float hyp = sqrtf(powf(INCLINATION_RATIO * bw, 2.0f) + powf(bw, 2.0f)); // Hypotenuse = top width of beam
	newVertex = model.back()->vertices.back()->point + (vRight * hyp);
	EulerOp::mev(model.back()->halfEdges[16], NULL, 9, newVertex);
	// v11
	newVertex = model.back()->vertices.back()->point + (vUp * tv) + (vRight * th);
	EulerOp::mev(model.back()->halfEdges[18], NULL, 10, newVertex);

	// MIRROR
	{
		// v12
		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[11]->point - model.back()->vertices[0]->point, vUp);
		EulerOp::mev(model.back()->halfEdges[20], NULL, 11, newVertex);
		// v13
		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[10]->point - model.back()->vertices[0]->point, vUp);
		EulerOp::mev(model.back()->halfEdges[22], NULL, 12, newVertex);
		// v14
		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[9]->point - model.back()->vertices[0]->point, vUp);
		EulerOp::mev(model.back()->halfEdges[24], NULL, 13, newVertex);
		// v15
		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[8]->point - model.back()->vertices[0]->point, vUp);
		EulerOp::mev(model.back()->halfEdges[26], NULL, 14, newVertex);
		// v16
		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[7]->point - model.back()->vertices[0]->point, vUp);
		EulerOp::mev(model.back()->halfEdges[28], NULL, 15, newVertex);
		// v17
		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[6]->point - model.back()->vertices[0]->point, vUp);
		EulerOp::mev(model.back()->halfEdges[30], NULL, 16, newVertex);
		// v18
		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[5]->point - model.back()->vertices[0]->point, vUp);
		EulerOp::mev(model.back()->halfEdges[32], NULL, 17, newVertex);
		// v19
		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[4]->point - model.back()->vertices[0]->point, vUp);
		EulerOp::mev(model.back()->halfEdges[34], NULL, 18, newVertex);
		// v20
		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[3]->point - model.back()->vertices[0]->point, vUp);
		EulerOp::mev(model.back()->halfEdges[36], NULL, 19, newVertex);
		// v21
		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[2]->point - model.back()->vertices[0]->point, vUp);
		EulerOp::mev(model.back()->halfEdges[38], NULL, 20, newVertex);
		// v22
		newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[1]->point - model.back()->vertices[0]->point, vUp);
		EulerOp::mev(model.back()->halfEdges[40], NULL, 21, newVertex);
	}

	// f1
	EulerOp::mef(model.back()->halfEdges[0], model.back()->halfEdges[43], 0);

	// SWEEP
	for (int i = 0; i < path.size(); i++)
	{
		if (i == 0) // First segment
		{
			// First Sweep
			t = 1.0f / DIVIDER;
			{	// UPDATE Local axis
				vRight = cross(path[i]->getTan(t), { 0.0f, 1.0f, 0.0f, 0.0f }).to_unitary();
				vUp = cross(vRight, path[i]->getTan(t)).to_unitary();
			}
			next_position = path[i]->getPosition(t) - (vUp * offset);
			segment_L = (next_position - start_point).length();
			EulerOp::SWEEP(model.back()->faces.back(), path[i]->getTan(t), segment_L);
			start_point = next_position;

			for (int j = 1; j < DIVIDER; j++)
			{
				t += 1.0f / DIVIDER;
				{	// UPDATE Local axis
					vRight = cross(path[i]->getTan(t), { 0.0f, 1.0f, 0.0f, 0.0f }).to_unitary();
					vUp = cross(vRight, path[i]->getTan(t)).to_unitary();
				}
				next_position = path[i]->getPosition(t) - (vUp * offset);
				segment_L = (next_position - start_point).length();
				EulerOp::SWEEP(model.back()->faces[0], path[i]->getTan(t), segment_L);
				start_point = next_position;
			}
		}
		else // Others segments
		{
			t = 0.0f;
			for (int j = 0; j < DIVIDER; j++)
			{
				t += 1.0f / DIVIDER;
				{	// UPDATE Local axis
					vRight = cross(path[i]->getTan(t), { 0.0f, 1.0f, 0.0f, 0.0f }).to_unitary();
					vUp = cross(vRight, path[i]->getTan(t)).to_unitary();
				}
				next_position = path[i]->getPosition(t) - (vUp * offset);
				segment_L = (next_position - start_point).length();
				EulerOp::SWEEP(model.back()->faces[0], path[i]->getTan(t), segment_L);
				start_point = next_position;
			}
		}
	}
#pragma endregion DECK

#pragma region U_SECTION
	// UPDATE Local axis
	t = 0.0f;
	vRight = cross(path.front()->getTan(t), { 0.0f, 1.0f, 0.0f, 0.0f }).to_unitary();
	vUp = cross(vRight, path.front()->getTan(t)).to_unitary();
	// OFFSET
	offset = (B / 2.0f - GUARD_RAIL) * SLOPE + H;
	// v0
	start_point = path.front()->getStartPoint() - (vUp * offset);
	EulerOp::mvfs(model, start_point);
	model.back()->name = "U_SECTION";
	model.back()->material = { 0.8f, 0.8f, 0.8f, 1.0f };
	// v1
	newVertex = model.back()->vertices.back()->point + (vRight * (b / 2.0f));
	EulerOp::mev(model.back()->faces[0]->hEdge, NULL, 0, newVertex);
	// v2
	newVertex = model.back()->vertices.back()->point + (vRight * (B - b - 2 * Lb) / 2.0f) + (vUp * (H - h - tv));
	EulerOp::mev(model.back()->halfEdges[0], NULL, 1, newVertex);
	// v3
	newVertex = model.back()->vertices.back()->point - (vRight * hyp);
	EulerOp::mev(model.back()->halfEdges[2], NULL, 2, newVertex);
	// v4
	newVertex = model.back()->vertices.back()->point - (vUp * (H - 2 * (h + tv))) - (vRight * (INCLINATION_RATIO * ((H - 2 * (h + tv)))));
	EulerOp::mev(model.back()->halfEdges[4], NULL, 3, newVertex);
	// v5
	newVertex = model.back()->vertices.back()->point - (vUp * tv) - (vRight * th);
	EulerOp::mev(model.back()->halfEdges[6], NULL, 4, newVertex);
	// MIRROR
	// v6
	newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[5]->point - model.back()->vertices[0]->point, vUp);
	EulerOp::mev(model.back()->halfEdges[8], NULL, 5, newVertex);
	// v7
	newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[4]->point - model.back()->vertices[0]->point, vUp);
	EulerOp::mev(model.back()->halfEdges[10], NULL, 6, newVertex);
	// v8
	newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[3]->point - model.back()->vertices[0]->point, vUp);
	EulerOp::mev(model.back()->halfEdges[12], NULL, 7, newVertex);
	// v9
	newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[2]->point - model.back()->vertices[0]->point, vUp);
	EulerOp::mev(model.back()->halfEdges[14], NULL, 8, newVertex);
	// v10
	newVertex = model.back()->vertices[0]->point + reflection(model.back()->vertices[1]->point - model.back()->vertices[0]->point, vUp);
	EulerOp::mev(model.back()->halfEdges[16], NULL, 9, newVertex);
	// f1
	EulerOp::mef(model.back()->halfEdges[0], model.back()->halfEdges[19], 0);

	// SWEEP
	for (int i = 0; i < path.size(); i++)
	{
		if (i == 0) // First segment
		{
			// First Sweep
			t = 1.0f / DIVIDER;
			{	// UPDATE Local axis
				vRight = cross(path[i]->getTan(t), { 0.0f, 1.0f, 0.0f, 0.0f }).to_unitary();
				vUp = cross(vRight, path[i]->getTan(t)).to_unitary();
			}
			next_position = path[i]->getPosition(t) - (vUp * offset);
			segment_L = (next_position - start_point).length();
			EulerOp::SWEEP(model.back()->faces.back(), path[i]->getTan(t), segment_L);
			start_point = next_position;

			for (int j = 1; j < DIVIDER; j++)
			{
				t += 1.0f / DIVIDER;
				{	// UPDATE Local axis
					vRight = cross(path[i]->getTan(t), { 0.0f, 1.0f, 0.0f, 0.0f }).to_unitary();
					vUp = cross(vRight, path[i]->getTan(t)).to_unitary();
				}
				next_position = path[i]->getPosition(t) - (vUp * offset);
				segment_L = (next_position - start_point).length();
				EulerOp::SWEEP(model.back()->faces[0], path[i]->getTan(t), segment_L);
				start_point = next_position;
			}
		}
		else // Others segments
		{
			t = 0.0f;
			for (int j = 0; j < DIVIDER; j++)
			{
				t += 1.0f / DIVIDER;
				{	// UPDATE Local axis
					vRight = cross(path[i]->getTan(t), { 0.0f, 1.0f, 0.0f, 0.0f }).to_unitary();
					vUp = cross(vRight, path[i]->getTan(t)).to_unitary();
				}
				next_position = path[i]->getPosition(t) - (vUp * offset);
				segment_L = (next_position - start_point).length();
				EulerOp::SWEEP(model.back()->faces[0], path[i]->getTan(t), segment_L);
				start_point = next_position;
			}
		}
	}

#pragma endregion U_SECTION
}
