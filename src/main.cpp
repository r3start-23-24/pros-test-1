#include "main.h"
#include "gif-pros/gifclass.hpp"
#include "robot.hpp"
//#include "lvgl.h"

void initialize() {
	cata_motor.set_brake_mode(MOTOR_BRAKE_HOLD);
	cata_limit_switch.calibrate();
	cata_motor.set_brake_mode(MOTOR_BRAKE_HOLD);
	intake_motors.set_brake_modes(MOTOR_BRAKE_BRAKE);
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

	cata_motor.move(127);
	while (cata_limit_switch.get_value_calibrated() < 25)
	{
		pros::c::delay(2);
	}
	while (cata_limit_switch.get_value_calibrated() > 25)
	{
		pros::c::delay(2);
	}
	cata_motor.brake();

	while (true) {
		bool on = false;
    	int power = mainController.get_analog(ANALOG_LEFT_Y);
	    int turn = mainController.get_analog(ANALOG_RIGHT_X);
	    int left = power + turn;
	    int right = power - turn;
	    left_drive_motors.move(left);
		right_drive_motors.move(right);

		if (mainController.get_digital_new_press(DIGITAL_R1))
		{
			// off bumper 28
			// on bumper 10
			cata_motor.move(127);
			while (cata_limit_switch.get_value_calibrated() < 25)
			{
				pros::c::delay(2);
			}
			while (cata_limit_switch.get_value_calibrated() > 25)
			{
				pros::c::delay(2);
			}
			cata_motor.brake();
		}
		if (mainController.get_digital_new_press(DIGITAL_R2))
		{
			cata_motor.move(127);
			while (cata_limit_switch.get_value_calibrated() < 25)
			{
				pros::c::delay(2);
			}
			pros::c::delay(250);
			cata_motor.brake();
		}
		if (mainController.get_digital_new_press(DIGITAL_UP))
		{
			if (on)
			{
				cata_motor.brake();
				on = false;
			}
			else
			{
				cata_motor.move_velocity(70);
				on = true;
			}
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
		}
		
		//Gif gif("/usd/mygif.gif", lv_scr_act());
		printf("%d\n", cata_limit_switch.get_value());
	    pros::c::delay(5);
	}
}
