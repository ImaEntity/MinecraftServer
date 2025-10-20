#ifndef HH_SERVER_VECTOR
#define HH_SERVER_VECTOR

#include <math.h>

typedef struct {
    float x;
    float y;
} Vec2f;

typedef struct {
    double x;
    double y;
} Vec2d;

typedef struct {
    float x;
    float y;
    float z;
} Vec3f;

typedef struct {
    double x;
    double y;
    double z;
} Vec3d;

typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec4f;

typedef struct {
    double x;
    double y;
    double z;
    double w;
} Vec4d;

static inline Vec2f vec2f(float x, float y) { return (Vec2f) {x, y}; }
static inline Vec2f vec2f_add(Vec2f a, Vec2f b) { return vec2f(a.x + b.x, a.y + b.y); }
static inline Vec2f vec2f_sub(Vec2f a, Vec2f b) { return vec2f(a.x - b.x, a.y - b.y); }
static inline Vec2f vec2f_mul(Vec2f a, Vec2f b) { return vec2f(a.x * b.x, a.y * b.y); }
static inline Vec2f vec2f_div(Vec2f a, Vec2f b) { return vec2f(a.x / b.x, a.y / b.y); }
static inline Vec2f vec2f_scl(Vec2f a, float b) { return vec2f(a.x * b, a.y * b); }
static inline float vec2f_dot(Vec2f a, Vec2f b) { return a.x * b.x + a.y * b.y; }
static inline float vec2f_mag2(Vec2f a) { return a.x * a.x + a.y * a.y; }
static inline Vec2f vec2f_unit(Vec2f a) { return vec2f_scl(a, sqrtf(vec2f_mag2(a))); }

static inline Vec2d vec2d(double x, double y) { return (Vec2d) {x, y}; }
static inline Vec2d vec2d_add(Vec2d a, Vec2d b) { return vec2d(a.x + b.x, a.y + b.y); }
static inline Vec2d vec2d_sub(Vec2d a, Vec2d b) { return vec2d(a.x - b.x, a.y - b.y); }
static inline Vec2d vec2d_mul(Vec2d a, Vec2d b) { return vec2d(a.x * b.x, a.y * b.y); }
static inline Vec2d vec2d_div(Vec2d a, Vec2d b) { return vec2d(a.x / b.x, a.y / b.y); }
static inline Vec2d vec2d_scl(Vec2d a, double b) { return vec2d(a.x * b, a.y * b); }
static inline double vec2d_dot(Vec2d a, Vec2d b) { return a.x * b.x + a.y * b.y; }
static inline double vec2d_mag2(Vec2d a) { return a.x * a.x + a.y * a.y; }
static inline Vec2d vec2d_unit(Vec2d a) { return vec2d_scl(a, sqrt(vec2d_mag2(a))); }

static inline Vec3f vec3f(float x, float y, float z) { return (Vec3f) {x, y, z}; }
static inline Vec3f vec3f_add(Vec3f a, Vec3f b) { return vec3f(a.x + b.x, a.y + b.y, a.z + b.z); }
static inline Vec3f vec3f_sub(Vec3f a, Vec3f b) { return vec3f(a.x - b.x, a.y - b.y, a.z - b.z); }
static inline Vec3f vec3f_mul(Vec3f a, Vec3f b) { return vec3f(a.x * b.x, a.y * b.y, a.z * b.z); }
static inline Vec3f vec3f_div(Vec3f a, Vec3f b) { return vec3f(a.x / b.x, a.y / b.y, a.z / b.z); }
static inline Vec3f vec3f_scl(Vec3f a, float b) { return vec3f(a.x * b, a.y * b, a.z * b); }
static inline float vec3f_dot(Vec3f a, Vec3f b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static inline Vec3f vec3f_cross(Vec3f a, Vec3f b) { return vec3f(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
static inline float vec3f_mag2(Vec3f a) { return a.x * a.x + a.y * a.y + a.z * a.z; }
static inline Vec3f vec3f_unit(Vec3f a) { return vec3f_scl(a, sqrtf(vec3f_mag2(a))); }

static inline Vec3d vec3d(double x, double y, double z) { return (Vec3d) {x, y, z}; }
static inline Vec3d vec3d_add(Vec3d a, Vec3d b) { return vec3d(a.x + b.x, a.y + b.y, a.z + b.z); }
static inline Vec3d vec3d_sub(Vec3d a, Vec3d b) { return vec3d(a.x - b.x, a.y - b.y, a.z - b.z); }
static inline Vec3d vec3d_mul(Vec3d a, Vec3d b) { return vec3d(a.x * b.x, a.y * b.y, a.z * b.z); }
static inline Vec3d vec3d_div(Vec3d a, Vec3d b) { return vec3d(a.x / b.x, a.y / b.y, a.z / b.z); }
static inline Vec3d vec3d_scl(Vec3d a, double b) { return vec3d(a.x * b, a.y * b, a.z * b); }
static inline double vec3d_dot(Vec3d a, Vec3d b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static inline Vec3d vec3d_cross(Vec3d a, Vec3d b) { return vec3d(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
static inline double vec3d_mag2(Vec3d a) { return a.x * a.x + a.y * a.y + a.z * a.z; }
static inline Vec3d vec3d_unit(Vec3d a) { return vec3d_scl(a, sqrt(vec3d_mag2(a))); }

static inline Vec4f vec4f(float x, float y, float z, float w) { return (Vec4f) {x, y, z, w}; }
static inline Vec4f vec4f_add(Vec4f a, Vec4f b) { return vec4f(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
static inline Vec4f vec4f_sub(Vec4f a, Vec4f b) { return vec4f(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
static inline Vec4f vec4f_mul(Vec4f a, Vec4f b) { return vec4f(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }
static inline Vec4f vec4f_div(Vec4f a, Vec4f b) { return vec4f(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }
static inline Vec4f vec4f_scl(Vec4f a, float b) { return vec4f(a.x * b, a.y * b, a.z * b, a.w * b); }
static inline float vec4f_dot(Vec4f a, Vec4f b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
static inline float vec4f_mag2(Vec4f a) { return a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w; }
static inline Vec4f vec4f_unit(Vec4f a) { return vec4f_scl(a, sqrtf(vec4f_mag2(a))); }

static inline Vec4d vec4d(double x, double y, double z, double w) { return (Vec4d) {x, y, z, w}; }
static inline Vec4d vec4d_add(Vec4d a, Vec4d b) { return vec4d(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
static inline Vec4d vec4d_sub(Vec4d a, Vec4d b) { return vec4d(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
static inline Vec4d vec4d_mul(Vec4d a, Vec4d b) { return vec4d(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }
static inline Vec4d vec4d_div(Vec4d a, Vec4d b) { return vec4d(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }
static inline Vec4d vec4d_scl(Vec4d a, double b) { return vec4d(a.x * b, a.y * b, a.z * b, a.w * b); }
static inline double vec4d_dot(Vec4d a, Vec4d b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
static inline double vec4d_mag2(Vec4d a) { return a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w; }
static inline Vec4d vec4d_unit(Vec4d a) { return vec4d_scl(a, sqrt(vec4d_mag2(a))); }

#endif