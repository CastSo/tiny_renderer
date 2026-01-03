#include "vector_math.h"

float dot_vec3f(vector2f a, vector2f b) {
    return a.x * b.x + a.y * b.y;
}

float dot_vec3f(vector3f a, vector3f b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float dot_vec4f(vector4f a, vector4f b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}
