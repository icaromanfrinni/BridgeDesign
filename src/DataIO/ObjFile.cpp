#include "ObjFile.h"
#include <iostream>
#include <fstream>
#include <sstream>

//DEFAULT CONSTRUCTOR
ObjFile::ObjFile()
{
	objectList.clear();
}

//DESTRUCTOR
ObjFile::~ObjFile()
{
}

//CLEAR OBJ VARIABLE TYPE
void ObjFile::ClearCurrentObj(obj &_obj)
{
	_obj.Name = "";
	_obj.Vertices.clear();
	_obj.vNormals.clear();
	_obj.Lines.clear();
	_obj.Faces.clear();
}

//READ .OBJ FILE
bool ObjFile::ReadObjFile(const std::string& inName)
{
	std::ifstream inFile(inName);
	if (!inFile.is_open())
	{
		std::cerr << "\n\t!!! FILE COULD NOT BE OPENED !!!\n" << std::endl;
		return false;
	}

	//Initialize
	objectList.clear();
	int vertexCount = 0;
	int vertexNormalCount = 0;
	//Read a file line by line
	std::string line;
	obj currentObj;
	bool object = false;

	while (!inFile.eof())
	{
		getline(inFile, line);

		/*-------------------* OBJECT *-------------------*/

		if (line.substr(0, 1) == "o")
		{
			//Objeto anterior, se existir
			//e limpa a variável q recebe o objeto corrente
			if (object == true)
			{
				vertexCount += currentObj.Vertices.size();
				vertexNormalCount += currentObj.vNormals.size();
				objectList.push_back(currentObj);
				ClearCurrentObj(currentObj);
			}

			object = true;							//ativa a leitura de um novo objeto
			currentObj.Name = line.substr(2);		//recebe o nome do objeto
			std::cout << "\n\tReading Object ...: " << currentObj.Name << std::endl;
		}
		else if (object == true)
		{

			/*-------------------* VERTEX NORMAL *-------------------*/

			if (line.substr(0, 2) == "vn")
			{
				CRAB::Vector4Df vertex;
				line = line.substr(2);
				std::istringstream s(line);
				s >> vertex.x;
				s >> vertex.y;
				s >> vertex.z;
				vertex.w = 0.0f;
				currentObj.vNormals.push_back(vertex);
			}

			/*-------------------* VERTEX *-------------------*/

			if (line.substr(0, 1) == "v")
			{
				CRAB::Vector4Df vertex;
				line = line.substr(1);
				std::istringstream s(line);
				s >> vertex.x;
				s >> vertex.y;
				s >> vertex.z;
				vertex.w = 1.0f;
				currentObj.Vertices.push_back(vertex);
			}

			/*--------------------* LINE *--------------------*/

			if (line.substr(0, 1) == "l")
			{
				int vFrom, vTo;
				objLine edge;
				line = line.substr(1);
				std::istringstream s(line);
				s >> vFrom;
				s >> vTo;
				edge.vStart = vFrom - vertexCount;
				edge.vEnd = vTo - vertexCount;
				currentObj.Lines.push_back(edge);
			}

			/*--------------------* FACE *--------------------*/

			if (line.substr(0, 1) == "f")
			{
				unsigned int v, vn;
				char c;
				objFace face;
				line = line.substr(1);
				std::istringstream s(line);

				while (true)
				{
					s >> v >> c >> c >> vn;
					if (!s)
						break;
					face.vertices.push_back(v - vertexCount);
					face.normals.push_back(vn - vertexNormalCount);
				}
				currentObj.Faces.push_back(face);
			}
		}
	}
	//END OF FILE

	//Recebe último objeto válido
	if (object == true)
		objectList.push_back(currentObj);

	std::cout << "\n\tFile [" << inName << "] has been opened successfully!" << std::endl;

	return true;
}

//WRITE .OBJ FILE
//void ObjFile::WriteObjFile(const std::string& name)
//{
//	std::string outName = name + "Out.obj";
//	std::ofstream outFile(outName);
//	if (!outFile.is_open())
//	{
//		std::cerr << "\n\t!!! FILE COULD NOT BE OPENED !!!\n" << std::endl;
//		system("pause");
//		exit(EXIT_FAILURE);
//	}
//
//	/*-------------------* HEADER *-------------------*/
//
//	outFile << "# ICARO 2020 OBJ File" << std::endl;
//	outFile << "# icaro@lia.ufc.br" << std::endl;
//
//	/*------------------* OBJECTS *-------------------*/
//
//	int vCount = 0;
//	for (int i = 0; i < objectList.size(); i++)
//	{
//		//group name (o)
//		outFile << "o " << objectList[i].Name << std::endl;
//
//		//geometric vertices (v)
//		for (int j = 0; j < objectList[i].Vertices.size(); j++)
//			outFile << "v " << objectList[i].Vertices[j].x << " " << objectList[i].Vertices[j].y << " " << "0.00" << std::endl;
//		outFile << "# " << objectList[i].Vertices.size() << " vertices" << std::endl;
//
//		//geometric lines (l)
//		for (int j = 0; j < objectList[i].Lines.size(); j++)
//			outFile << "l " << objectList[i].Lines[j].vStart << " " << objectList[i].Lines[j].vEnd << std::endl;
//		outFile << "# " << objectList[i].Lines.size() << " lines" << std::endl;
//
//		//face vertices (f)
//		outFile << "f";
//		for (int j = 0; j < objectList[i].Faces.size(); j++)
//			outFile << " " << objectList[i].Faces[j];
//		outFile << std::endl;
//	}
//
//	/*--------------------* END *--------------------*/
//
//	outFile.close();
//
//	cout << "\n\tFile [" << outName << "] has been created successfully!" << endl;
//}

