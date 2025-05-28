#ifndef HH_VECTOR
#define HH_VECTOR

#include "math.h"

typedef struct {
    float x;
    float y;
} Vec2;

typedef struct {
    float x;
    float y;
    float z;
} Vec3;

#define vec2(x, y) ((Vec2) {x, y})
#define vec3(x, y, z) ((Vec3) {x, y, z})

#define vec2_add(a, b) (vec2(a.x + b.x, a.y + b.y))
#define vec2_sub(a, b) (vec2(a.x - b.x, a.y - b.y))
#define vec2_mul(a, b) (vec2(a.x * b.x, a.y * b.y))
#define vec2_div(a, b) (vec2(a.x / b.x, a.y / b.y))
#define vec2_scl(a, b) (vec2(a.x * b, a.y * b))
#define vec2_dot(a, b) (a.x * b.x + a.y * b.y)
#define vec2_mag2(a) (a.x * a.x + a.y * a.y)
#define vec2_unit(a) (vec2_scale(a, Q_rsqrt(vec2_mag2(a))))

#define vec3_add(a, b) (vec3(a.x + b.x, a.y + b.y, a.z + b.z))
#define vec3_sub(a, b) (vec3(a.x - b.x, a.y - b.y, a.z - b.z))
#define vec3_mul(a, b) (vec3(a.x * b.x, a.y * b.y, a.z * b.z))
#define vec3_div(a, b) (vec3(a.x / b.x, a.y / b.y, a.z / b.z))
#define vec3_scl(a, b) (vec3(a.x * b, a.y * b, a.z * b))
#define vec3_dot(a, b) (a.x * b.x + a.y * b.y + a.z * b.z)
#define vec3_mag2(a) (a.x * a.x + a.y * a.y + a.z * a.z)
#define vec3_unit(a) (vec3_scale(a, Q_rsqrt(vec3_mag2(a))))

#endif