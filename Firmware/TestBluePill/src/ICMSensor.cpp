#include "ICMSensor.h"

void ICMSensor::setup() {
    Serial.begin(115200);
    while (!Serial); // Wait for the serial port to connect. Necessary for boards like Leonardo only
    Serial.println(F("Starting ICM-20948 I2C communication example"));

    Wire.begin();
    Wire.setClock(100000); // Use 400 kHz I2C speed

    myICM.begin(Wire, AD0_VAL);

    if (myICM.status != ICM_20948_Stat_Ok) {
        Serial.print(F("ICM-20948 initialization error: "));
        Serial.println(myICM.statusString());
        while (1); // Hold here to check the error
    }

    Serial.println(F("ICM-20948 initialization successful!"));
}

void ICMSensor::loop() {
    if (myICM.dataReady()) {
        myICM.getAGMT(); // Updates the sensor data: accel, gyro, mag, temp

        Serial.print(F("Gyro: "));
        Serial.print(myICM.gyrX());
        Serial.print(F(", "));
        Serial.print(myICM.gyrY());
        Serial.print(F(", "));
        Serial.println(myICM.gyrZ());

        delay(100); // Adjust as needed
    }
}