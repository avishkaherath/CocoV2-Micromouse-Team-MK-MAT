#ifndef ICM_SENSOR_H
#define ICM_SENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include "ICM_20948.h"

#define AD0_VAL 1 // Define it here

class ICMSensor {
public:
    void setup();
    void loop();
private:
    ICM_20948_I2C myICM; // Sensor object
};

#endif // ICM_SENSOR_H