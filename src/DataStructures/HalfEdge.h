#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Linear_Algebra.h"
//#include "LoadOBJ.h"
#include "ObjFile.h"

namespace CRAB
{
	typedef struct vertex Vertex;
	//typedef struct edge Edge;
	typedef struct halfEdge HalfEdge;
	typedef struct face Face;
	typedef struct solid Solid;

	struct vertex
	{
		int id;
		//halfEdge *hEdge;
		CRAB::Vector4Df point;
		int enable;
	};

	//struct edge
	//{
	//	int id;
	//	halfEdge *he1, *he2;
	//};

	struct halfEdge
	{
		int id;
		halfEdge* opp, * next, * prev;
		vertex* vStart;
		//Edge *edge;
		face* leftFace;
	};

	struct face
	{
		int id;
		CRAB::Vector4Df normal;
		halfEdge* hEdge;
		solid* HedSolid;
	};

	struct solid
	{
		int id;
		std::string name;
		std::vector<vertex*> vertices;
		//std::vector<edge> edges;
		std::vector<halfEdge*> halfEdges;
		std::vector<face*> faces;

		//transform
		CRAB::Vector4Df location, scale;

		//Constructor
		solid() {
			location = { 0.0f, 0.0f, 0.0f, 1.0f };
			scale = { 1.0f, 1.0f, 1.0f, 0.0f };
		}

		//Overload Constructor (from OBJ File)
		solid(const int& index, const obj& OBJ) : name(OBJ.Name)
		{
			//Initialize
			id = index;
			vertices.resize(OBJ.Vertices.size());
			//edges.resize(OBJ.Lines.size());
			faces.resize(OBJ.Faces.size());
			location = { 0.0f, 0.0f, 0.0f, 1.0f };
			scale = { 1.0f, 1.0f, 1.0f, 0.0f };


			for (int i = 0; i < OBJ.Faces.size(); i++)
				for (int j = 0; j < OBJ.Faces[i].vertices.size(); j++)
					halfEdges.push_back(new CRAB::halfEdge);

			//Vertices
			for (int i = 0; i < OBJ.Vertices.size(); i++)
			{
				//PEGA CADA V�RTICE DO *.OBJ
				vertices[i] = new CRAB::vertex;
				vertices[i]->id = i;
				vertices[i]->point = OBJ.Vertices[i];
				vertices[i]->enable = 0;
			}

			//Faces
			int heCount = 0; //contador de 'half-edges'
			for (int i = 0; i < OBJ.Faces.size(); i++)
			{
				//PARA CADA FACE DO *.OBJ
				faces[i] = new CRAB::face;
				faces[i]->id = i; //atribui um 'id'
				faces[i]->normal = OBJ.vNormals[OBJ.Faces[i].normals[0] - 1]; //prrimeiro vetor normal da face do OBJ File
				faces[i]->hEdge = halfEdges[heCount]; //primeira 'half-edge' da face fica sendo a refer�ncia
				faces[i]->HedSolid = this;

				for (int j = 0; j < OBJ.Faces[i].vertices.size(); j++)
				{
					halfEdges[heCount]->id = heCount; //atribui um 'id'
					halfEdges[heCount]->leftFace = faces[i]; //todas as 'half-edges' da face recebem como refer�ncia a face corrente
					halfEdges[heCount]->vStart = vertices[OBJ.Faces[i].vertices[j] - 1];
					halfEdges[heCount]->opp = NULL; //por enquanto

					if (j == 0) //se for a primeira 'half-edge' do loop
					{
						halfEdges[heCount]->next = halfEdges[heCount + 1];
						halfEdges[heCount]->prev = halfEdges[heCount + OBJ.Faces[i].vertices.size() - 1];
					}
					else if (j == OBJ.Faces[i].vertices.size() - 1) //se for a �ltima 'half-edge' do loop
					{
						halfEdges[heCount]->next = halfEdges[heCount - OBJ.Faces[i].vertices.size() + 1];
						halfEdges[heCount]->prev = halfEdges[heCount - 1];
					}
					else //se for uma 'half-edge' intermedi�ria
					{
						halfEdges[heCount]->next = halfEdges[heCount + 1];
						halfEdges[heCount]->prev = halfEdges[heCount - 1];
					}

					heCount++; //'id' da pr�xima 'half-edge'
				}
			}

			//find the opposite half-edge
			for (int i = 0; i < halfEdges.size(); i++)
				for (int j = 0; j < halfEdges.size(); j++)
				{
					if (halfEdges[j]->vStart == halfEdges[i]->next->vStart &&
						halfEdges[i]->vStart == halfEdges[j]->next->vStart)
						halfEdges[i]->opp = halfEdges[j];
				}
		}

		void transform(CRAB::Matrix4 m)
		{
			for (int i = 0; i < vertices.size(); i++)
				vertices[i]->point = m * vertices[i]->point;
		}

		float Area()
		{
			float area = 0;

			for (int j = 0; j < faces.size(); j++)
			{
				CRAB::halfEdge* he = faces[j]->hEdge->next;

				while (he != faces[j]->hEdge)
				{
					CRAB::Vector4Df P1P2 = he->vStart->point - faces[j]->hEdge->vStart->point;
					CRAB::Vector4Df P1P3 = he->next->vStart->point - faces[j]->hEdge->vStart->point;
					area += cross(P1P2, P1P3).length();
					he = he->next;
				}
			}

			return area / 2.0f;
		}
	};
} // end namespace