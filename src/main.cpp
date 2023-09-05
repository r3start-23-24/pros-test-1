#include "main.h"
#include "gif-pros/gifclass.hpp"
//#include "lvgl.h"

void initialize() {
	//LV_IMG_DECLARE(DillyPic);
}

// for when robot is disabled
void disabled() {}

// pre-auton (eg auton selector)
void competition_initialize() {}

/**
 * This function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 */
void autonomous() {}

/**
 * If no competition control is connected, this function will run immediately
 * following--ignore-fail-on-non-empty initialize().
 */
void opcontrol() {
	bool intakeOn = false;
	bool intakeOnReversed = false;

	pros::Controller mainController (pros::E_CONTROLLER_MASTER);

	pros::Motor left_motor_1 (2, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
	pros::Motor left_motor_2 (3, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
	pros::Motor right_motor_1 (8, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
	pros::Motor right_motor_2 (9, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
	pros::Motor_Group left_drive_motors ({left_motor_1, left_motor_2});
	pros::Motor_Group right_drive_motors ({right_motor_1, right_motor_2});

	pros::Motor cata_motor_1 (5, pros::E_MOTOR_GEARSET_36, true, pros::E_MOTOR_ENCODER_DEGREES);
	pros::Motor cata_motor_2 (6, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES);
	pros::Motor_Group cata_motors ({cata_motor_1, cata_motor_2});

	pros::Motor intake_motor_1 (4, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
	pros::Motor intake_motor_2 (7, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
	pros::Motor_Group intake_motors ({intake_motor_1, intake_motor_2});
	intake_motors.set_brake_modes(MOTOR_BRAKE_BRAKE);

	cata_motors.move_relative(120, 100);

	while (true) {
    	int power = mainController.get_analog(ANALOG_LEFT_Y);
	    int turn = mainController.get_analog(ANALOG_RIGHT_X);
	    int left = power + turn;
	    int right = power - turn;
	    left_drive_motors.move(left);
		right_drive_motors.move(right);

		if (mainController.get_digital_new_press(DIGITAL_R1))
		{
			cata_motors.move_relative(360, 100);
		}

		if (mainController.get_digital_new_press(DIGITAL_X))
		{
			if (intakeOn)
			{
				intake_motors.move(0);
				intakeOn = false;
				intakeOnReversed = false;
			}
			else
			{
				intake_motors.move(127);
				intakeOn = true;
				intakeOnReversed = false;
			}
		}
		else if (mainController.get_digital_new_press(DIGITAL_Y))
		{
			if (intakeOnReversed)
			{
				intake_motors.move(0);
				intakeOn = false;
				intakeOnReversed = false;
			}
			else
			{
				intake_motors.move(-127);
				intakeOn = false;
				intakeOnReversed = true;
			}
		}

		if (mainController.get_digital(DIGITAL_L1))
		{
			intake_motors.move(127);
		}
		else if (mainController.get_digital(DIGITAL_L2))
		{
			intake_motors.move(-127);
		}
		else if (!(intakeOn || intakeOnReversed))
		{
			intake_motors.move(0);
			//intakeOn = false;
			//intakeOnReversed = false;
		}
		
		//Gif gif("/usd/mygif.gif", lv_scr_act());

	    pros::c::delay(2);
	}
}
