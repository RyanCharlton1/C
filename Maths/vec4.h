#ifndef VEC4RYAN
#define VEC4RYAN

typedef struct{
    double x, y, z, w;
} Vec4;

Vec4 vec4_add(Vec4 a, Vec4 b);
Vec4 vec4_sub(Vec4 a, Vec4 b);
Vec4 vec4_mul(Vec4 n, double x);
Vec4 vec4_div(Vec4 n, double x);
double vec4_dot(Vec4 a, Vec4 b);

#endif