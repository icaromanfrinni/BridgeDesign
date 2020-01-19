#pragma once
#ifndef V_SECTION_H
#define V_SECTION_H

#include "Linear_Algebra.h"
#include "HalfEdge.h"

// Default bridge values
const float SLOPE = 0.02f;
const float TOP_LAYER = 0.07f;
const float GUARD_RAIL = 0.40f;
const float INCLINATION_RATIO = 0.25f;

class V_section
{
public:
    // Road Attributes
    CRAB::Vector4Df start_point;
    CRAB::Vector4Df direction;
    // Input Parameters
    float B;    // Width
    float L;    // Length
    // Bridge Attributes
    float H;    // Height
    float Lb;   // slab cantilever
    float h;    // slab thickness
    float bw;   // webs slab thickness
    float b;    // bottom slab width
    float th;   // horizontal haunch
    float tv;   // vertical haunch

    // Model
    HED::solid* model;

    // default Constructor
    V_section();
    // overload constructor
    V_section(float B, float L, const CRAB::Vector4Df& refPoint, const CRAB::Vector4Df& dir);
    // destructor
    ~V_section();

private:
    void update();
};

#endif // V_SECTION_H