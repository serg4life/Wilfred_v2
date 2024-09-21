#ifndef IMU_H
#define IMU_H

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define BNO055_SAMPLERATE_DELAY_MS (100)

class IMU : public Adafruit_BNO055{   
    public:
        using Adafruit_BNO055::Adafruit_BNO055;

    private:

};

#endif