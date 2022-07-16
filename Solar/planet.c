#include <stdlib.h>
#include <string.h>

#include "Solar/planet.h"

//name must be 0 terminated string
//returns blank planet with name
Planet* init_planet(const char *name){
    Planet *p = calloc(1, sizeof(Planet));

    int len = strlen(name);
    p->name = malloc(len + 1);
    memcpy(p->name, name, len + 1);     //copy terminating 0 also

    return p;
}

void free_planet(Planet *p){
    free(p->name);
    free(p);
}