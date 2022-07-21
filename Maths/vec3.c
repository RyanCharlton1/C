#include "Maths/vec3.h"

Vec3 vec3_add(Vec3 a, Vec3 b){
    Vec3 c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    c.z = a.z + b.z;

    return c;
}

Vec3 vec3_sub(Vec3 a, Vec3 b){
    Vec3 c;
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    c.z = a.z - b.z;

    return c;
}

Vec3 vec3_mul(Vec3 v, double x){
    Vec3 c;
    c.x = v.x * x;
    c.y = v.y * x;
    c.z = v.z * x;

    return c;
}

Vec3 vec3_div(Vec3 v, double x){
    Vec3 c;
    c.x = v.x / x;
    c.y = v.y / x;
    c.z = v.z / x;

    return c;
}

double vec3_dot(Vec3 a, Vec3 b){
    double n = 0.0;

    n += a.x * b.x;
    n += a.y * b.y;
    n += a.z * b.z;

    return n;
}
