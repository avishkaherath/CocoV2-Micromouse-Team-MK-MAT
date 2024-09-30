#include "PD.h"

// VARIABLES
u32 l_start = 0;
static u32 r_start = 0, previous_time = 0, current_time = 0, start_time = 0, last_exit_time = 0; // STORE STARTING POSITION
static float start_angle = 0;

static MV_Type mv_type = IDLE;
static float dist_ang;
float MIDDLE_VALUE_DL = 3310;  //FOR DIAGONAL 1050
float fl_offset = 3800;
float fr_offset = 3800;

static float PD_correction_sc = 0, PD_correction_ac = 0, PD_correction_ir = 0;
static float sc_last_error = 0, ac_last_error = 0, ir_last_error = 0;

/////////////////////////////////////////////////// CONTROLLER /////////////////////////////////////////////////////////////////
static int fm_counter = 0;
const float TERMINATION_TH = 2e-1;
bool align_select = false;

void frontAlignController(void);

bool finishMove(MV_Type mv_type_, float dist_ang_)
{
	mv_type = mv_type_, dist_ang = dist_ang_;
	current_time = HAL_GetTick();

	// FIRST CALL
	if (l_start == 0)
	{
		resetEncoder();
		l_start = l_position, r_start = r_position, start_angle = angle_z;
		(mv_type == STRAIGHT_RUN) ? LED1_ON : ((mv_type == POINT_TURN) ? LED2_ON : LED3_ON);
		sc_last_error = 0, ac_last_error = 0, fm_counter = 0;
		previous_time = current_time;
		start_time = current_time, last_exit_time = 0;
		assignParameters();
		return false;
	}

	// CORRECTIONS
	PD_correction_ac = 0, PD_correction_ir = 0;
	speedController();

	//	 TERMINATION CODITION
	if (fabs(PD_correction_sc) < fabs(TERMINATION_TH) || (current_time - start_time > 2000) ||( (current_time - last_exit_time) > 100 && last_exit_time != 0))
	{
		if (fm_counter > 5)
		{
			l_start = 0, r_start = 0;
			fm_counter = 0;
			(mv_type == STRAIGHT_RUN) ? LED1_OFF : ((mv_type == POINT_TURN) ? LED2_OFF : LED3_OFF);
			return true;
		}
		fm_counter++;
		HAL_Delay(20);
	}

	// SET WHEEL
	switch (mv_type)
	{
	case STRAIGHT_RUN:
		l_speed = PD_correction_sc - PD_correction_ac + PD_correction_ir;
		r_speed = PD_correction_sc + PD_correction_ac - PD_correction_ir;
		break;
	case POINT_TURN:
		l_speed = -PD_correction_sc;// + PD_correction_ac;
		r_speed = +PD_correction_sc;// - PD_correction_ac;
		break;
	case FRONT_ALIGN:
		l_speed = -PD_correction_sc - PD_correction_ac;
		r_speed = -PD_correction_sc + PD_correction_ac;
		break;
//	case FRONT_DIST:
//		l_speed = -PD_correction_sc ;
//		r_speed = -PD_correction_sc ;
//		break;
	}
	setWheels();
	previous_time = current_time;
	align_select = false;
	return false;
}

////////////////////////////////////////////////// PARAMETER CHOICE ///////////////////////////////////////////////////////////////
//  PARAMETERS
static float sc_kp = 0, sc_kd = 0, sc_red = 1e3;
static float ac_kp = 0, ac_kd = 0, ac_red = 1e3;
static float ir_kp = 0, ir_kd = 0, ir_red = 1e3;
static float counts_ = 0; // CONVERTING ANGLE/ DISTANCE TO ENCODER COUNTS
static float speed_th_ = 0;

void assignParameters(void)
{
	switch (mv_type)
	{
	case IDLE:
		sc_red = 1e5, ac_red = 1e5;
		break;

	case STRAIGHT_RUN:
		speed_th_ = st_speed;
		counts_ = dist_ang * LINEAR_SENSITIVITY;

		if (fabs(st_speed - 0.5) < .05)
		{
			sc_kp = 1, sc_kd = 5e-3, sc_red = 200;
			ac_kp = 1.1, ac_kd = 8e-2, ac_red = 100;
			ir_kp = 1, ir_kd = 3e-2, ir_red = 5e2;
		}
		else if (fabs(st_speed - 0.8) < .05)
		{
			sc_kp = 1.0, sc_kd = 50e-3, sc_red = 400;
			ac_kp = 1.3, ac_kd = 50e-3, ac_red = 500;	//1e-3
			ir_kp = 2, ir_kd = 100e-3, ir_red = 1000;   //1e-3
		}
		else
		{
			sc_kp = 1.2, sc_kd = 2e-3, sc_red = 450;
			ac_kp = 1.3, ac_kd = 1e-3, ac_red = 600;
			ir_kp = 1, ir_kd = 1e-3, ir_red = 2500;
		}
		break;

	case POINT_TURN:
		counts_ = dist_ang * TURN_SENSITIVITY;
		speed_th_ = rt_speed;

		sc_kp = 1.0, sc_kd = 65e-3, sc_red = 60; // 2e-3, 1e-3
		ac_kp = 1, ac_kd = 65e-3, ac_red = 1000; // 1e-3
		break;

	case FRONT_ALIGN:
		speed_th_ = al_speed;
		sc_kp = 10, sc_kd = 300e-3, sc_red = 400;
		ac_kp = 50, ac_kd = 300e-3, ac_red = 400;
		break;

//	case FRONT_DIST:
//		speed_th_ = al_speed;
//		sc_kp = 10, sc_kd = 300e-3, sc_red = 400;
//		break;
	}
	return;
}
/////////////////////////////////////////////////// SPEED CONTROLLER /////////////////////////////////////////////////////////////
static float sc_error = 0;

void speedController(void)
{
	switch (mv_type)
	{
	case IDLE:
		sc_error = 0;
		break;

	case STRAIGHT_RUN:
		sc_error = l_start + r_start + 2 * counts_, sc_error -= l_position, sc_error -= r_position; // sc_error = (l_start + r_start + 2*counts_) - l_position - r_position
		break;

	case POINT_TURN:
		// ENCODER BASED TURN :
//		sc_error  = (counts_ - (l_start - l_position)) + (counts_ + (r_start - r_position));
		sc_error = 2*counts_ - l_start + r_start, sc_error += l_position, sc_error -= r_position; // BACK UP : ENCODER BASED TURN
		// GYRO BASED TURN
//		sc_error = (start_angle + dist_ang) - angle_z;
		break;

	case FRONT_ALIGN:
		sc_error = (LFSensor - fl_offset) + (RFSensor - fr_offset);
		break;

//	case FRONT_DIST:
//		sc_error = (LFSensor - fl_offset) + (RFSensor - fr_offset);
//		break;
	}

	PD_correction_sc = (float)(sc_kp * sc_error + sc_kd * 1e3 * (sc_error - sc_last_error) / (current_time - previous_time)) / sc_red;

	if (fabs(sc_error-sc_last_error)>1e-3)
	{
		last_exit_time = current_time;
	}

	sc_last_error = sc_error;

	if (fabs(PD_correction_sc) > speed_th_)
	{
		PD_correction_sc = (PD_correction_sc > 0) ? speed_th_ : -speed_th_;

		if (irController() && align_select)
		{
//			LED2_ON;
			PD_correction_ac = 0;
			irController();
		}
		else
		{
//			LED2_ON;
			PD_correction_ir = 0;
			angularController();
		}
		return;
	}
	// PD_correction_ac = 0;
}

////////////////////////////////////////// ANGULAR CONTROLLER //////////////////////////////////////////////////////////
static float ac_error = 0;

void angularController(void)
{
	switch (mv_type)
	{
	// STRAIGHT RUN
	case (STRAIGHT_RUN):
		ac_error = l_position, ac_error -= r_position;
		break;

	// POINT-ROTATION
	case (POINT_TURN):
		ac_error = r_position, ac_error += l_position, ac_error -= (l_start + r_start), ac_error = (float)ac_error; // a_error = (l_position - l_start) + (r_position - r_start)
		break;

	case (FRONT_ALIGN):
		ac_error = (LFSensor - RFSensor);
		break;

	default:
		return;
	}

	PD_correction_ac = (ac_kp * ac_error + ac_kd * 1e3 * (ac_error - ac_last_error) / (current_time - previous_time)) / ac_red;
	ac_last_error = ac_error;
	if (fabs(PD_correction_ac) > .5 * speed_th_)
		PD_correction_ac = (PD_correction_ac > 0) ? .5 * speed_th_ : -.5 * speed_th_;
	return;
}

void frontAlignController(void)
{
	ac_error = (LFSensor - RFSensor);
	PD_correction_ac = (ac_kp * ac_error + ac_kd * 1e3 * (ac_error - ac_last_error) / (current_time - previous_time)) / ac_red;
	ac_last_error = ac_error;
	if (fabs(PD_correction_ac) > speed_th_)
		PD_correction_ac = (PD_correction_ac > 0) ? speed_th_ : -1* speed_th_;

	return;
}

///////////////////////////////////////////////////////  IR-CONTROLLER /////////////////////////////////////////////////////////////////////////////////
static float ir_error = 0;


bool twoWalls(void)
{
	if (((averageLL - 2100) * (averageLL - 4500) < 0) && ((averageRR - 1900) * (averageRR - 4500) < 0))  //900  //2300
		return true;
	return false;
}

bool leftWall(void)
{
	if ((averageLL - 2100) * (averageLL - 4500) < 0) //900  //2300
		return true;
	return false;
}

bool rightWall(void)
{
	if ((averageRR - 1900) * (averageRR - 4500) < 0) //900  //2300
		return true;
	return false;
}
static bool a = false;
// DL SENSOR ALIGNMENT
bool irController(void)
{
	calculateAndSaveAverages();
	a = leftWall();
	if ((mv_type == STRAIGHT_RUN))
	{
		if (twoWalls())
			ir_error = averageLL - averageRR;
		else if (leftWall())
			ir_error = 2 * (averageLL - MIDDLE_VALUE_DL);
		else if (rightWall())
			ir_error = 2 * (MIDDLE_VALUE_DL - averageRR);
		else
		{
			ir_error = 0;
			return false;
		}
		PD_correction_ir = (ir_kp * ir_error + ir_kd * 1e3 * (ir_error - ir_last_error) / (current_time - previous_time)) / ir_red;
		ir_last_error = ir_error;
		if (fabs(PD_correction_ir) > speed_th_ * .3)
			PD_correction_ir = (PD_correction_ir > 0) ? .3 * speed_th_ : -.3 * speed_th_;
		return true;
	}
	PD_correction_ir = 0;
	return false;
}
