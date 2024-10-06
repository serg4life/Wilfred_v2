#ifndef PID_H
#define PID_H

#include <cmath>

class PID_Controller {
    public:
        PID_Controller(float Kp_par, float Ki_par, float Kd_par, float a_par, float ref_par, float start_meassure, float sat_par, bool filter_par);
        float update(float meassure, float dt);
        static float checkAngle(float angle);

    private:
        float Kp;
        float Ki;
        float Kd;
        float a;
        float b;    //Ponderation
        float c;    //Ponderation
        float sat;
        float Kr;
        float derivative;
        float integral;
        float saturation;
        bool filter;
        float ref;
        float ref_prev;
        float error_prev;
        float meassure_prev;
        float u_prev;
        float es;
        PID_Filter filterObject;

};

class PID_Filter {
    public:
        PID_Filter(float fc, float T);
        float getValue(float data);

    private:
        float v0_prev;
        float vi_prev;
        float a;

};
#endif