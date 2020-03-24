#pragma once

#ifndef SKYBOX_H
#define SKYBOX_H

#include <glad\glad.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#include <vector>
#include <string>
#include <iostream>

class Skybox {
public:
    Skybox();
    Skybox(std::vector<std::string>& faces);
    ~Skybox();

    void Draw(Shader shader);

private:
    std::vector<float> skyboxVertices;
    unsigned int skyboxVAO, skyboxVBO;
    unsigned int cubemapTexture;

    unsigned int loadCubemap(std::vector<std::string> &faces);
};
#endif