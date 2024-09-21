#ifndef IMU_H
#define IMU_H


void displaySensorOffsets(const adafruit_bno055_offsets_t &calibData);
void displayCalStatus(void);
void displaySensorDetails(void);
void displaySensorStatus(void);
#endif