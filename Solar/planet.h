#ifndef PLANETRYAN
#define PLANETRYAN

#include "Maths/vec4.h"

typedef struct{
    const char *name;
    Vec4 pos;
    Vec4 forward;
    double vel;
    double mass;
} Planet;

Planet* init_planet(const char *name);
void free_planet(Planet *p);

#endif