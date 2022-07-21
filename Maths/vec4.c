#include "Maths/vec4.h"

Vec4 vec4_add(Vec4 a, Vec4 b){
    Vec4 c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    c.z = a.z + b.z;
    c.w = a.w + b.w;

    return c;
}

Vec4 vec4_sub(Vec4 a, Vec4 b){
    Vec4 c;
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    c.z = a.z - b.z;
    c.w = a.w - b.w;

    return c;
}

Vec4 vec4_mul(Vec4 v, double x){
    Vec4 c;
    c.x = v.x * x;
    c.y = v.y * x;
    c.z = v.z * x;
    c.w = v.w * x;

    return c;
}

Vec4 vec4_div(Vec4 v, double x){
    Vec4 c;
    c.x = v.x / x;
    c.y = v.y / x;
    c.z = v.z / x;
    c.w = v.w / x;

    return c;
}

double vec4_dot(Vec4 a, Vec4 b){
    double n = 0.0;

    n += a.x * b.x;
    n += a.y * b.y;
    n += a.z * b.z;
    n += a.w * b.w;

    return n;
}
