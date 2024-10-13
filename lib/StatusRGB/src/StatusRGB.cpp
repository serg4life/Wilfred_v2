#include <StatusRGB.h>


StatusRGB::StatusRGB(void){
    //uint8_t ledArray[3] = {1, 2, 3}; // three led channels
    ledcAttachPin(ledR, 1); // assign RGB led pins to channels
    ledcAttachPin(ledG, 2);
    ledcAttachPin(ledB, 3);

    // Initialize channels 
    // channels 0-15, resolution 1-16 bits, freq limits depend on resolution
    // ledcSetup(uint8_t channel, uint32_t freq, uint8_t resolution_bits);
    ledcSetup(1, 12000, 8); // 12 kHz PWM, 8-bit resolution
    ledcSetup(2, 12000, 8);
    ledcSetup(3, 12000, 8);
};

void StatusRGB::hueToRGB(uint8_t hue){
    hueToRGB(hue, brightness);
};

void StatusRGB::on(void){
    ledcWrite(1, R); // write red component to channel 1, etc.
    ledcWrite(2, G);   
    ledcWrite(3, B);
}

void StatusRGB::off(void){
    ledcWrite(1, 255); // write red component to channel 1, etc.
    ledcWrite(2, 255);   
    ledcWrite(3, 255);
};

void StatusRGB::blynk(uint8_t times, int interval_millis){
    for(int i=0; i<times; i++){
        on();
        delay(interval_millis);
        off();
        delay(interval_millis);
    };
};

void StatusRGB::hueToRGB(uint8_t hue, uint8_t brightness_){
    brightness = brightness_;
    uint16_t scaledHue = (hue * 6);
    uint8_t segment = scaledHue / 256; // segment 0 to 5 around the
                                            // color wheel
    uint16_t segmentOffset =
      scaledHue - (segment * 256); // position within the segment

    uint8_t complement = 0;
    uint16_t prev = (brightness * ( 255 -  segmentOffset)) / 256;
    uint16_t next = (brightness *  segmentOffset) / 256;

    //Common anode
    brightness = 255 - brightness;
    complement = 255;
    prev = 255 - prev;
    next = 255 - next;

    switch(segment ) {
    case 0:      // red
        R = brightness;
        G = next;
        B = complement;
    break;
    case 1:     // yellow
        R = prev;
        G = brightness;
        B = complement;
    break;
    case 2:     // green
        R = complement;
        G = brightness;
        B = next;
    break;
    case 3:    // cyan
        R = complement;
        G = prev;
        B = brightness;
    break;
    case 4:    // blue
        R = next;
        G = complement;
        B = brightness;
    break;
   case 5:      // magenta
    default:
        R = brightness;
        G = complement;
        B = prev;
    break;
    }
};