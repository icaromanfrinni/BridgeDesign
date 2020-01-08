#pragma once

#include "Mesh.h"

#include <vector>

namespace CRAB
{
    std::vector<Mesh> LoadOBJ(const std::string& inName)
    {
        std::vector<Mesh> Mesh_List;

        std::ifstream inFile(inName);
        if (!inFile.is_open())
        {
            std::cerr << "\n\t!!! FILE COULD NOT BE OPENED !!!\n" << std::endl;
            return Mesh_List;
        }

        //Initialize
        std::vector<glm::vec3> OBJ_vertices;
        std::vector<glm::vec3> OBJ_normals;
        std::vector<Vertex> current_Vertex_List;
        bool object = false;
        //Read a file line by line
        std::string line;

        while (!inFile.eof())
        {
            getline(inFile, line);

            /*-------------------* OBJECT *-------------------*/

            if (line.substr(0, 1) == "o")
            {
                if (object == true)
                {
                    Mesh_List.push_back(Mesh(current_Vertex_List));
                    current_Vertex_List.clear();
                }

                object = true;
                std::cout << "\n\tReading Object ...: " << line.substr(2) << std::endl;
            }

            /*-------------------* VERTEX NORMAL *-------------------*/

            if (line.substr(0, 2) == "vn")
            {
                glm::vec3 vertex;
                line = line.substr(2);
                std::istringstream s(line);
                s >> vertex.x;
                s >> vertex.y;
                s >> vertex.z;
                OBJ_normals.push_back(vertex);
            }

            /*-------------------* VERTEX *-------------------*/

            if (line.substr(0, 1) == "v")
            {
                glm::vec3 vertex;
                line = line.substr(1);
                std::istringstream s(line);
                s >> vertex.x;
                s >> vertex.y;
                s >> vertex.z;
                OBJ_vertices.push_back(vertex);
            }

            /*--------------------* FACE *--------------------*/

            if (line.substr(0, 1) == "f")
            {
                Vertex vertex;
                unsigned int v1, v2, v3, vn1, vn2, vn3;
                char c;
                line = line.substr(1);
                std::istringstream s(line);
                s >> v1 >> c >> c >> vn1;
                s >> v2 >> c >> c >> vn2;
                s >> v3 >> c >> c >> vn3;
                vertex.Position = OBJ_vertices[v1 - 1]; vertex.Normal = OBJ_normals[vn1 - 1]; current_Vertex_List.push_back(vertex);
                vertex.Position = OBJ_vertices[v2 - 1]; vertex.Normal = OBJ_normals[vn2 - 1]; current_Vertex_List.push_back(vertex);
                vertex.Position = OBJ_vertices[v3 - 1]; vertex.Normal = OBJ_normals[vn3 - 1]; current_Vertex_List.push_back(vertex);
            }
        }

        if (object == true)
            Mesh_List.push_back(Mesh(current_Vertex_List));

        std::cout << "\n\tFile [" << inName << "] has been opened successfully!" << std::endl;

        return Mesh_List;
    }
}