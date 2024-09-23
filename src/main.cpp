#include <Arduino.h>
#include <Motor.h>

#include <IMU.h>

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

const int ENABLE_PIN = D2;
const int MOTOR_L_PIN_A = A0;
const int MOTOR_L_PIN_B = A1;
const int MOTOR_R_PIN_A = A2;
const int MOTOR_R_PIN_B = A3;

IMU bno = IMU();

Motor motor_R = Motor(MOTOR_R_PIN_A, MOTOR_R_PIN_B, 0.0);
Motor motor_L = Motor(MOTOR_L_PIN_A, MOTOR_L_PIN_B, 0.0);


void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);  // wait for serial port to open!
  
  /* Initialise the sensor */
  if (!bno.begin())
  {
      /* There was a problem detecting the BNO055 ... check your connections */
      Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
      while (1);
  }
  digitalWrite(ENABLE_PIN, HIGH);
  motor_L.setRotation(Motor::COUNTERCLOCKWISE);
  motor_R.setRotation(Motor::CLOCKWISE);
};

void loop() {
  Serial.println("Working");
  //motor_R.setPower(100.0);
  //motor_L.setPower(100.0);
  delay(1000);
  Serial.println("STOPED");
  //motor_R.setPower(0.0);
  //motor_L.setPower(0.0);
  delay(1000);
}

