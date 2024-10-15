#include <PID.h>

PIDController::PIDController(double Kp_, double Ki_, double Kd_, double a_){
    Kp = Kp_;    
    Ki = Ki_;
    Kd = Kd_;
    a = a_;

    Kr = sqrt(Ki*Kd);
    checkAngleEnabled = false;
    reference = NAN;
    saturation = 255;
    lastInput = 0;
    integral = 0;
    lastError = 0;
    lastRef = 0;
    lastU = 0;
    es = 0;
};

void PIDController::setCheckAngle(bool newState){
    checkAngleEnabled = newState;
};

void PIDController::updateInput(double newInput){
    if(checkAngleEnabled){
        input = checkAngle(newInput);
    } else {
        input = newInput;
    }
};

void PIDController::setReference(double newRef){
    reference = newRef;
};

void PIDController::setSaturation(double newSat){
    saturation = newSat;
};

double PIDController::getOutput(){
    return u;
};

double PIDController::getLastError(){
    return lastError;
};

void PIDController::compute(double deltaTime){
    if(reference == NAN){
        return;
    }
    double i_control;
    double derivative;
    double error;
    if(checkAngleEnabled){
        error = checkAngle(reference - input);
    } else {
        error = reference - input;
    }
    //derivative = (error - lastError) / deltaTime; NORMAL DERIVATIVE
    derivative = (1 - a) * ((reference - a * lastRef) - (input - a * lastInput)) / deltaTime;


    //Anti-Windup
    if(lastU > saturation){
        es = saturation - lastU;
        integral += (Ki * error + Kr * es) * deltaTime;
        i_control = integral;
    } else {
        integral += error * deltaTime;
        i_control = Ki * integral;
    }

    u = Kp * derivative + i_control;

    //LIMITAR LA TENSION
    if(u > saturation){
        u = saturation;
    } else if (u < -saturation) {  //CUIDADO QUE LA CLASE DE MOTORES ES DIFERENTE
        u = -saturation;
    }

    //ACTUALIZAR LOS VALORES PREVIOS
    lastError= error;
    lastInput = input;
    lastRef = reference;
    lastU = u;
};

float PIDController::checkAngle(float angle) {
    float new_angle;
    if (angle > 180){
        new_angle = -(360 - angle);
    } else {
        new_angle = angle;
    }
    return new_angle;
};

