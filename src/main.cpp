#include "main.h"

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
 * following initialize().
 */
void opcontrol() {
	pros::Controller mainController (pros::E_CONTROLLER_MASTER);

	pros::Motor left_motor_1 (2);
	pros::Motor left_motor_2 (3);
	pros::Motor right_motor_1 (4);
	pros::Motor right_motor_2 (5);
	pros::Motor_Group left_drive_motors ({left_motor_1, left_motor_2});
	pros::Motor_Group right_drive_motors ({right_motor_1, right_motor_2});

	while (true) {
    	int power = mainController.get_analog(ANALOG_LEFT_Y);
	    int turn = mainController.get_analog(ANALOG_RIGHT_X);
	    int left = power + turn;
	    int right = power - turn;
	    right *= -1;

	    left_drive_motors.move(left);
		right_drive_motors.move(right);

		printf("%d", left_motor_1.get_actual_velocity());
	    pros::c::delay(2);
	}
}
