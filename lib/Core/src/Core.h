#ifndef CORE_H
#define CORE_H

#include <Arduino.h>
#include <Motor.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define BNO055_SAMPLERATE_DELAY_MS (100)
typedef enum {FORWARD, BACKWARD} Directions;

typedef enum {
    ENABLE_PIN = D2,
    DRIVER_STATUS_PIN = D3,
    MOTOR_L_PIN_A = A0,
    MOTOR_L_PIN_B = A1,
    MOTOR_R_PIN_A = A2,
    MOTOR_R_PIN_B = A3
} Pins;

class Core {
    public:
        Core();
        void enableMotors(void);
        void disableMotors(void);
        void initIMU(void);
        void stop(void);
        void rotate(float angles);
        void rotate(Rotations rotation);
        void move(Directions direction, float power_value);
        void changeDirection(void);

        int8_t getTemperature(void);
        double getHeading(void);

    private:
        Directions lastDirection;
        double lastHeading;
        int8_t lastTemperature;
        bool areMotorsEnabled;
        Adafruit_BNO055 bno;
        Motor motor_R;
        Motor motor_L;
};

#endif