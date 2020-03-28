
#pragma once

#include <iostream>
#include <Windows.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

#include "Shader.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "Mesh.h"
#include "ObjFile.h"
#include "Skybox.h"

#include "Alignment.h"
#include "Line.h"
#include "CircularArc.h"
#include "Model.h"
#include "BoxGirder.h"

namespace CRAB
{
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    void processInput(GLFWwindow* window);

    // settings
    const unsigned int SCR_WIDTH = 1280;
    const unsigned int SCR_HEIGHT = 720;

    // camera
    Camera camera;
    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    bool firstMouse = true;

    // projection matrix
    glm::mat4 projection = glm::mat4(1.0f);

    // timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // lighting
    DirectionalLight mainLight({ 1.0f, 1.0f, 1.0f }, { -1.0f, -1.0f, -1.0f });

    // mouse event handlers
    int TheKeyState = GLFW_KEY_LEFT_CONTROL;

    //List of Solids (to load .obj file)
    std::vector<HED::solid*> solids;

    /* ================== BRIDGE DESIGN ================== */

    //List of Bridge Models
    std::vector<Model> models;
    //List of Alignments
    std::vector<Alignment> alignments;
    //List of Roads
    std::vector<Road*> roads;

    /* ====================== END ====================== */

    // List of Meshes
    std::vector<Mesh> ourMesh_List;

    int main()
    {
        // glfw: initialize and configure
        // ------------------------------
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
        GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CRAB::BRIDGE", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        // tell GLFW to capture our mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }

        // configure global opengl state
        // -----------------------------
        glEnable(GL_DEPTH_TEST);

        // build and compile our shader zprogram
        // -------------------------------------
        Shader ourShader("shaders/shader.vert", "shaders/shader.frag");
        Shader skyboxShader("shaders/skybox.vert", "shaders/skybox.frag");
        
        // load SKYBOX textures
        // --------------------
        //std::vector<std::string> faces = {
        //    "skybox/right.jpg",        // right
        //    "skybox/left.jpg",        // left
        //    "skybox/top.jpg",        // top
        //    "skybox/bottom.jpg",        // bottom
        //    "skybox/front.jpg",        // front
        //    "skybox/back.jpg"         // back
        //};
        std::vector<std::string> faces = {
           "skybox/color/side.jpg",        // right
           "skybox/color/side.jpg",        // left
           "skybox/color/top.jpg",        // top
           "skybox/color/bottom.jpg",        // bottom
           "skybox/color/side.jpg",        // front
           "skybox/color/side.jpg"         // back
        };
        Skybox skybox(faces);

        // shader configuration
        // --------------------
        skyboxShader.use();
        skyboxShader.setInt("skybox", 0);

        // load models (from OBJ file)
        // ---------------------------
        //std::vector<Mesh> ourMesh_List;
        //ourMesh_List = CRAB::LoadOBJ("objects/cubes.obj");

        //fill the road list
        roads.push_back(new Road("ROAD", 12.80f, 80.0f));
        /*roads.push_back(new Road("Classe 0", 11.90f));
        roads.push_back(new Road("Classe I-A", 11.60f));
        roads.push_back(new Road("Classe I-B", 12.80f));
        roads.push_back(new Road("Classe II", 12.80f));
        roads.push_back(new Road("Classe III", 10.80f));
        roads.push_back(new Road("Classe IV", 9.80f));*/

        alignments.push_back(Alignment("Rodovia_001", roads.back()));

        // EXEMPLO RETO (200 metros)
        alignments.back().AddSegment(new Line(CRAB::Vector4Df{ 0.0f, 0.0f, 0.0f, 1.0f }, CRAB::Vector4Df{ 200.0f, 0.0f, 0.0f, 1.0f }));
        
        // EXEMPLO LONGO
        /*alignments.back().AddSegment(new Line(CRAB::Station{ 0, 0.0f, 8.663f }, CRAB::Station{ 2, 0.0f, 11.269f }));
        alignments.back().AddSegment(new CircularArc(CRAB::Station{ 2, 0.0f, 11.269f }, CRAB::Station{ 4, 5.0f, 14.200f }, CRAB::Station{ 6, 10.0f, 11.266f }));
        alignments.back().AddSegment(new Line(CRAB::Station{ 6, 10.0f, 11.266f }, CRAB::Station{ 8, 0.0f, 9.310f }));*/

        // EXEMPLO CURVA HORIZONTAL
        /*alignments.back().AddSegment(new Line(CRAB::Vector4Df{ 0.0f, 0.0f, 0.0f, 1.0f }, CRAB::Vector4Df{ 20.0f, 0.0f, 0.0f, 1.0f }));
        alignments.back().AddSegment(new CircularArc(CRAB::Vector4Df{ 20.0f, 0.0f, 0.0f, 1.0f }, CRAB::Vector4Df{ 70.0f, 0.0f, 0.0f, 1.0f }, CRAB::Vector4Df{ 110.0f, 0.0f, 30.0f, 1.0f }));*/

        // EXEMPLO CURVA 3D
        //alignments.back().AddSegment(new CircularArc(CRAB::Vector4Df{ 0.0f, 0.0f, 0.0f, 1.0f }, CRAB::Vector4Df{ 50.0f, 10.0f, 0.0f, 1.0f }, CRAB::Vector4Df{ 100.0f, 0.0f, 50.0f, 1.0f }));


        // load models (bridges)
        // ---------------------
        models.push_back(Model("Rio_Pacoti", new BoxGirder(alignments.back(), 0.5f, 6.0f, 100.0f)));

        for (int i = 0; i < models.back().getBridge()->getModel().size(); i++)
            ourMesh_List.push_back(Mesh(models.back().getBridge()->getModel()[i]));

        // draw in wireframe
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // pass projection matrix to shader (as projection matrix rarely changes there's no need to do this per frame)
        // -----------------------------------------------------------------------------------------------------------
        projection = glm::perspective(glm::radians(camera.FieldOfView), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 500.0f);

        // render loop
        // -----------
        while (!glfwWindowShouldClose(window))
        {
            // per-frame time logic
            // --------------------
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // input
            // -----
            processInput(window);

            // render
            // ------
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            ourShader.use();
            ourShader.setVec3("viewPos", camera.Position);

            // light properties
            // ----------------
            // directional light
            ourShader.setDirLight(mainLight);

            // view/projection transformations
            glm::mat4 view = camera.GetViewMatrix();
            ourShader.setMat4("projection", projection);
            ourShader.setMat4("view", view);

            // world transformation
            glm::mat4 model = glm::mat4(1.0f);
            //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
            ourShader.setMat4("model", model);

            // render
            for (int i = 0; i < ourMesh_List.size(); i++)
            {
                ourMesh_List[i].Draw(ourShader);
            }

            // draw skybox as last
            glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
            skyboxShader.use();
            view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
            skyboxShader.setMat4("view", view);
            skyboxShader.setMat4("projection", projection);
            // skybox cube
            skybox.Draw(ourShader);
            glDepthFunc(GL_LESS); // set depth function back to default

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        // glfw: terminate, clearing all previously allocated GLFW resources.
        // ------------------------------------------------------------------
        glfwTerminate();
        return 0;
    }

    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    void processInput(GLFWwindow* window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(UP, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(DOWN, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);

        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            TheKeyState = GLFW_KEY_LEFT_CONTROL;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            TheKeyState = GLFW_KEY_LEFT_SHIFT;

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            for (int i = 0; i < models.size(); i++)
                HED::WriteObjFile(models[i].getBridge()->getModel());
        }
        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
        {
            solids.clear();
            ourMesh_List.clear();
        }
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        {
            std::string fileName;
            std::cout << "Enter file name (*.obj): " << std::endl;
            std::cin >> fileName;
            fileName = "objects/" + fileName + ".obj";
            ObjFile inputObjFile;
            if (inputObjFile.ReadObjFile(fileName) == false) {
                MessageBox(NULL, (LPCWSTR)L"File could not be opened!", (LPCWSTR)L"Info",
                    MB_OK | MB_ICONEXCLAMATION);
            }
            else {
                //solids.clear();
                //ourMesh_List.clear();
                for (int i = 0; i < inputObjFile.objectList.size(); i++)
                {
                    // from OBJ -> Mesh
                    // ----------------
                    ourMesh_List.push_back(Mesh(inputObjFile.objectList[i]));
                    // from OBJ -> HalfEdge -> Mesh
                    // ----------------------------
                    //solids.push_back(new HED::solid(i, inputObjFile.objectList[i]));
                    //ourMesh_List.push_back(Mesh(solids[i]));
                }
            }
        }
    }

    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // ---------------------------------------------------------------------------------------------
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);

        // update
        projection = glm::perspective(glm::radians(camera.FieldOfView), (float)width / (float)height, 0.1f, 500.0f);
    }

    // glfw: whenever the mouse moves, this callback is called
    // -------------------------------------------------------
    void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
        {
            if (TheKeyState == GLFW_KEY_LEFT_SHIFT)
            {
                float radius = glm::length(camera.View - camera.Position);
                camera.ProcessMouseMovement(xoffset, yoffset);
                camera.View = camera.Position + camera.LookAt * radius;
            }
            if (TheKeyState == GLFW_KEY_LEFT_CONTROL)
            {
                float radius = glm::length(camera.View - camera.Position);
                camera.ProcessMouseMovement(xoffset, yoffset);
                camera.Position = camera.View - camera.LookAt * radius;
            }
        }
    }

    // glfw: whenever the mouse scroll wheel scrolls, this callback is called
    // ----------------------------------------------------------------------
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        camera.ProcessMouseScroll(yoffset);
    }
}