#include "read_sensor.h"

int reflectionRate = REFLECTION_RATE_;

int32_t LFSensor = 0;
int32_t RFSensor = 0;
int32_t LSensor = 0;
int32_t RSensor = 0;

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


void readSensor(void)
{
    
}