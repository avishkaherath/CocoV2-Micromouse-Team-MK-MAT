#include "display.h"

DISP_State disp_state;

void displayInit(void){
	ssd1306_Init();
	disp_state = INIT;
	displayUpdate();
}

void clearScreen()
{
	ssd1306_Fill(Black);
}

void writeString(char *str, FONT_Size font_size)
{
	switch (font_size)
	{
	case SMALL:
		ssd1306_WriteString(str, Font_6x8, White);
		break;
	case MEDIUM:
		ssd1306_WriteString(str, Font_7x10, White);
		break;
	case LARGE:
		ssd1306_WriteString(str, Font_11x18, White);
		break;
	case HUGE:
		ssd1306_WriteString(str, Font_16x24, White);
		break;
	}
}

void putString(char *str, int x, int y, FONT_Size font_size)
{
	ssd1306_SetCursor(x, y);
	writeString(str, font_size);
}

void putChar(char chr, int x, int y, FONT_Size font_size)
{
	char str[2];
	// Copy the character into the string
	str[0] = chr;
	// Null-terminate the string
	str[1] = '\0';
	ssd1306_SetCursor(x, y);
	writeString(str, font_size);
}

void putInt(int INT, int x, int y, FONT_Size font_size)
{
	char buff[64];
	snprintf(buff, sizeof(buff), "%d", INT);
	ssd1306_SetCursor(x, y);
	writeString(buff, font_size);
}

void putFloat(float FLOAT, int x, int y, FONT_Size font_size)
{
	char buff[64];
	snprintf(buff, sizeof(buff), "%.2f", FLOAT); // Change %.2f to adjust precision
	ssd1306_SetCursor(x, y);
	writeString(buff, font_size);
}


void displayUpdate(void)
{
	clearScreen();
	switch (disp_state)
	{
	// INITIALIZATION BLOCK: AUDI CAR EXPECTED
	case (INIT):
		putString("TEAM",20,8,LARGE);
		putString("MK-MAT",20,40,LARGE);
		break;

	// DEFAULT SCREEN --> BATTERY PERCENTAGE, STATE OF THE ROBOT
	case (DEFAULT):
		// DISPLAYING BATTERY VOLTAGE
//		putString("V:",86,24,SMALL);
//		putFloat(voltage,104 , 24, SMALL);

		// putString(turn,44,16,MEDIUM);
		switch (mouseState) {
		case (0):
			putString("RF:",2,5,MEDIUM);
			putInt(averageFR,26,5, MEDIUM);

			putString("LF:",70,5,MEDIUM);
			putInt(averageFL,94,5, MEDIUM);

			putString("AvgF:",22,20,MEDIUM);
			putInt((averageFL+averageFR)/2,60,20, MEDIUM);

			putString("DR:",2,35,MEDIUM);
			putInt(averageR,26,35, MEDIUM);

			putString("DL:",70,35,MEDIUM);
			putInt(averageL,94,35, MEDIUM);

			putString("RR:",2,50,MEDIUM);
			putInt(averageRR,26,50, MEDIUM);

			putString("LL:",70,50,MEDIUM);
			putInt(averageLL,94,50, MEDIUM);
			break;
		case (8):
			putString("Near AvgF :",30,15,MEDIUM);
			putFloat(nearVal,30,40,MEDIUM);break;
		case (9):
			putString("Far AvgF :",30,5,MEDIUM);
			putFloat(farVal,30,20, MEDIUM);

			putString("Threshold :",30,35,MEDIUM);
			putFloat(fr_thresh,30,50, MEDIUM);break;
		case (1):
			putString("SET INITIAL", 30, 28, MEDIUM);break;
		case (2):
			putString("SEARCH IDLE", 30, 28, MEDIUM);break;
		case (5):
			putString("FAST IDLE", 35, 28, MEDIUM);break;

//		case (2):
//			putString("SEARCH FWD", 42, 2, SMALL);break;
//		case (3):
//			putString("SEARCH BWD", 42, 2, SMALL);break;

//		case (5):
//			putString("FAST FWD", 42, 2, SMALL);break;
//		case (6):
//			putString("FAST BWD", 42, 2, SMALL);break;
//		case (9):
//			putString("Front Calib", 42, 2, SMALL);break;
//		case (7):
//			putString("Side Calib", 42, 2, SMALL);break;

//		case (10):
//			putString("Front Calib IDLE", 42, 2, SMALL);break;
		}
		

//		switch (runState) {
//		case (0):
//			putString("STARTING", 42, 12, SMALL);break;
//		case (1):
//			putString("DECISION", 42, 12, SMALL);break;
//		case (2):
//			putString("MV_CENTER", 42, 12, SMALL);break;
//		case (3):
//			putString("TURNING", 42, 12, SMALL);break;
//		case (4):
//			putString("MV_EDGE", 42, 12, SMALL);break;
//		}

//		putString("O:",2,24,SMALL);
//		putInt(ORIENT,20 , 24, SMALL);
//
//
//		putString("S:",30,24,SMALL);
//		putFloat(st_speed,48,24, SMALL);
		break;

//	case (GYRO_CALIB):
//		putString("NOISE: ",2,2,SMALL);
//		putFloat(noise,64, 2, SMALL);
//
//		putString("OFFSET: ",2,11,SMALL);
//		putInt(offset,64,11,SMALL);
//
//		putString("ANGLE: ",2,22,SMALL);
//		putFloat(angle_z,64,22,SMALL);
//		break;

//	case (SENSOR_READ):
//
//		putString("RF:",2,2,SMALL);
//		putFloat(averageFR,26,2, SMALL);
//
//		putString("LF:",76,2,SMALL);
//		putFloat(averageFL,100,2, SMALL);
//
//		putString("AvgF:",22,13,SMALL);
//		putFloat((averageFR+averageFL)/2,60,13, SMALL);
//
//		putString("DR:",2,35,SMALL);
//		putFloat(averageR,26,35, SMALL);
//
//		putString("DL:",76,35,SMALL);
//		putFloat(averageL,100,35, SMALL);
//
//		putString("RR:",2,46,SMALL);
//		putFloat(averageRR,26,46, SMALL);
//
//		putString("LL:",76,46,SMALL);
//		putFloat(averageLL,100,46, SMALL);
//
//		break;

//	case (LOW_BAT):
//		putString("BAT LOW...!",2,7,LARGE);
//		break;

	case (SUCESS_MSG):
		putString("HURRAYYYY!!!",2,2,LARGE);
		putString("MK MAT",2,22,SMALL);
		break;
	}
	ssd1306_UpdateScreen();
//	LED6_TOG;
	return;
}
