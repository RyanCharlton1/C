#ifndef PLANETRYAN
#define PLANETRYAN

#include "Maths/vec3.h"

typedef struct{
    const char *name;
    Vec3 pos;
    Vec3 forward;
    double vel;
    double mass;
} Planet;

Planet* init_planet(const char *name);
void free_planet(Planet *p);

#endif