#ifndef motor_h
#define motor_h

class Motor {
  public:
    enum Rotation {CLOCKWISE, COUNTERCLOCKWISE};

    Motor(int pinAA, int pinBB, float deathZone);
    void setPower(float value);
    void setPower(void);
    void setRotation(Rotation nRotation);
    void invertRotation(void);
    void writePins(float values);
    void brake(void);

  private:
    float power;
    Rotation rotation;
    int pinA;
    int pinB;
    float deathzone;
};

#endif