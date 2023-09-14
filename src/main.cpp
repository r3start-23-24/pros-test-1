#include "main.h"
#include "gif-pros/gifclass.hpp"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include "robot.hpp"

void cata_thread() {
	// off bumper 28
	// on bumper 10
	while (true)
	{
		bool on = false;

		if (mainController.get_digital_new_press(DIGITAL_R1))
		{
			cata_motors.move_velocity(70);
			while (cata_limit_switch.get_value_calibrated() < 25)
			{
				pros::c::delay(2);
			}
			while (cata_limit_switch.get_value_calibrated() > 25)
			{
				pros::c::delay(2);
			}
			cata_motors.brake();
		}
		if (mainController.get_digital_new_press(DIGITAL_R2))
		{
			cata_motors.move_velocity(70);
			while (cata_limit_switch.get_value_calibrated() < 25)
			{
				pros::c::delay(2);
			}
			pros::c::delay(250);
			cata_motors.brake();
		}
		if (mainController.get_digital_new_press(DIGITAL_UP))
		{
			if (on)
			{
				cata_motors.move_velocity(0);
				cata_motors.brake();
				on = false;
			}
			else
			{
				cata_motors.move_velocity(70);
				on = true;
			}
		}
		pros::c::delay(2);
	}
}
void gifthread() {
	while (true)
	{
		Gif gif1("/usd/gif1.gif", lv_scr_act());
		pros::c::delay(10000);
		gif1.clean();
		Gif gif2("/usd/gif2.gif", lv_scr_act());
		pros::c::delay(10000);
		gif2.clean();
		Gif gif3("/usd/gif3.gif", lv_scr_act());
		pros::c::delay(10000);
		gif3.clean();
	}
}

void initialize() {
	cata_motors.set_brake_modes(MOTOR_BRAKE_HOLD);
	cata_limit_switch.calibrate();
	cata_motors.set_brake_modes(MOTOR_BRAKE_HOLD);
	intake_motors.set_brake_modes(MOTOR_BRAKE_BRAKE);

	pros::Task cata(cata_thread);
	pros::Task gifs(gifthread);
}

void disabled() {}

// pre-auton (eg auton selector)
void competition_initialize() {}

void autonomous() {}

void opcontrol() {
	bool intakeOn = false;
	bool intakeOnReversed = false;

	pros::c::delay(500);
	cata_motors.move(127);
	while (cata_limit_switch.get_value_calibrated() < 25)
	{
		pros::c::delay(2);
	}
	while (cata_limit_switch.get_value_calibrated() > 25)
	{
		pros::c::delay(2);
	}
	cata_motors.brake();

	while (true) {
		bool on = false;
    	int power = mainController.get_analog(ANALOG_LEFT_Y);
	    int turn = pow(2, (log(108)/(127*log(2))) * abs(mainController.get_analog(ANALOG_RIGHT_X))) + 19;
		if (mainController.get_analog(ANALOG_RIGHT_X) < 2 && mainController.get_analog(ANALOG_RIGHT_X) > -2)
		{
			turn = 0;
		}
		if (mainController.get_analog(ANALOG_RIGHT_X) < 0)
		{
			turn *= -1;
		}
	    int left = power + turn;
	    int right = power - turn;
	    left_drive_motors.move(left);
		right_drive_motors.move(right);

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
		
		printf("%d\n", cata_limit_switch.get_value());
	    pros::c::delay(5);
	}
}
