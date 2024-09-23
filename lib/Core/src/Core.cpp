#include <Core.h>

Core::Core() : 
    motor_R(MOTOR_R_PIN_A, MOTOR_R_PIN_B, 0.0),         //Initialization list, when there are objects inside other objects contructors.
    motor_L(MOTOR_L_PIN_A, MOTOR_L_PIN_B, 0.0)
{
    pinMode(ENABLE_PIN, OUTPUT);
    digitalWrite(ENABLE_PIN, LOW);  //NO funciona
    motor_R.setRotation(CLOCKWISE);
    motor_L.setRotation(COUNTERCLOCKWISE);
    imu = IMU();
    imu.begin();
    imu.setMode(OPERATION_MODE_NDOF);
    imu.setAxisRemap(Adafruit_BNO055::REMAP_CONFIG_P1);     //remap, X points forward
    //imu.enterSuspendMode();
    lastDirection = FORWARD;
    areMotorsEnabled = false;
    lastHeading = 0;
    lastTemperature = 0;
};

void Core::enableMotors(void){
    areMotorsEnabled = true;
    digitalWrite(ENABLE_PIN, HIGH);
};

void Core::disableMotors(void){
    areMotorsEnabled = false;
    digitalWrite(ENABLE_PIN, LOW);
};

void Core::stop(void){
    motor_L.setPower(0);
    motor_R.setPower(0);
};

void Core::rotate(float angles){
};

void Core::rotate(Rotations rotation){};

void Core::changeDirection(void){
    Directions tempDirection;
    motor_L.invertRotation();
    motor_R.invertRotation();
    switch (lastDirection)
    {
        case BACKWARD:
            tempDirection = FORWARD;
            break;

        case FORWARD:
            tempDirection = BACKWARD;
        default:
            break;
    }
    lastDirection = tempDirection;
};

double Core::getHeading(void){
    imu::Vector<3> euler = imu.getVector(Adafruit_BNO055::VECTOR_EULER);
    lastHeading = euler.x();
    return lastHeading;
};

int8_t Core::getTemperature(void){
    lastTemperature = imu.getTemp();
    return lastTemperature;
};

void Core::move(Directions direction, float power_value){
    switch (direction)
    {
        case FORWARD:
            motor_L.setRotation(COUNTERCLOCKWISE);
            motor_R.setRotation(CLOCKWISE);
            break;
        case BACKWARD:
            motor_R.setRotation(COUNTERCLOCKWISE);
            motor_L.setRotation(CLOCKWISE);
            break;
        default:
            break;
    }
    lastDirection = direction;
    motor_L.setPower(power_value);
    motor_R.setPower(power_value);
};
