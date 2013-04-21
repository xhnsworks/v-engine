#ifndef MATH_BASE_H
#define MATH_BASE_H
#define E_PI 3.1415927f
static inline float DegreesToRadian(float deg)
{
    return (deg / 180.0f) * E_PI;
}
#endif
