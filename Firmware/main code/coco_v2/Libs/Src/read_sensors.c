#include "read_sensors.h"

int reflectionRate = REFLECTION_RATE_;
const float LOW_BAT_TH = LOW_BAT_TH_;

int32_t volMeter=0;
float voltage = 0;
int32_t LFSensor = 0;
int32_t RFSensor = 0;
int32_t DLSensor = 0;
int32_t DRSensor = 0;
int32_t LLSensor = 0;
int32_t RRSensor = 0;

int32_t LFSensor_bg = 0;
int32_t RFSensor_bg = 0;
int32_t DLSensor_bg = 0;
int32_t DRSensor_bg = 0;
int32_t LLSensor_bg = 0;
int32_t RRSensor_bg = 0;

// dc offset due to the reflections from robot itself
int32_t LFSensor_DC = 2000;
int32_t RFSensor_DC = 1330;
int32_t DLSensor_DC = 1560;
int32_t DRSensor_DC = 745;
int32_t LLSensor_DC = 0;
int32_t RRSensor_DC = 0;

int32_t count=0;

static int32_t LBuff[15] = {0};
static int32_t RBuff[15] = {0};
static int32_t FLBuff[15] = {0};
static int32_t FRBuff[15] = {0};

float averageL = 0;
float averageR = 0;
float averageFL = 0;
float averageFR = 0;

bool L = false;
bool R = false;
bool F = false;

int start_count = 0;
int elapse_count = 60;

static int point = 0;

/*read IR sensors*/
void readSensor(void)
{
	LED7_ON;

	//read ambient values
	LFSensor_bg = read_LF_Sensor;
	RFSensor_bg = read_RF_Sensor;
	DLSensor_bg = read_DL_Sensor;
	DRSensor_bg = read_DR_Sensor;
	
	__HAL_TIM_SET_COUNTER(&htim5,0);

    //left front sensor
	LF_EM_ON;
//	start_count = __HAL_TIM_GET_COUNTER(&htim5);
	while(__HAL_TIM_GET_COUNTER(&htim5)< 60); //start_count + elapse_count);
	LFSensor = read_LF_Sensor - LFSensor_bg - LFSensor_DC;
	LF_EM_OFF;
//	start_count = __HAL_TIM_GET_COUNTER(&htim5);
	if(LFSensor < 0)//error check
		LFSensor = 0;
	while(__HAL_TIM_GET_COUNTER(&htim5)<140); //start_count + elapse_count);

    //right side sensor
//	R_EM_ON;
//	start_count = __HAL_TIM_GET_COUNTER(&htim5);
//	while(__HAL_TIM_GET_COUNTER(&htim5)<start_count + elapse_count);
//	RRSensor = read_RR_Sensor - RRSensor_bg - RRSensor_DC;
//	R_EM_OFF;
//	start_count = __HAL_TIM_GET_COUNTER(&htim5);
//	if(RRSensor < 0)//error check
//		RRSensor = 0;
//	while(__HAL_TIM_GET_COUNTER(&htim5)<start_count + elapse_count);
//
//    //Left side sensor
//	L_EM_ON;
//	start_count = __HAL_TIM_GET_COUNTER(&htim5);
//	while(__HAL_TIM_GET_COUNTER(&htim5)<start_count + elapse_count);
//	LLSensor = read_LL_Sensor - LLSensor_bg - LLSensor_DC;
//	L_EM_OFF;
//	start_count = __HAL_TIM_GET_COUNTER(&htim5);
//	if(LLSensor < 0)//error check
//		LLSensor = 0;
//	while(__HAL_TIM_GET_COUNTER(&htim5)<start_count + elapse_count);

	//right front sensor
	RF_EM_ON;
//	start_count = __HAL_TIM_GET_COUNTER(&htim5);
	while(__HAL_TIM_GET_COUNTER(&htim5)<200); //start_count + elapse_count);
	RFSensor = read_RF_Sensor - RFSensor_bg - RFSensor_DC;
	RF_EM_OFF;
//	start_count = __HAL_TIM_GET_COUNTER(&htim5);
	if(RFSensor < 0)
		RFSensor = 0;
	while(__HAL_TIM_GET_COUNTER(&htim5)<280); //start_count + elapse_count);


    //diagonal sensors
	D_EM_ON;
//	start_count = __HAL_TIM_GET_COUNTER(&htim5);
	while(__HAL_TIM_GET_COUNTER(&htim5)<340); //start_count + elapse_count);
	count = __HAL_TIM_GET_COUNTER(&htim5);
	DLSensor = read_DL_Sensor - DLSensor_bg - DLSensor_DC;
	DRSensor = read_DR_Sensor - DRSensor_bg - DRSensor_DC;
	D_EM_OFF;
	if(DLSensor < 0)
		DLSensor = 0;
	if(DRSensor < 0)
		DRSensor = 0;
	// while(__HAL_TIM_GET_COUNTER(&htim1)<500);


	readVolMeter();
//
//	LFSensor = LFSensor*reflectionRate/1000;
//	RFSensor = RFSensor*reflectionRate/1000;
//	DLSensor = DLSensor*reflectionRate/1000;
//	DRSensor = DRSensor*reflectionRate/1000;

	point++;
	if (point>=15){
		point = 0;
	}


	LBuff[point] = DLSensor;
	RBuff[point] = DRSensor;
	FLBuff[point] = LFSensor;
	FRBuff[point] = RFSensor;
	
	LED7_OFF;
}




/*read voltage meter*/
void readVolMeter(void)
{          //3240 = 7.85V
	volMeter = read_Vol_Meter;//raw value
	voltage = (volMeter*ADC_REF_VOL/4095)* 2.8;//actual voltage value  ex) 8.2V = 8200
	// if (voltage<LOW_BAT_TH)
	// 	stop_it_all();
}

void stop_it_all(void){
	disp_state=LOW_BAT;
	displayUpdate();
	OFF_BUZZ;
	STOP_ROBOT;
	ALL_LED_OFF;
	TIM13_IT_STOP;
	return;
}




bool irBlink() {
	static uint32_t t3 = 1000;
	if (LFSensor > t3 || RFSensor > t3) {
			return true;
		}
	return false;
}

bool rightIrBlink(){
	static uint32_t t2 = 3000;
	if (DRSensor > t2) {
			return true;
		}
	return false;
}

bool leftIrBlink(){
	static uint32_t t1 = 3000;
	if (DLSensor > t1) {
			return true;
		}
	return false;
}

void calculateAndSaveAverages() {
    int i;
    // Calculate the average for each buffer
    for (i = 0; i < 15; i++) {
        averageL += LBuff[i];
        averageR += RBuff[i];
        averageFL += FLBuff[i];
        averageFR += FRBuff[i];
    }

    // Divide the sums by 15 to get the average
    averageL = averageL/15;
    averageR = averageR/15;
    averageFL = averageFL/15;
    averageFR = averageFR/15;
}

void getSensorReadings() {

	calculateAndSaveAverages();

	static float t1 = 500.0;
	static float t2 = 150.0;


	if (averageR > t1){
		R = true;
		LED10_ON;
	} else {
		R = false;
		LED10_OFF;
	}

	if (averageL > t1){
		LED9_ON;
		L = true;
	} else {
		L = false;
		LED9_OFF;
	}

	if ((averageFL+averageFR)/2 > t2){
		LED11_ON;
		F = true;
	} else {
		F = false;
		LED11_OFF;
	}

	// if (DLSensor > t1 && DRSensor > t1 && DRSensor > t1 && RFSensor > t1){
	// 	F = true;
	// 	R = true;
	// 	L = true;
	// } else if (DLSensor > t1 && DRSensor > t1){
	// 	F = false;
	// 	R = true;
	// 	L = true;
	// } else if (LFSensor > t1 && DLSensor > t1){
	// 	F = true;
	// 	R = false;
	// 	L = true;
	// } else if (RFSensor > t1 && DRSensor > t1){
	// 	F = true;
	// 	R = true;
	// 	L = false;
	// } else if (DLSensor > t1){
	// 	F = false;
	// 	R = false;
	// 	L = true;
	// } else if (DRSensor > t1){
	// 	F = false;
	// 	R = true;
	// 	L = false;
	// } else if (RFSensor > t2 || LFSensor > t2){
	// 	F = true;
	// 	R = false;
	// 	L = false;
	// } else {
	// 	F = false;
	// 	R = false;
	// 	L = false;
	// }
}


