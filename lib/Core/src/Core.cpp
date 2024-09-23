#include <Core.h>

Core::Core(){
    motor_R = Motor(MOTOR_R_PIN_A, MOTOR_R_PIN_B, 0.0);
    motor_L = Motor(MOTOR_L_PIN_A, MOTOR_L_PIN_B, 0.0);
    imu = IMU(55, 0x28);
    areMotorsEnabled = false;
    lastHeading = 0;
    lastTemperature = 0;
};