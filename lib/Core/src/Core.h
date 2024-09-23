#ifndef CORE_H
#define CORE_H

#include <Motor.h>
#include <IMU.h>

typedef enum {FORWARD, BACKWARD} Directions;

typedef enum {
    ENABLE_PIN = D2,
    MOTOR_L_PIN_A = A0,
    MOTOR_L_PIN_B = A1,
    MOTOR_R_PIN_A = A2,
    MOTOR_R_PIN_B = A3
} Pins;

class Core {
    public:
        Core();
        void enableMotors(void);
        void stop(void);
        void rotate(float angles);
        void move(Directions direction = FORWARD);

        float getTemperature(void);
        float getHeading(void);

    private:
        float lastHeading;
        float lastTemperature;
        bool areMotorsEnabled;
        IMU imu;
        Motor motor_R;
        Motor motor_L;
};

#endif