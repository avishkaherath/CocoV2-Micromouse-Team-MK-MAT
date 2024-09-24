#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_


#include "main.h"
#include "typedefs.h"


#define _ENCODER_START (u32)u32_max/2  //1e4
#define l_position TIM1 -> CNT
#define r_position TIM4 -> CNT


void encoderInit(void);
void encoderUpdate(void);
void resetEncoder(void);


#endif /* INC_ENCODER_H_ */
