#ifndef PLANETRYAN
#define PLANETRYAN

#include "Maths/vec3.h"

typedef struct{
    char name[32];
    Vec3 pos;
    Vec3 forward;
    double vel;
    double mass;
    //radius in megameters
    double radius;
} Planet;

Planet* init_planet(const char *name);
void free_planet(Planet *p);

#endif