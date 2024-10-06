#ifndef motor_h
#define motor_h

typedef enum {CLOCKWISE, COUNTERCLOCKWISE} Rotations;

class Motor {
  public:
    Motor(int pinAA, int pinBB, float deathZone);
    void setPower(float value);
    void setPower(void);
    void setRotation(Rotations nRotation);
    void invertRotation(void);
    void writePins(float values);
    void brake(void);
    float getPower(void);
    Rotations getRotation(void);

  private:
    float power;
    Rotations rotation;
    int pinA;
    int pinB;
    float deathzone;
};

#endif