#ifndef RYANMODEL
#define RYANMODEL

#include "Maths/vec3.h"

//loads only vertices from .obj file(no normals or texture coords)
//returns list of triangles made of 3 vec3 coords
float *load_obj(const char *path, unsigned int *len);

#endif