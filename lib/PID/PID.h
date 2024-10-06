#ifndef PID_H
#define PID_H

#include <cmath>

class PIDController {
    public:
        PIDController(double Kp_, double Ki_, double Kd_, double a_);
        void updateInput(double newInput);
        void compute(double deltaTime);
        void setReference(double newRef);
        void setSaturation(double newSat);
        double getOutput(void);

        static float checkAngle(float angle);

    private:
        double Kp;
        double Ki;
        double Kd;
        double a;
        double Kr;
        double derivative;
        double integral;
        double saturation;
        double reference;
        double lastRef;
        double lastError;
        double input;
        double lastInput;
        double lastU;
        double u;
        double es;
};
#endif