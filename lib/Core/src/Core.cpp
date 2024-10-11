#include <Core.h>

void displaySensorOffsets(const adafruit_bno055_offsets_t &calibData)
{
    Serial.print("Accelerometer: ");
    Serial.print(calibData.accel_offset_x); Serial.print(" ");
    Serial.print(calibData.accel_offset_y); Serial.print(" ");
    Serial.print(calibData.accel_offset_z); Serial.print(" ");

    Serial.print("\nGyro: ");
    Serial.print(calibData.gyro_offset_x); Serial.print(" ");
    Serial.print(calibData.gyro_offset_y); Serial.print(" ");
    Serial.print(calibData.gyro_offset_z); Serial.print(" ");

    Serial.print("\nMag: ");
    Serial.print(calibData.mag_offset_x); Serial.print(" ");
    Serial.print(calibData.mag_offset_y); Serial.print(" ");
    Serial.print(calibData.mag_offset_z); Serial.print(" ");

    Serial.print("\nAccel Radius: ");
    Serial.print(calibData.accel_radius);

    Serial.print("\nMag Radius: ");
    Serial.print(calibData.mag_radius);
}

Core::Core() : 
    motor_R(MOTOR_R_PIN_A, MOTOR_R_PIN_B, 0.0),         //Initialization list, when there are objects inside other objects contructors.
    motor_L(MOTOR_L_PIN_A, MOTOR_L_PIN_B, 0.0),
    bno(55, 0x28)
{
    pinMode(ENABLE_PIN, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(ENABLE_PIN, LOW);  
    motor_R.setRotation(CLOCKWISE);
    motor_L.setRotation(COUNTERCLOCKWISE);
    lastDirection = FORWARD;
    areMotorsEnabled_var = false;
    lastHeading = 0;
    lastTemperature = 0;
};

void Core::initIMU(void){
    /* Initialise the sensor */
    if (!bno.begin())
    {
        /* There was a problem detecting the BNO055 ... check your connections */
        Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while (1);
    }
    bno.setAxisRemap(Adafruit_BNO055::REMAP_CONFIG_P1);     //remap, X points forward
    bno.setMode(OPERATION_MODE_NDOF);
    bno.setExtCrystalUse(true);
    bno.enterSuspendMode();
};

void Core::wakeIMU(void){
    bno.enterNormalMode();
};

void Core::sleepIMU(void){
    bno.enterSuspendMode();
};

void Core::calibrateIMU(void){
    sensors_event_t event;
    uint8_t system, gyro, accel, mag = 0;
    while(!(system == 3 && gyro == 3 && mag == 3)){
        bno.getEvent(&event);
        /* Get the four calibration values (0..3) */
        /* Any sensor data reporting 0 should be ignored, */
        /* 3 means 'fully calibrated" */
        bno.getCalibration(&system, &gyro, &accel, &mag);

        /* Display the individual values */
        Serial.print("Sys:");
        Serial.print(system, DEC);
        Serial.print(" G:");
        Serial.print(gyro, DEC);
        Serial.print(" A:");
        Serial.print(accel, DEC);
        Serial.print(" M:");
        Serial.println(mag, DEC);

        delay(BNO055_SAMPLERATE_DELAY_MS);
    }
    Serial.println("\nFully calibrated!");
    Serial.println("--------------------------------");
    Serial.println("Calibration Results: ");
    adafruit_bno055_offsets_t newCalib;
    bno.getSensorOffsets(newCalib);
    displaySensorOffsets(newCalib);
    bno.setSensorOffsets(newCalib);
};

bool Core::areMotorsEnabled(void){
    return areMotorsEnabled_var;
};

void Core::enableMotors(void){
    stop();
    areMotorsEnabled_var = true;
    digitalWrite(ENABLE_PIN, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
};

void Core::disableMotors(void){
    areMotorsEnabled_var = false;
    stop();
    digitalWrite(ENABLE_PIN, LOW);
    digitalWrite(LED_BUILTIN, LOW);
};

void Core::stop(void){
    motor_L.setPower(0);
    motor_R.setPower(0);
};

void Core::rotate(float angles){
};

void Core::rotate(Rotations rotation, float value){
    switch (rotation)
    {
    case CLOCKWISE:
        motor_L.setRotation(COUNTERCLOCKWISE);
        motor_R.setRotation(COUNTERCLOCKWISE);
        break;
    case COUNTERCLOCKWISE:
        motor_L.setRotation(CLOCKWISE);
        motor_R.setRotation(CLOCKWISE);
    default:
        break;
    }
    motor_L.setPower(value);
    motor_R.setPower(value);
};

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
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    lastHeading = euler.x();
    return lastHeading;
};

int8_t Core::getTemperature(void){
    lastTemperature = bno.getTemp();
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
