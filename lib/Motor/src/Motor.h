#ifndef motor_h
#define motor_h

typedef enum {CLOCKWISE, COUNTERCLOCKWISE} Rotations;

class Motor {
  public:
    Motor(int pinAA, int pinBB, float deathZone);
    void setPower(uint32_t value);
    void setPower(void);
    void setRotation(Rotations nRotation);
    void invertRotation(void);
    void writePins(uint32_t values);
    void brake(void);
    uint32_t getPower(void);
    Rotations getRotation(void);

  private:
    uint32_t power;
    Rotations rotation;
    int pinA;
    int pinB;
    float deathzone;
};

#endif