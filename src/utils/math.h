#ifndef HH_MATH
#define HH_MATH

#define PI 3.141592653589793238462643383279502
#define TWO_PI 6.28318530717958647692528676655900
#define HALF_PI 1.57079632679489661923132169163975
#define E 2.718281828459045235360287471352662

#define deg2rad(x) ((x) * PI / 180.0)
#define rad2deg(x) ((x) * 180.0 / PI)
// #define abs(x) ((x) < 0 ? -(x) : (x))

float Q_rsqrt(float x);
float Q_sin(float x);
float Q_cos(float x);
float Q_tan(float x);

#endif