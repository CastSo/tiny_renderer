

#ifndef COMPONENT_H
#define COMPONENT_H

typedef struct Model {
    struct vector3f* vertices;
    int* triangles;
    int vertices_size;
    int triangles_size;
}Model;

#endif