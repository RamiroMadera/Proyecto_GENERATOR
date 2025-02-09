#include "mpu6050.h"

static MPU60X0_address_t addr = MPU60X0_ADDRESS_0;
static float prevMagnitude = 0;
static float umbralMagnitudAccel = 1.6;

void mpu6050Init() {
   boardConfig();
   mpu60X0Init(addr);
   mpu60X0Read();
   float AccelX = mpu60X0GetAccelX_mss();
   float AccelY = mpu60X0GetAccelY_mss();
   float AccelZ = mpu60X0GetAccelZ_mss();
   prevMagnitude = sqrt(pow(AccelX, 2) + pow(AccelY, 2) + pow(AccelZ, 2));
}

int agitando() {
   mpu60X0Read();
   float AccelX = mpu60X0GetAccelX_mss();
   float AccelY = mpu60X0GetAccelY_mss();
   float AccelZ = mpu60X0GetAccelZ_mss();
   float currentMagnitude = sqrt(pow(AccelX, 2) + pow(AccelY, 2) + pow(AccelZ, 2));
   int movimiento = fabs(currentMagnitude - prevMagnitude) > umbralMagnitudAccel ? 1 : 0;
   prevMagnitude = currentMagnitude;
   return movimiento;
}