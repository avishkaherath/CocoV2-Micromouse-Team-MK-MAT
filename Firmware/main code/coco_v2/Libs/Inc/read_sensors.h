#ifndef INC_READ_SENSORS_H_
#define INC_READ_SENSORS_H_


#include "main.h"
#include "led.h"
#include "typedefs.h"
#include "adc.h"
#include "motor.h"
#include "buzzer.h"
#include "parameters_.h"
#include "display.h"
#include "PD.h"

#define ADC_REF_VOL 3.322
#define IR_BUFFER_LENGTH ((u32)1000u)  // 1000u

extern int reflectionRate;

extern float voltage;
extern int32_t LFSensor;
extern int32_t RFSensor;
extern int32_t DLSensor;
extern int32_t DRSensor;
extern int32_t RRSensor;
extern int32_t LLSensor;

extern int32_t LFSensor_bg;
extern int32_t RFSensor_bg;
extern int32_t DLSensor_bg;
extern int32_t DRSensor_bg;
extern int32_t RRSensor_bg;
extern int32_t LLSensor_bg;
extern int32_t count;
extern float fr_thresh;

extern bool L;
extern bool R;
extern bool F;

extern float averageLL;
extern float averageRR;
extern float averageFL;
extern float averageFR;
extern float averageL;
extern float averageR;


void readSensor(void);
void readVolMeter(void);

bool irBlink();
bool rightIrBlink();
bool leftIrBlink();
void getSensorReadings();
void stop_it_all(void);
void calculateAndSaveAverages();
float frontWallCalibrate(void);
//void irFrontCalibrate(void);
//void irSideCalibrate(void);

#endif /* INC_READ_SENSORS_H_ */
