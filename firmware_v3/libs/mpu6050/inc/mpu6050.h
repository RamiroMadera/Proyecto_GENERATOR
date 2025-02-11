#ifndef MPU6050_H
#define MPU6050_H

#include "sapi.h"
#include <math.h>

void mpu6050Init();
int agitando(float umbralMagnitudAccel);

#endif // MPU6050_H