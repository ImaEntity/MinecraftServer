#include "math.h"

typedef union {
    float f;
    unsigned int i;
} float_bits;

float Q_rsqrt(float x) {
    float x2 = x * 0.5f;

    int i = ((float_bits) {x}).i;
    i = 0x5f3759df - (i >> 1);

    x = ((float_bits) {i}).f;
    return x * 0.703952253f * (2.38924456f - (x2 * x * x));
}

float Q_sin(float x) {
    return Q_cos(x - HALF_PI);
}

float Q_cos(float x) {
    while(x < -PI) x += TWO_PI;
    while(x >  PI) x -= TWO_PI;

    if(x >  HALF_PI) return -Q_cos(x - PI);
    if(x < -HALF_PI) return -Q_cos(x + PI);

    float x2 = x * x;
    float tpi2 = TWO_PI * TWO_PI;
    return 1.0f - (20.0f * x2) / (4.0f * x2 + tpi2);
}

float Q_tan(float x) {
    float s = Q_sin(x);
    float c = Q_cos(x);
    
    if(c == 0.0f) return 1e30f;
    return s / c;
}