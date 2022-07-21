#ifndef VEC3RYAN
#define VEC3RYAN

typedef struct{
    double x, y, z;
} Vec3;

Vec3 vec3_add(Vec3 a, Vec3 b);
Vec3 vec3_sub(Vec3 a, Vec3 b);
Vec3 vec3_mul(Vec3 n, double x);
Vec3 vec3_div(Vec3 n, double x);
double vec3_dot(Vec3 a, Vec3 b);

#endif