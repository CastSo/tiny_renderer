#pragma once

typedef struct Model {
    struct vector3f* vertices;
    int* triangles;
    int vertices_size;
    int triangles_size;
}Model;

