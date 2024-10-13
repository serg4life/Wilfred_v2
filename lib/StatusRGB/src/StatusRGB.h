#ifndef STATUS_RGB_H
#define STATUS_RGB_H

#include <Arduino.h>

class statusRGB {
    public:
        statusRGB(void);
        void hueToRGB(uint8_t hue);

        uint8_t color;          // a value from 0 to 255 representing the hue
        uint32_t R, G, B;           // the Red Green and Blue color components
        uint8_t brightness;  // 255 is maximum brightness, but can be changed.  Might need 256 for common anode to fully turn off.

    private:
        uint8_t ledR;
        uint8_t ledG;
        uint8_t ledB;
};

#endif