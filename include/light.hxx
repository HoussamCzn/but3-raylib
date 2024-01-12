#pragma once

#include "color.hxx" // color
#include "vec3.hxx"  // vec3

struct light
{
    vec3 position;
    color color;
    double intensity{1.0};
};