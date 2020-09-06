#pragma once
#include "HalfEdge.h"
#include "Linear_Algebra.h"
#include "BoxGirder.h"

namespace EulerOp
{
	//MAKE Vertex Face Solid
	inline void mvfs(std::vector<HED::solid*>& model, const CRAB::Vector4Df& point)
	{
		//Initialize
		HED::solid* newSolid = new HED::solid;
		HED::vertex* newVertex = new HED::vertex;
		HED::face* newFace = new HED::face;
		HED::halfEdge* newHE = new HED::halfEdge;

		//Solid
		newSolid->id = model.size();

		//Vertex
		newVertex->id = 0;
		newVertex->point = point;
		newVertex->enable = 0;
		newSolid->vertices.push_back(newVertex);

		//Face
		newFace->id = 0;
		newFace->hEdge = newHE;
		newFace->HedSolid = newSolid;
		newSolid->faces.push_back(newFace);

		//Half-Edge
		newHE->id = 0;
		newHE->leftFace = newFace;
		newHE->next = newHE;
		newHE->opp = newHE;
		newHE->prev = newHE;
		newHE->vStart = newVertex;

		//RETURN
		model.push_back(newSolid);
	}

	//MAKE Edge Vertex
	inline void mev(HED::halfEdge* he1, HED::halfEdge* he2, const int& v, const CRAB::Vector4Df& point)
	{
		//Get Solid
		HED::solid* currentSolid = he1->leftFace->HedSolid;

		//Initialize
		HED::vertex* newVertex = new HED::vertex;
		HED::halfEdge* newHE1 = new HED::halfEdge;
		HED::halfEdge* newHE2 = new HED::halfEdge;

		//Vertex
		newVertex->id = currentSolid->vertices.size();
		newVertex->point = point;
		newVertex->enable = 0;
		currentSolid->vertices.push_back(newVertex);

		//Half-Edge
		newHE1->id = currentSolid->halfEdges.size();
		newHE2->id = currentSolid->halfEdges.size() + 1;
		newHE1->leftFace = he1->leftFace;
		newHE1->vStart = currentSolid->vertices[v];
		newHE2->vStart = currentSolid->vertices.back();
		newHE1->opp = newHE2;
		newHE2->opp = newHE1;

		if (he2 == NULL)
		{
			newHE2->leftFace = he1->leftFace;
			newHE1->next = newHE2;
			newHE2->prev = newHE1;

			if (currentSolid->halfEdges.empty())
			{
				newHE1->prev = newHE2;
				newHE2->next = newHE1;
				he1 = newHE1;
			}
			else
			{
				newHE1->prev = he1;
				newHE2->next = he1->next;
				he1->next->prev = newHE2;
				he1->next = newHE1;
			}
		}
		else
		{
			newHE2->leftFace = he2->leftFace;

			//std::cout << "entrou" << std::endl;

			for (HED::halfEdge* he = he1; he != he2; he = he->opp->next)
				he->vStart = currentSolid->vertices.back();

			//std::cout << "meio" << std::endl;

			//adjacencies
			/*newHE1->next = he1;
			newHE1->prev = he1->prev;
			he1->prev->next = newHE1;
			he1->prev = newHE1;
			he1->opp->next = newHE2;
			newHE2->next = he2;
			newHE2->prev = he2->prev;
			he2->prev->next = newHE2;
			he2->prev = newHE2;*/
			newHE1->next = he1;
			newHE1->prev = he1->prev;
			he1->prev->next = newHE1;
			he1->prev = newHE1;
			newHE2->next = he2;
			newHE2->prev = he2->prev;
			he2->prev->next = newHE2;
			he2->prev = newHE2;

			//std::cout << "saiu" << std::endl;
		}

		//RETURN
		currentSolid->halfEdges.push_back(newHE1);
		currentSolid->halfEdges.push_back(newHE2);
	}

	//MAKE Edge Face
	inline void mef(HED::halfEdge* he1, HED::halfEdge* he2, const int& f)
	{
		//Get Solid
		HED::solid* currentSolid = he1->leftFace->HedSolid;

		//Initialize
		HED::face* newFace = new HED::face;
		HED::halfEdge* newHE1 = new HED::halfEdge;
		HED::halfEdge* newHE2 = new HED::halfEdge;

		/* FUNCIONANDO PRA APENAS UMA ORIENTAÇÃO */

		//Face
		//ok
		newFace->id = currentSolid->faces.size();
		newFace->HedSolid = currentSolid;

		{
			//nok
			newFace->hEdge = he1;
			//nok
			for (HED::halfEdge* he = he1; he != he2; he = he->next)
				he->leftFace = newFace; //currentSolid->faces.back();
		}

		//ok
		currentSolid->faces.push_back(newFace);

		//Half-Edge
		//ok
		newHE1->id = currentSolid->halfEdges.size();
		newHE2->id = currentSolid->halfEdges.size() + 1;
		newHE1->leftFace = currentSolid->faces.back();
		newHE2->leftFace = currentSolid->faces[f];
		newHE1->opp = newHE2;
		newHE2->opp = newHE1;

		{
			//nok
			newHE1->vStart = he2->vStart;
			newHE2->vStart = he1->vStart;
			newHE1->next = he1;
			newHE2->next = he2;
			newHE1->prev = he2->prev;
			newHE2->prev = he1->prev;
			//novas adjacências das half-edges existentes
			//nok
			he1->prev->next = newHE2;
			he2->prev->next = newHE1;
			he1->prev = newHE1;
			he2->prev = newHE2;
			//nova half-edge de referência da face existente
			//nok
			currentSolid->faces[f]->hEdge = he2;
		}

		/* CONDIÇÃO DE ORIENTAÇÃO NÃO FUNCIONANDO */
		/*
		//Face
		//ok
		newFace->id = currentSolid->faces.size();
		newFace->HedSolid = currentSolid;
		//check the loop orientation
		if (he2->vStart == currentSolid->faces[f]->hEdge->vStart)
		{
			//nok
			newFace->hEdge = he1;
			//nok
			for (HED::halfEdge* he = he1; he != he2; he = he->next)
				he->leftFace = newFace; //currentSolid->faces.back();
		}
		else
		{
			newFace->hEdge = he2;
			for (HED::halfEdge* he = he2; he != he1; he = he->next)
				he->leftFace = newFace;
		}
		//ok
		currentSolid->faces.push_back(newFace);

		//Half-Edge
		//ok
		newHE1->id = currentSolid->halfEdges.size();
		newHE2->id = currentSolid->halfEdges.size() + 1;
		newHE1->leftFace = currentSolid->faces.back();
		newHE2->leftFace = currentSolid->faces[f];
		newHE1->opp = newHE2;
		newHE2->opp = newHE1;
		//check the loop orientation
		if (he2->vStart == currentSolid->faces[f]->hEdge->vStart)
		{
			//nok
			newHE1->vStart = he2->vStart;
			newHE2->vStart = he1->vStart;
			newHE1->next = he1;
			newHE2->next = he2;
			newHE1->prev = he2->prev;
			newHE2->prev = he1->prev;
			//novas adjacências das half-edges existentes
			//nok
			he1->prev->next = newHE2;
			he2->prev->next = newHE1;
			he1->prev = newHE1;
			he2->prev = newHE2;
			//nova half-edge de referência da face existente
			//nok
			currentSolid->faces[f]->hEdge = he2;
		}
		else
		{
			newHE1->vStart = he1->vStart;
			newHE2->vStart = he2->vStart;
			newHE1->next = he2;
			newHE2->next = he1;
			newHE1->prev = he1->prev;
			newHE2->prev = he2->prev;
			//novas adjacências das half-edges existentes
			he1->prev->next = newHE1;
			he2->prev->next = newHE2;
			he1->prev = newHE2;
			he2->prev = newHE1;
			//nova half-edge de referência da face existente
			//nok
			currentSolid->faces[f]->hEdge = he1;
		}
		*/

		//RETURN
		currentSolid->halfEdges.push_back(newHE1);
		currentSolid->halfEdges.push_back(newHE2);
	}

	// MODIFY
	// ------

	// Translational Sweep
	// ID da superfície de base = 1 (face q permanece onde foi criada)
	// ID da superfície q acompanha a construção do volume = 0 (cópia da face base com deslocamento no sentido do vetor 'dir', sem alterar o sentido da normal)
	inline void EXTRUDE(HED::face* f, const CRAB::Vector4Df& dir, const float& depth)
	{
		// Get Solid
		HED::solid* currentSolid = f->HedSolid;
		HED::halfEdge* he = f->hEdge;

		if (currentSolid->faces.size() == 2)
		{
			// first edge
			CRAB::Vector4Df newVertex = he->vStart->point + dir * depth;
			mev(he->opp, NULL, he->vStart->id, newVertex);

			// edges and faces
			for (he = f->hEdge->prev; he != f->hEdge; he = he->prev)
			{
				newVertex = he->vStart->point + dir * depth;
				mev(currentSolid->halfEdges.back()->prev, NULL, currentSolid->vertices.back()->id, newVertex);
				mef(currentSolid->halfEdges.back(), he->prev->opp, he->prev->opp->leftFace->id);
			}

			// last face
			mef(currentSolid->halfEdges.back(), currentSolid->halfEdges.back()->next->next->next, currentSolid->halfEdges.back()->next->next->next->leftFace->id);
		}
		else if (currentSolid->faces.size() > 2)
		{
			// first edge
			CRAB::Vector4Df newVertex = he->vStart->point + dir * depth;
			mev(he->prev->opp, he->opp->next, he->vStart->id, newVertex);
			// the other edges
			for (he = f->hEdge->next; he != f->hEdge; he = he->next)
			{
				newVertex = he->vStart->point + dir * depth;
				mev(he->prev->opp, he->opp->next, he->vStart->id, newVertex);
			}
			// close the first new_loop (new face)
			mef(he->opp->prev, he->opp->next->next, he->opp->leftFace->id);
			// the other loops
			for (he = f->hEdge->next; he != f->hEdge; he = he->next)
				mef(he->opp->prev, he->opp->next->next, he->opp->leftFace->id);
		}
	}

	// Translational Sweep
	// ID da superfície de base = 1 (face q permanece onde foi criada)
	// ID da superfície q acompanha a construção do volume = 0 (cópia da face base com deslocamento no sentido do vetor 'dir')
	// Vetor normal da nova face na mesma direção do deslocamento 'dir'
	// Distância dos novos vértices é calculada em função do ponto de referência (vStart da halfEdge de referência da face base)
	inline void NORMAL_EXTRUDE(HED::face* f, const CRAB::Vector4Df& dir, const float& depth)
	{
		// Get Solid
		HED::solid* currentSolid = f->HedSolid;
		HED::halfEdge* he = f->hEdge;
		CRAB::Vector4Df refPoint = he->vStart->point;

		if (currentSolid->faces.size() == 2)
		{
			// new vertex
			CRAB::Vector4Df newVertex = refPoint + dir * depth;
			// first edge
			mev(he->opp, NULL, he->vStart->id, newVertex);
			// edges and faces
			for (he = f->hEdge->prev; he != f->hEdge; he = he->prev)
			{
				// new vertex
				CRAB::Vector4Df refP_p = he->vStart->point - refPoint;
				CRAB::Vector4Df normal = cross(refP_p, dir).to_unitary();
				CRAB::Vector4Df new_dir = cross(dir, normal).to_unitary();
				newVertex = currentSolid->vertices.back()->point + new_dir * (refP_p.length());
				// update parameters
				refPoint = he->vStart->point;
				// new edge
				mev(currentSolid->halfEdges.back()->prev, NULL, currentSolid->vertices.back()->id, newVertex);
				// new face
				mef(currentSolid->halfEdges.back(), he->prev->opp, he->prev->opp->leftFace->id);
			}
			// last face
			mef(currentSolid->halfEdges.back(), currentSolid->halfEdges.back()->next->next->next, 0);
		}
		else if (currentSolid->faces.size() > 2)
		{
			// first edge
			CRAB::Vector4Df newVertex = refPoint + dir * depth;
			mev(he->prev->opp, he->opp->next, he->vStart->id, newVertex);
			// the other edges
			for (he = f->hEdge->next; he != f->hEdge; he = he->next)
			{
				// new vertex
				CRAB::Vector4Df refP_p = he->vStart->point - refPoint;
				CRAB::Vector4Df normal = cross(refP_p, dir).to_unitary();
				CRAB::Vector4Df new_dir = cross(dir, normal).to_unitary();
				newVertex = currentSolid->vertices.back()->point + new_dir * (refP_p.length());

				// update parameters
				refPoint = he->vStart->point;

				// new edge
				//mev(he->opp, NULL, he->vStart->id, newVertex);
				mev(he->prev->opp, he->opp->next, he->vStart->id, newVertex);
			}
			// close the first new_loop (new face)
			mef(he->opp->prev, he->opp->next->next, he->opp->leftFace->id);
			// the other loops
			for (he = f->hEdge->next; he != f->hEdge; he = he->next)
				mef(he->opp->prev, he->opp->next->next, he->opp->leftFace->id);
		}
	}

	// SWEEP: the profile is aligned to be normal to the tangent direction of the sweep path
	// f = face q sofre a extrusão
	// tan_dir = direção tangente no novo ponto do alinhamento (perpendicular à nova face)
	// depth = distância entre o ponto de referência e o novo ponto
	inline void SWEEP(HED::face* f, const CRAB::Vector4Df& tan_dir, const float& depth)
	{
		// Get Solid
		HED::solid* currentSolid = f->HedSolid;
		HED::halfEdge* he = f->hEdge;
		CRAB::Vector4Df refPoint = he->vStart->point;

		if (currentSolid->faces.size() == 2)
		{
			// new vertex
			//CRAB::Vector4Df n = cross(he->next->vStart->point - he->vStart->point, he->next->next->vStart->point - he->next->vStart->point).to_unitary();
			CRAB::Vector4Df n = f->normal();
			CRAB::Vector4Df d = (n * (-1.0f) + tan_dir).to_unitary();

			/*std::cout << "normal = [" << n.x << "; " << n.y << "; " << n.z << "]" << std::endl;
			std::cout << "tangent = [" << tan_dir.x << "; " << tan_dir.y << "; " << tan_dir.z << "]" << std::endl;
			std::cout << "direction = [" << d.x << "; " << d.y << "; " << d.z << "]" << std::endl;
			system("pause");*/

			CRAB::Vector4Df newVertex = refPoint + d * depth;
			// first edge
			mev(he->opp, NULL, he->vStart->id, newVertex);
			// edges and faces
			for (he = f->hEdge->prev; he != f->hEdge; he = he->prev)
			{
				// new vertex
				CRAB::Vector4Df refP_p = he->vStart->point - refPoint;
				CRAB::Vector4Df normal = cross(refP_p, d).to_unitary();
				CRAB::Vector4Df new_dir = cross(tan_dir, normal).to_unitary();
				newVertex = currentSolid->vertices.back()->point + new_dir * (refP_p.length());
				// update parameters
				refPoint = he->vStart->point;
				// new edge
				mev(currentSolid->halfEdges.back()->prev, NULL, currentSolid->vertices.back()->id, newVertex);
				// new face
				mef(currentSolid->halfEdges.back(), he->prev->opp, he->prev->opp->leftFace->id);
			}
			// last face
			mef(currentSolid->halfEdges.back(), currentSolid->halfEdges.back()->next->next->next, 0);
		}
		else if (currentSolid->faces.size() > 2)
		{
			// first edge
			//CRAB::Vector4Df n = cross(he->next->vStart->point - he->vStart->point, he->next->next->vStart->point - he->next->vStart->point).to_unitary();
			CRAB::Vector4Df n = f->normal();
			CRAB::Vector4Df d = (n + tan_dir).to_unitary();
			CRAB::Vector4Df newVertex = refPoint + d * depth;
			mev(he->prev->opp, he->opp->next, he->vStart->id, newVertex);
			// the other edges
			for (he = f->hEdge->next; he != f->hEdge; he = he->next)
			{
				// new vertex
				CRAB::Vector4Df refP_p = he->vStart->point - refPoint;
				CRAB::Vector4Df normal = cross(refP_p, d).to_unitary();
				CRAB::Vector4Df new_dir = cross(tan_dir, normal).to_unitary();
				newVertex = currentSolid->vertices.back()->point + new_dir * (refP_p.length());

				// update parameters
				refPoint = he->vStart->point;

				// new edge
				//mev(he->opp, NULL, he->vStart->id, newVertex);
				mev(he->prev->opp, he->opp->next, he->vStart->id, newVertex);
			}
			// close the first new_loop (new face)
			mef(he->opp->prev, he->opp->next->next, he->opp->leftFace->id);
			// the other loops
			for (he = f->hEdge->next; he != f->hEdge; he = he->next)
				mef(he->opp->prev, he->opp->next->next, he->opp->leftFace->id);
		}
	}

	// SWEEP
	// f = face q sofre a varredura
	// path = curva parametrizada
	//inline void SWEEP(HED::face* f, Alignment* path, Road* roadway)
	inline void SWEEP(HED::face* f, BoxGirder* bridge)
	{
		// Get Solid
		HED::solid* currentSolid = f->HedSolid;

		for (int i = 0; i < ELEMENTS; i++)
		{
			/* ---------- TRANSFORMATION MATRIX ---------- */

			// CURRENT POSITION
			float t = float(i) / ELEMENTS;
			// Local View
			CRAB::Matrix4 ViewMatrix = toLocal(bridge->alignment->getPosition(t), bridge->alignment->getTangent(t), bridge->getNormal(t));

			// NEXT POSITION
			t = float(i + 1) / ELEMENTS;
			// Next View
			CRAB::Matrix4 ModelMatrix = toWorld(bridge->alignment->getPosition(t), bridge->alignment->getTangent(t), bridge->getNormal(t));

			/* ---------------- EXTRUDE ---------------- */

			if (currentSolid->faces.size() == 2)
			{	// use the back face
				HED::face* backFace = currentSolid->faces.back();
				HED::halfEdge* he = backFace->hEdge;

				// new vertex
				CRAB::Vector4Df newVertex = ModelMatrix * (ViewMatrix * he->vStart->point);
				// first edge
				mev(he->opp, NULL, he->vStart->id, newVertex);
				// edges and faces
				for (he = backFace->hEdge->prev; he != backFace->hEdge; he = he->prev)
				{
					// new vertex
					newVertex = ModelMatrix * (ViewMatrix * he->vStart->point);
					// new edge
					mev(currentSolid->halfEdges.back()->prev, NULL, currentSolid->vertices.back()->id, newVertex);
					// new face
					mef(currentSolid->halfEdges.back(), he->prev->opp, he->prev->opp->leftFace->id);
				}
				// last face
				mef(currentSolid->halfEdges.back(), currentSolid->halfEdges.back()->next->next->next, 0);
			}
			else if (currentSolid->faces.size() > 2)
			{	// use the current face
				// Get HalfEdge
				HED::halfEdge* he = f->hEdge;

				// new vertex
				CRAB::Vector4Df newVertex = ModelMatrix * (ViewMatrix * he->vStart->point);
				// first edge
				mev(he->prev->opp, he->opp->next, he->vStart->id, newVertex);
				// the other edges
				for (he = f->hEdge->next; he != f->hEdge; he = he->next)
				{
					// new vertex
					newVertex = ModelMatrix * (ViewMatrix * he->vStart->point);
					// new edge
					mev(he->prev->opp, he->opp->next, he->vStart->id, newVertex);
				}
				// close the first new_loop (new face)
				mef(he->opp->prev, he->opp->next->next, he->opp->leftFace->id);
				// the other loops
				for (he = f->hEdge->next; he != f->hEdge; he = he->next)
					mef(he->opp->prev, he->opp->next->next, he->opp->leftFace->id);
			}
		}
	}
}