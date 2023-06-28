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

	while (true) {
    	int power = master.get_analog(ANALOG_LEFT_Y);
	    int turn = master.get_analog(ANALOG_RIGHT_X);
	    int left = power + turn;
	    int right = power - turn;
	    right *= -1;

	    left_motor_1.move(left);
	    left_motor_2.move(left);
		right_motor_1.move(right);
		right_motor_2.move(right);

	    delay(2);
	}
}
