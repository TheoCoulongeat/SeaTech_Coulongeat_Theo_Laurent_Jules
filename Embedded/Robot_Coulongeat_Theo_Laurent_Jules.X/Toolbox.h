#ifndef TOOLBOX_H
#define TOOLBOX_H

#ifndef PI
#define PI 3.14159265358979323846
#endif

float Abs(float value);
float Max(float value, float value2);
float Min(float value, float value2);
float LimitToInterval(float value, float lowLimit, float highLimit);
float RadianToDegree(float value);
float DegreeToRadian(float value);

#endif // TOOLBOX_H
