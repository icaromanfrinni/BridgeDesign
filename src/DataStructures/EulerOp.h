#pragma once
#include "HalfEdge.h"
#include "Linear_Algebra.h"

namespace CRAB
{
	//MAKE Vertex Face Solid
	void mvfs(std::vector<CRAB::solid*>& model, const CRAB::Vector4Df& point)
	{
		//Initialize
		CRAB::solid* newSolid = new CRAB::solid;
		CRAB::vertex* newVertex = new CRAB::vertex;
		CRAB::face* newFace = new CRAB::face;
		CRAB::halfEdge* newHE = new CRAB::halfEdge;

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
	void mev(CRAB::halfEdge* he1, CRAB::halfEdge* he2, const int& v, const CRAB::Vector4Df& point)
	{
		//Get Solid
		CRAB::solid* currentSolid = he1->leftFace->HedSolid;

		//Initialize
		CRAB::vertex* newVertex = new CRAB::vertex;
		CRAB::halfEdge* newHE1 = new CRAB::halfEdge;
		CRAB::halfEdge* newHE2 = new CRAB::halfEdge;

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

		if (he2 == NULL)
		{
			newHE2->leftFace = he1->leftFace;
			newHE1->next = newHE2;
			newHE1->opp = newHE2;
			newHE2->prev = newHE1;
			newHE2->opp = newHE1;

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

			std::cout << "entrou" << std::endl;

			for (CRAB::halfEdge* he = he1; he != he2; he = he->opp->next)
				he->vStart = currentSolid->vertices.back();

			std::cout << "meio" << std::endl;

			//adjacencies
			newHE1->next = he1;
			newHE1->prev = he1->prev;
			he1->prev->next = newHE1;
			he1->prev = newHE1;
			he1->opp->next = newHE2;
			newHE2->next = he2;
			newHE2->prev = he2->prev;
			he2->prev->next = newHE2;
			he2->prev = newHE2;

			std::cout << "saiu" << std::endl;
		}

		//RETURN
		currentSolid->halfEdges.push_back(newHE1);
		currentSolid->halfEdges.push_back(newHE2);
	}

	//MAKE Edge Face
	void mef(CRAB::halfEdge* he1, CRAB::halfEdge* he2, const int& f)
	{
		//Get Solid
		CRAB::solid* currentSolid = he1->leftFace->HedSolid;

		//Initialize
		CRAB::face* newFace = new CRAB::face;
		CRAB::halfEdge* newHE1 = new CRAB::halfEdge;
		CRAB::halfEdge* newHE2 = new CRAB::halfEdge;

		/* FUNCIONANDO PRA APENAS UMA ORIENTAÇÃO */

		//Face
		//ok
		newFace->id = currentSolid->faces.size();
		newFace->HedSolid = currentSolid;

		{
			//nok
			newFace->hEdge = he1;
			//nok
			for (CRAB::halfEdge* he = he1; he != he2; he = he->next)
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

		//RETURN
		currentSolid->halfEdges.push_back(newHE1);
		currentSolid->halfEdges.push_back(newHE2);
	}
}