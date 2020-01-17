#pragma once
#ifndef BOXGIRDER_H
#define BOXGIRDER_H

#include "Linear_Algebra.h"
#include "HalfEdge.h"

// Default bridge values
const float SLOPE = 0.30f;
const float TOP_LAYER = 0.07f;
const float GUARD_RAIL = 0.40f;
const float INCLINATION_RATIO = 0.25f;

class BoxGirder
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
    BoxGirder();
    // overload constructor
    BoxGirder(float B, float L, const CRAB::Vector4Df& refPoint, const CRAB::Vector4Df& dir);
    // destructor
    ~BoxGirder();

private:
    void update();
};

#endif // BOXGIRDER_H