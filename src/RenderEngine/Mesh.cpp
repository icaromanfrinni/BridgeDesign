#include "Mesh.h"
#include "Geometry.h"

#define PRINT 0

// constructor
Mesh::Mesh(std::vector<Mesh::Vertex> vertices)
{
    this->vertices = vertices;

    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    setupMesh();
}

// constructor (from OBJ struct)
Mesh::Mesh(const obj& object)
{
    if (!object.vTexture.empty())
    {
        material.hasTexture = true;
        material.textures.push_back(new Texture("textures/" + object.Name + "_diffuse.jpg", "diffuse"));
        material.textures.push_back(new Texture("textures/" + object.Name + "_specular.jpg", "specular"));
        material.textures.push_back(new Texture("textures/" + object.Name + "_normal.jpg", "normal"));
    }

    /*for (int i = 0; i < object.Faces.size(); i++)
        for (int j = 0; j < object.Faces[i].vertices.size(); j++)
        {
            unsigned int index;
            Mesh::Vertex vertex;

            index = object.Faces[i].vertices[j] - 1;
            vertex.Position = glm::vec3(object.Vertices[index].x, object.Vertices[index].y, object.Vertices[index].z);
            index = object.Faces[i].normals[j] - 1;
            vertex.Normal = glm::vec3(object.vNormals[index].x, object.vNormals[index].y, object.vNormals[index].z);

            if (material.hasTexture)
            {
                index = object.Faces[i].textures[j] - 1;
                vertex.TexCoords = glm::vec2(object.vTexture[index].x, object.vTexture[index].y);
            }

            vertices.push_back(vertex);
        }*/

    for (int i = 0; i < object.Faces.size(); i++)
    {
        unsigned int index;
        Mesh::Vertex vertex1, vertex2, vertex3;
        // ------------------------------------
        index = object.Faces[i].vertices[0] - 1;
        vertex1.Position = glm::vec3(object.Vertices[index].x, object.Vertices[index].y, object.Vertices[index].z);
        index = object.Faces[i].vertices[1] - 1;
        vertex2.Position = glm::vec3(object.Vertices[index].x, object.Vertices[index].y, object.Vertices[index].z);
        index = object.Faces[i].vertices[2] - 1;
        vertex3.Position = glm::vec3(object.Vertices[index].x, object.Vertices[index].y, object.Vertices[index].z);
        // ------------------------------------
        index = object.Faces[i].normals[0] - 1;
        vertex1.Normal = glm::vec3(object.vNormals[index].x, object.vNormals[index].y, object.vNormals[index].z);
        index = object.Faces[i].normals[1] - 1;
        vertex2.Normal = glm::vec3(object.vNormals[index].x, object.vNormals[index].y, object.vNormals[index].z);
        index = object.Faces[i].normals[2] - 1;
        vertex3.Normal = glm::vec3(object.vNormals[index].x, object.vNormals[index].y, object.vNormals[index].z);
        // ------------------------------------
        if (material.hasTexture)
        {
            index = object.Faces[i].textures[0] - 1;
            vertex1.TexCoords = glm::vec2(object.vTexture[index].x, object.vTexture[index].y);
            index = object.Faces[i].textures[1] - 1;
            vertex2.TexCoords = glm::vec2(object.vTexture[index].x, object.vTexture[index].y);
            index = object.Faces[i].textures[2] - 1;
            vertex3.TexCoords = glm::vec2(object.vTexture[index].x, object.vTexture[index].y);

            glm::vec3 tangent = Calc_Tangent(vertex1, vertex2, vertex3);
            vertex1.Tangent = tangent;
            vertex2.Tangent = tangent;
            vertex3.Tangent = tangent;
        }
        // ------------------------------------
        vertices.push_back(vertex1);
        vertices.push_back(vertex2);
        vertices.push_back(vertex3);
    }

    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    setupMesh();
}

// constructor (from HalfEdge struct)
Mesh::Mesh(const HED::solid* solid)
{
#if PRINT == 1
    std::cout << "solid " << solid->name << "....................." << std::endl;
#endif

    material.hasTexture = true;
    if (solid->name == "TOP_LAYER")
        material.textures = texPavement;
    else
        material.textures = texConcrete;
    /*if (solid->name == "TOP_LAYER")
    {
        material.textures.push_back(new Texture("textures/half_pavement_diffuse.jpg", "diffuse"));
        material.textures.push_back(new Texture("textures/half_pavement_roughness.jpg", "specular"));
        material.textures.push_back(new Texture("textures/half_pavement_normal.jpg", "normal"));
    }
    else
    {
        material.textures.push_back(new Texture("textures/Concrete_Wall_002_height.png", "diffuse"));
        material.textures.push_back(new Texture("textures/Concrete_Wall_002_ambient_occlusion.jpg", "specular"));
        material.textures.push_back(new Texture("textures/Concrete_Wall_002_normal.jpg", "normal"));
    }*/

    // DEBUG
    /*if (solid->name == "TOP_LAYER")
    {
        material.hasTexture = false;
        material.ka = { 0.1f, 0.1f, 0.1f };
        material.kd = { 0.1f, 0.1f, 0.1f };
        material.ks = { 0.5f, 0.5f, 0.5f };
    }*/
    /*if (solid->name == "ROAD")
    {
        material.hasTexture = false;
        material.ka = { 0.1f, 0.1f, 0.1f };
        material.kd = { 0.1f, 0.1f, 0.1f };
        material.ks = { 0.5f, 0.5f, 0.5f };
    }*/
    
    for (int i = 0; i < solid->faces.size(); i++)
    {
#if PRINT == 1
        std::cout << "i = " << i << std::endl;
#endif
        // Initialize Polygon
        CRAB::Polygon p;
        HED::halfEdge* he = solid->faces[i]->hEdge;
        p.v.push_back(he->vStart->point);
        for (he = solid->faces[i]->hEdge->next; he != solid->faces[i]->hEdge; he = he->next)
            p.v.push_back(he->vStart->point);
        
        // Triangulate (apenas as faces com mais de 4 vértices)
        std::vector<CRAB::Triangle> t;
        if (p.v.size() == 4)
        {
            t.push_back(CRAB::Triangle(p.v[3], p.v[0], p.v[1]));
            t.push_back(CRAB::Triangle(p.v[3], p.v[1], p.v[2]));
        }
        else
            p.triangulate(t);
        /*if (p.v.size() > 4)
            p.triangulate(t);
        else if (p.v.size() == 4)
        {
            t.push_back(CRAB::Triangle(p.v[3], p.v[0], p.v[1]));
            t.push_back(CRAB::Triangle(p.v[3], p.v[1], p.v[2]));
        }
        else
            t.push_back(CRAB::Triangle(p.v[0], p.v[1], p.v[2]));*/
        
        // MESH
        CRAB::Vector4Df normal_plane = solid->faces[i]->normal();
        for (int j = 0; j < t.size(); j++)
        {
#if PRINT == 1
            std::cout << "j = " << j << std::endl;
#endif
            Mesh::Vertex vertex1, vertex2, vertex3;
            // ------------------------------------
            glm::vec3 v_normal = glm::vec3(normal_plane.x, normal_plane.y, normal_plane.z);
            // ------------------------------------
            vertex1.Normal = v_normal;
            vertex2.Normal = v_normal;
            vertex3.Normal = v_normal;
            // ------------------------------------
            vertex1.Position = glm::vec3(t[j].v[0].x, t[j].v[0].y, t[j].v[0].z);
            vertex2.Position = glm::vec3(t[j].v[1].x, t[j].v[1].y, t[j].v[1].z);            
            vertex3.Position = glm::vec3(t[j].v[2].x, t[j].v[2].y, t[j].v[2].z);
            // ------------------------------------
            if (j % 2 == 0)
            {
                float y = glm::distance(vertex1.Position, vertex2.Position);
                float x = glm::distance(vertex2.Position, vertex3.Position);
                /*vertex1.TexCoords = { 1.0f, 0.0f };
                vertex2.TexCoords = { 1.0f, 1.0f };
                vertex3.TexCoords = { 0.0f, 1.0f };*/
                vertex1.TexCoords = { x/y, 0.0f };
                vertex2.TexCoords = { x/y, 1.0f };
                vertex3.TexCoords = { 0.0f, 1.0f };
            }
            else
            {
                float y = glm::distance(vertex2.Position, vertex3.Position);
                float x = glm::distance(vertex1.Position, vertex3.Position);
                /*vertex1.TexCoords = { 1.0f, 0.0f };
                vertex2.TexCoords = { 0.0f, 1.0f };
                vertex3.TexCoords = { 0.0f, 0.0f };*/
                vertex1.TexCoords = { x/y, 0.0f };
                vertex2.TexCoords = { 0.0f, 1.0f };
                vertex3.TexCoords = { 0.0f, 0.0f };
            }
            // ------------------------------------
            glm::vec3 tangent = Calc_Tangent(vertex1, vertex2, vertex3);
            vertex1.Tangent = tangent;
            vertex2.Tangent = tangent;
            vertex3.Tangent = tangent;
            // ------------------------------------
            vertices.push_back(vertex1);            
            vertices.push_back(vertex2);
            vertices.push_back(vertex3);
        }
    }
    
    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    setupMesh();

#if PRINT == 1
    std::cout << "OK" << std::endl;;
#endif
}

// destructor
Mesh::~Mesh()
{
}

// render the mesh
void Mesh::Draw(Shader shader)
{
    // material properties
    shader.setFloat("material.shininess", material.shininess);
    if (material.hasTexture)
    {
        shader.setBool("material.hasTexture", true);

        for (int i = 0; i < material.textures.size(); i++)
        {
            // active proper texture unit before binding
            glActiveTexture(GL_TEXTURE0 + i);

            if (material.textures[i]->type == "diffuse")
                shader.setInt("u_Texture_diffuse", i);
            else if (material.textures[i]->type == "specular")
                shader.setInt("u_Texture_specular", i);
            else if (material.textures[i]->type == "normal")
                shader.setInt("u_Texture_normal", i);

            // bind the texture
            glBindTexture(GL_TEXTURE_2D, material.textures[i]->Texture_ID);
        }
    }
    else
    {
        shader.setBool("material.hasTexture", false);

        shader.setVec3("material.ka", material.ka);
        shader.setVec3("material.kd", material.kd);
        shader.setVec3("material.ks", material.ks);
    }

    // draw mesh
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

// initializes all the buffer objects/arrays
void Mesh::setupMesh()
{
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

    glBindVertexArray(0);
}

glm::vec3 Mesh::Calc_Tangent(const Vertex& v1, const Vertex& v2, const Vertex& v3)
{
    glm::vec3 t;

    glm::vec3 edge1 = v2.Position - v1.Position;
    glm::vec3 edge2 = v3.Position - v1.Position;
    glm::vec2 deltaUV1 = v2.TexCoords - v1.TexCoords;
    glm::vec2 deltaUV2 = v3.TexCoords - v1.TexCoords;

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    t.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    t.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    t.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    t = glm::normalize(t);

    return t;
}