#include "main.h"
#include "gif-pros/gifclass.hpp"

void initialize() {}

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
	pros::Controller mainController (pros::E_CONTROLLER_MASTER);

	pros::Motor left_motor_1 (2, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
	pros::Motor left_motor_2 (3, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
	pros::Motor right_motor_1 (4, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
	pros::Motor right_motor_2 (5, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
	pros::Motor_Group left_drive_motors ({left_motor_1, left_motor_2});
	pros::Motor_Group right_drive_motors ({right_motor_1, right_motor_2});

	pros::Motor cata_motor_1 (6, pros::E_MOTOR_GEARSET_36, true, pros::E_MOTOR_ENCODER_DEGREES);
	pros::Motor cata_motor_2 (7, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES);
	pros::Motor_Group cata_motors ({cata_motor_1, cata_motor_2});

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

		printf("%f", left_motor_1.get_actual_velocity());
		
		Gif gif("/usd/mygif.gif", lv_scr_act());

	    pros::c::delay(2);
	}
}
