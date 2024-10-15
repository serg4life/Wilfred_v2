#ifndef STATUS_RGB_H
#define STATUS_RGB_H

#include <Arduino.h>

class StatusRGB {
    public:
        StatusRGB(void);
        void hueToRGB(uint8_t hue, uint8_t brightness_);
        void hueToRGB(uint8_t hue);
        void off(void);
        void on(void);
        void blynk(uint8_t times, int interval_millis);
        uint8_t brightness;  // 255 is maximum brightness, but can be changed.  Might need 256 for common anode to fully turn off.

    private:
        uint8_t ledR = LEDR;
        uint8_t ledG = LEDG;
        uint8_t ledB = LEDB;
        uint32_t R, G, B;           // the Red Green and Blue color components
};

#endif