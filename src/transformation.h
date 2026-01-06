#pragma once
#include "type.h"
#include "math.h"
#include "vector_math.h"
#define _USE_MATH_DEFINES

void project(vector3f *v, int width, int height);

matrix4f viewport(int x, int y, int w, int h);
matrix4f perspective(double f);
matrix4f lookat(vector3f eye, vector3f center, vector3f up); 

void rotation(vector3f *v, double a);
