/*
 MAIN FILE CONTAINING ALL OUR CODES
 TEAM: SINDiB
 AUTHOR: BIYON
 */
#include "cppmain.h"

u32 i;
u32 DELAY_MID = 1;
bool buttonPress = false;
int mouseState = 0;
int runState = 0;
static coordinate XY;
static coordinate XY_prev;

int orient = 0;
int ORIENT = 0;

char direction;
bool starting = false;
float startingDist = 8.8;  //8.0
float edgeToCenter = 17;
float centerToEdgeSides = 2.1;
float centerToEdgeForward = 2.2;
float centerToEdgeBack = 2.5;
float Angle180 = 180;
float centerToEdge;

char back_path[ROWS * COLUMNS] = {'x'};
char fwd_path[ROWS * COLUMNS] = {'x'};
int ptr = 0;

static coordinate dumXY;	  // Declare a coordinate struct
static coordinate dumXY_prev; // Declare a coordinate struct
static int dumOrient;

int backPtr = 0;
int fwdPtr;
int16_t gyro_reading;
uint32_t l_pos;
uint32_t r_pos;
float omega;
uint16_t m_reciever,l_reciever,r_reciever,rf_reciever,lf_reciever,dl_reciever,dr_reciever;

void mouseRun();

int cppmain(void)
{

	initialization_block();

	if (orient == 1)
	{
		XY.x = 1;
		XY.y = 0;
		cells[0][0] = 10;
	}
	else
	{
		XY.x = 0;
		XY.y = 1;
		cells[0][0] = 9;
	}

	XY_prev.y = 0;
	XY_prev.x = 0;
	runState = 0;

	while(1)
	{
		mouseRun();
		HAL_Delay(1);
//
//		getSensorReadings();

//		align_select = 1;
//		if (finishMove(STRAIGHT_RUN, 19.2))
//		{
//			STOP_ROBOT;
//			HAL_Delay(1000);
//			runState ++;
//		}
//		if (runState == 3)
//		{
//			STOP_ROBOT;
//		break;
//		}
//		HAL_Delay(1);
//		if (finishMove(STRAIGHT_RUN, 10))
//		{
//			STOP_ROBOT;
//			break;
//		}
	}
}

int initialization_block(void)
{
	ALL_LED_ON;
	TIM5_START; ////////////////// CRUCIAL PART DON"T OFFFFFFFFFFFFFFFF ///////////////////////////
	HAL_Delay(100);

	motorInit();
	encoderInit();
//	gyroInit();

	displayInit();
	disp_state = DEFAULT;

	buzzerInit();
//	gyroCalibration();

	TIM13_IT_START;
//	TIM14_IT_START;
	HAL_Delay(100);
	ALL_LED_OFF;

	return 0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim13)

		gyroUpdate(),readSensor();
//	else if (htim == &htim14)
		//displayUpdate();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == TB_Pin)
	{
		buttonPress = true;
	}
	else
	{
		__NOP();
	}
}

void mouseRun()
{
	switch (mouseState)
	{

	case 0: // Idle
		LED7_ON;
		displayUpdate();
		if (buttonPress)
		{
			STOP_ROBOT;
			playSound(TONE4);
			LED7_OFF;
			HAL_Delay(500);
			mouseState = 8;
			buttonPress = false;
			l_start = 0;
		}
		break;

	case 1: // search Idle
		LED9_ON;
		displayUpdate();
		if (irBlink())
		{
			LED9_OFF;
			HAL_Delay(1000);
			mouseState = 2;

			if (ORIENT == 1)
			{
				XY.x = 1;
				XY.y = 0;
				cells[0][0] = 10;
			}
			else
			{
				XY.x = 0;
				XY.y = 1;
				cells[0][0] = 9;
			}

			XY_prev.y = 0;
			XY_prev.x = 0;

			orient = ORIENT;
		}

		if (buttonPress)
		{
			STOP_ROBOT;
			playSound(TONE4);
			HAL_Delay(500);
			mouseState = 4;
			buttonPress = false;
			runState = 0;
			l_start = 0;
		}
		break;

	case 2: // searchForward

		switch (runState)
		{

		case 0: // starting
			if (finishMove(STRAIGHT_RUN, startingDist))
			{
				STOP_ROBOT;
				HAL_Delay(DELAY_MID);
				runState = 1;
			}
			break;

		case 1: // decision
			getSensorReadings();
			updateWalls(XY, orient, L, R, F);

			if (flood[XY.y][XY.x] >= 1) // NotInCenter
			{
				floodFill(XY, XY_prev);
				direction = toMove(XY, XY_prev, orient);
				runState = 2;
				LED2_ON;
			}
			else
			{ // in center
				backtrack();

				dumXY.x = XY.x;
				dumXY.y = XY.y;
				dumXY_prev.x = XY_prev.x;
				dumXY_prev.y = XY_prev.y;
				dumOrient = orient;

				forwardtrack(dumXY, dumXY_prev, dumOrient);

				playSound(TONE2);

				mouseState = 3;
				runState = 1;
				backPtr = 0;
				LED5_ON;
			}
			HAL_Delay(DELAY_MID);
			break;

		case 2: // move center
			if (!F)
			{
				align_select = true;
			}

			if (finishMove(STRAIGHT_RUN, edgeToCenter))
			{
				STOP_ROBOT;
				LED2_OFF;
				HAL_Delay(DELAY_MID);
				runState = 5;
			}
			break;

		case 5: // front align
			if (F)
			{
				if (finishMove(FRONT_ALIGN, 16))
				{
					STOP_ROBOT;
					HAL_Delay(DELAY_MID);
					runState = 3;
				}
			}
			else
			{
				runState = 3;
			}

			break;

		case 6: // front distance adjust

			if (finishMove(FRONT_DIST, 16))
				{
					STOP_ROBOT;
					HAL_Delay(DELAY_MID);
					runState = 3;
				}
			break;

		case 3: // turning
			if (direction == 'L')
			{
				LED6_ON;
				if (finishMove(POINT_TURN, 90))
				{
					STOP_ROBOT;
					LED6_OFF;
					HAL_Delay(DELAY_MID);
					resetEncoder();
					orient = orientation(orient, direction);
					runState = 4;
				}
			}
			else if (direction == 'R')
			{
				LED11_ON;
				if (finishMove(POINT_TURN, -90))
				{
					STOP_ROBOT;
					LED11_OFF;
					HAL_Delay(DELAY_MID);
					resetEncoder();
//					playSound(TONE4);
					orient = orientation(orient, direction);
					runState = 4;
				}
			}
			else if (direction == 'B')
			{
				LED1_ON;
				if (finishMove(POINT_TURN, Angle180))
				{
					STOP_ROBOT;
					LED1_OFF;
					HAL_Delay(DELAY_MID);
					resetEncoder();
					orient = orientation(orient, direction);
					runState = 4;
				}
			}
			else if (direction == 'F')
			{
				HAL_Delay(DELAY_MID);
				runState = 4;
			}
			break;

		case 4: // move to edge
			if (direction == 'L' || direction == 'R')
			{
				centerToEdge = centerToEdgeSides;
			}
			else if (direction == 'B')
			{
				centerToEdge = centerToEdgeBack;
			}
			else if (direction == 'F')
			{
				centerToEdge = centerToEdgeForward;
			}
			if (finishMove(STRAIGHT_RUN, centerToEdge))
			{
				STOP_ROBOT;
				HAL_Delay(DELAY_MID);
				runState = 1;
				XY_prev = XY;
				XY = updateCoordinates(XY, orient);
			}
			break;
		}

		if (buttonPress)
		{
			STOP_ROBOT;
			playSound(TONE4);
			HAL_Delay(500);
			mouseState = 4;
			buttonPress = false;
			l_start = 0;
		}
		break;

	case 3: // search backward

		switch (runState)
		{

		case 0: // finishing
			if (finishMove(STRAIGHT_RUN, edgeToCenter))
			{
				STOP_ROBOT;
				HAL_Delay(DELAY_MID);
				mouseState = 0;
				ALL_LED_OFF;
			}
			break;

		case 1: // decision
			getSensorReadings();

			if (backFlood[XY.y][XY.x] == 0)
			{
				backPtr = 0;
				runState = 0;
			}
			else
			{
				direction = back_path[backPtr];
				backPtr += 1;
				runState = 2;
			}
			break;

		case 2: // move center
			if (!F)
			{
				align_select = true;
			}
			if (finishMove(STRAIGHT_RUN, edgeToCenter))
			{
				STOP_ROBOT;
				HAL_Delay(DELAY_MID);
				runState = 5;
			}
			break;

		case 5: // front align
			if (F)
			{
				if (finishMove(FRONT_ALIGN, 16))
				{
					STOP_ROBOT;
					HAL_Delay(DELAY_MID);
					runState = 3;
				}
			}
			else
			{
				runState = 3;
			}
			break;

		case 3: // turning
			if (direction == 'L')
			{
				if (finishMove(POINT_TURN, 90))
				{
					STOP_ROBOT;
					HAL_Delay(DELAY_MID);
					resetEncoder();
					orient = orientation(orient, direction);
					runState = 4;
				}
			}
			else if (direction == 'R')
			{
				if (finishMove(POINT_TURN, -90))
				{
					STOP_ROBOT;
					HAL_Delay(DELAY_MID);
					resetEncoder();
					orient = orientation(orient, direction);
					runState = 4;
				}
			}
			else if (direction == 'B')
			{

				if (finishMove(POINT_TURN, Angle180))
				{
					STOP_ROBOT;
					HAL_Delay(DELAY_MID);
					resetEncoder();
					orient = orientation(orient, direction);
					runState = 4;
				}
			}
			else if (direction == 'F')
			{
				HAL_Delay(DELAY_MID);
				runState = 4;
			}
			break;

		case 4: // move edge
			if (direction == 'L' || direction == 'R')
			{
				centerToEdge = centerToEdgeSides;
			}
			else if (direction == 'B')
			{
				centerToEdge = centerToEdgeBack;
			}
			else if (direction == 'F')
			{
				centerToEdge = centerToEdgeForward;
			}
			if (finishMove(STRAIGHT_RUN, centerToEdge))
			{
				STOP_ROBOT;
				HAL_Delay(DELAY_MID);
				runState = 1;
				XY_prev = XY;
				XY = updateCoordinates(XY, orient);
			}
			break;
		}
		if (buttonPress)
		{
			STOP_ROBOT;
			playSound(TONE4);
			HAL_Delay(500);
			mouseState = 4;
			buttonPress = false;
			l_start = 0;
		}

		break;

	case 4: // fast idle
		displayUpdate();
		LED10_ON;

		if (irBlink())
		{
			LED10_OFF;
			HAL_Delay(1000);
			mouseState = 5;

			if (ORIENT == 1)
			{
				XY.x = 1;
				XY.y = 0;
				cells[0][0] = 10;
			}
			else
			{
				XY.x = 0;
				XY.y = 1;
				cells[0][0] = 9;
			}

			XY_prev.y = 0;
			XY_prev.x = 0;
			fwdPtr = ptr;

			orient = ORIENT;
		}

		if (buttonPress)
		{
			STOP_ROBOT;
			playSound(TONE4);
			HAL_Delay(500);
			mouseState = 7;

			buttonPress = false;
			l_start = 0;
		}

		break;

	case 5: // fastForward
		switch (runState)
		{

		case 0: // statig
			if (finishMove(STRAIGHT_RUN, startingDist))
			{
//				STOP_ROBOT;
				HAL_Delay(DELAY_MID);
				runState = 1;
			}
			break;

		case 1: // decision
			getSensorReadings();

			if (flood[XY.y][XY.x] >= 1)
			{
				direction = fwd_path[fwdPtr];
				fwdPtr -= 1;
				runState = 2;
			}
			else
			{
				playSound(TONE2);
				fwdPtr = ptr;
				mouseState = 6;
				runState = 1;
				backPtr = 0;
			}
			break;

		case 2: // move center
			if (!F)
			{
				align_select = true;
			}
			if(direction =='F'){
						if (finishMove(STRAIGHT_RUN, 16))
						{


							HAL_Delay(DELAY_MID);
							runState = 5;
						}
			}
			else{
							if (finishMove(STRAIGHT_RUN, edgeToCenter))
						{
							
							STOP_ROBOT;
							

							HAL_Delay(DELAY_MID);
							runState = 5;
						}
			}

			break;

		case 5: // fron align
			if (F)
			{
				if (finishMove(FRONT_ALIGN, 16))
				{
					STOP_ROBOT;
					HAL_Delay(DELAY_MID);
					runState = 3;
				}
			}
			else
			{
				runState = 3;
			}
			break;

		case 3: // turning
			if (direction == 'L')
			{
				if (finishMove(POINT_TURN, 90))
				{
					STOP_ROBOT;
					HAL_Delay(DELAY_MID);
					resetEncoder();
					orient = orientation(orient, direction);
					runState = 4;
				}
			}
			else if (direction == 'R')
			{
				if (finishMove(POINT_TURN, -90))
				{
					STOP_ROBOT;
					HAL_Delay(DELAY_MID);
					resetEncoder();
					orient = orientation(orient, direction);
					runState = 4;
				}
			}
			else if (direction == 'B')
			{
				
				if (finishMove(POINT_TURN, Angle180))
				{
					STOP_ROBOT;
					HAL_Delay(DELAY_MID);
					resetEncoder();
					orient = orientation(orient, direction);
					runState = 4;
				}
			}
			else if (direction == 'F')
			{
				HAL_Delay(DELAY_MID);
				runState = 4;
			}
			break;

		case 4: // move edge
			if (direction == 'L' || direction == 'R')
			{
				centerToEdge = centerToEdgeSides;
			}
			else if (direction == 'B')
			{
				centerToEdge = centerToEdgeBack;
			}
			else if (direction == 'F')
			{
				centerToEdge = centerToEdgeForward;
			}
			if(direction == 'F'){

				runState = 1;
				XY_prev = XY;
				XY = updateCoordinates(XY, orient);

			}
			else{
				if (finishMove(STRAIGHT_RUN, centerToEdge))
				{
	//				STOP_ROBOT;
					HAL_Delay(DELAY_MID);
					runState = 1;
					XY_prev = XY;
					XY = updateCoordinates(XY, orient);
				}
			}

			break;
		}

		if (buttonPress)
		{
			STOP_ROBOT;
			playSound(TONE4);
			HAL_Delay(500);
			mouseState = 7;
			buttonPress = false;
			l_start = 0;
		}

		break;

	case 6: // fastBackward

		switch (runState)
		{

		case 0: // finishing
			if (finishMove(STRAIGHT_RUN, edgeToCenter))
			{
				STOP_ROBOT;
				HAL_Delay(DELAY_MID);
				mouseState = 0;
			}
			break;

		case 1: // decision
			getSensorReadings();

			if (backFlood[XY.y][XY.x] == 0)
			{
				backPtr = 0;
				runState = 0;
			}
			else
			{
				direction = back_path[backPtr];
				backPtr += 1;
				runState = 2;
			}
			break;

		case 2: // move center
			if (!F)
			{
				align_select = true;
			}
			if (finishMove(STRAIGHT_RUN, edgeToCenter))
			{
				STOP_ROBOT;
				HAL_Delay(DELAY_MID);
				runState = 5;
			}
			break;

		case 5: // fron align
			if (F)
			{
				if (finishMove(FRONT_ALIGN, 16))
				{
					STOP_ROBOT;
					HAL_Delay(DELAY_MID);
					runState = 3;
				}
			}
			else
			{
				runState = 3;
			}
			break;

		case 3: // turning
			if (direction == 'L')
			{
				if (finishMove(POINT_TURN, 90))
				{
					STOP_ROBOT;
					HAL_Delay(DELAY_MID);
					resetEncoder();
					orient = orientation(orient, direction);
					runState = 4;
				}
			}
			else if (direction == 'R')
			{
				if (finishMove(POINT_TURN, -90))
				{
					STOP_ROBOT;
					HAL_Delay(DELAY_MID);
					resetEncoder();
					orient = orientation(orient, direction);
					runState = 4;
				}
			}
			else if (direction == 'B')
			{
				if (finishMove(POINT_TURN, Angle180))
				{
					STOP_ROBOT;
					HAL_Delay(DELAY_MID);
					resetEncoder();
					orient = orientation(orient, direction);
					runState = 4;
				}
			}
			else if (direction == 'F')
			{
				HAL_Delay(DELAY_MID);
				runState = 4;
			}
			break;

		case 4: // move edge
			if (direction == 'L' || direction == 'R')
			{
				centerToEdge = centerToEdgeSides;
			}
			else if (direction == 'B')
			{
				centerToEdge = centerToEdgeBack;
			}
			else if (direction == 'F')
			{
				centerToEdge = centerToEdgeForward;
			}
			if (finishMove(STRAIGHT_RUN, centerToEdge))
			{
				STOP_ROBOT;
				HAL_Delay(DELAY_MID);
				runState = 1;
				XY_prev = XY;
				XY = updateCoordinates(XY, orient);
			}
			break;
		}

		if (buttonPress)
		{
			STOP_ROBOT;
			playSound(TONE4);
			HAL_Delay(500);
			mouseState = 7;
			buttonPress = false;
			l_start = 0;
		}
		break;

	case 7:
		//			mouseState = speedAdjust();
		displayUpdate();
//		if (rightIrBlink()){
//			playSound(TONE1);
//			st_speed += 0.1;
//			HAL_Delay(500);
//		}
//		if (leftIrBlink()){
//			playSound(TONE1);
//			st_speed -= 0.1;
//			HAL_Delay(500);
//		}
		if (irBlink())
		{
			STOP_ROBOT;
			playSound(TONE4);
			HAL_Delay(500);
			irSideCalibrate();
			mouseState = 10;
			buttonPress = false;
			l_start = 0;
		}


		if (buttonPress)
		{
			STOP_ROBOT;
			playSound(TONE4);
			HAL_Delay(500);
			mouseState = 0;
			buttonPress = false;
			l_start = 0;
		}
		break;

	case 10:
		displayUpdate();
		if (buttonPress)
		{
			STOP_ROBOT;
			playSound(TONE4);
			HAL_Delay(500);
			mouseState = 9;
			buttonPress = false;
			l_start = 0;
		}
		break;

	case 9:
		displayUpdate();
		if (irBlink())
		{
			STOP_ROBOT;
			playSound(TONE4);
			HAL_Delay(500);
			irFrontCalibrate();
			mouseState = 0;
			buttonPress = false;
			l_start = 0;
		}

		if (buttonPress)
		{
			STOP_ROBOT;
			playSound(TONE4);
			HAL_Delay(500);
			mouseState = 0;
			buttonPress = false;
			l_start = 0;
		}
		break;

	case 8: // set initial //////////////////////////////////////////////////////////// ASK FROM ISHRATH //////////////////////////////////////////////////////////
		LED8_ON;
		displayUpdate();
		if (irBlink())
		{
			HAL_Delay(1000);
			if (ORIENT == 1)
			{
				playSound(TONE1);
				ORIENT = 0;
			}
			else
			{
				playSound(TONE1);
				ORIENT = 1;
			}

			if (ORIENT == 1)
			{
				XY.x = 1;
				XY.y = 0;
				cells[0][0] = 10;
			}
			else
			{
				XY.x = 0;
				XY.y = 1;
				cells[0][0] = 9;
			}

			XY_prev.y = 0;
			XY_prev.x = 0;
		}

		if (buttonPress)
		{
			STOP_ROBOT;
			playSound(TONE4);
			LED8_OFF;
			HAL_Delay(500);
			mouseState = 1;
			buttonPress = false;
			l_start = 0;
		}

		break;

	default:
		break;
	}
}
