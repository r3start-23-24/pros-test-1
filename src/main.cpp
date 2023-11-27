#include "main.h"
#include "gif-pros/gifclass.hpp"
#include "pros/adi.h"
#include "pros/misc.h"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include "robot.hpp"
#include "autoSelect/selection.h"

bool down_pressed = false;

const int oneTile = 1600;
void moveForward(float tiles, int velocity) {
	left_drive_motors.move_relative(tiles * oneTile, velocity);
	right_drive_motors.move_relative(tiles * oneTile, velocity);
	pros::c::delay(100);
	while (left_motor_1.get_actual_velocity() != 0)
	{
		pros::c::delay(5);
	}
}
void turnRight(float degrees, int velocity) {
	const int ninetyTurn = 80;
	const int oneDegree = 550/90;

	left_drive_motors.move_relative(oneDegree * degrees, velocity);
	right_drive_motors.move_relative(-(oneDegree * degrees), velocity);
	pros::c::delay(100);
	while (left_motor_1.get_actual_velocity() != 0)
	{
		pros::c::delay(5);
	}
}
void cata_down() {
	cata_motor.move(127);
	while (cata_limit_switch.get_value_calibrated() < 25 || down_pressed)
	{
		pros::c::delay(2);
	}
	while (cata_limit_switch.get_value_calibrated() > 25 || down_pressed)
	{
		pros::c::delay(2);
	}
	cata_motor.brake();
	down_pressed = false;
}


void cata_thread() {
	bool pressingR1 = false;
	// off bumper 28
	// on bumper 10
	while (true)
	{
		if (mainController.get_digital(DIGITAL_R1))
		{
			// cata shoot constantly
			cata_motor.move_velocity(100);
			pressingR1 = true;
		}
		else {
			if (pressingR1)
			{
				pressingR1 = false;
				while (cata_limit_switch.get_value_calibrated() < 25 || down_pressed)
				{
					pros::c::delay(2);
				}
				pros::c::delay(200);
				while (cata_limit_switch.get_value_calibrated() > 25 || down_pressed)
				{
					pros::c::delay(2);
				}
				cata_motor.brake();
				down_pressed = false;
			}
		}
		if (mainController.get_digital_new_press(DIGITAL_R2))
		{
			cata_down();
		}
		pros::c::delay(2);
	}
}
void up_button_thread() {
	bool on = false;
	while (true)
	{
		if (mainController.get_digital_new_press(DIGITAL_UP))
		{
			if (on)
			{
				cata_motor.move_velocity(0);
				cata_motor.brake();
				on = false;
			}
			else
			{
				cata_motor.move_velocity(100);
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
	selector::init();

	cata_motor.set_brake_mode(MOTOR_BRAKE_HOLD);
	cata_limit_switch.calibrate();
	intake_motor.set_brake_mode(MOTOR_BRAKE_BRAKE);

	pros::Task cata(cata_thread);
	//pros::Task cata_two(up_button_thread);
	//pros::Task gifs(gifthread);
}

void disabled() {
	
}

// pre-auton (eg auton selector)
void competition_initialize() {}

void autonomous() {
	blocker.set_value(true);
	if (selector::auton == 0) { // skills
		/*/ cata down
		pros::c::delay(100);
		cata_motor.move_velocity(70);
		while (cata_limit_switch.get_value_calibrated() > 25)
		{
			pros::c::delay(2);
		}
		pros::c::delay(250);
		cata_motor.brake();
		*/// end cata down

		// start of drift DO NOT TOUCH
		left_drive_motors.move_relative(1.5 * 1600, 250);
		right_drive_motors.move_relative(0.48 * 1600, 100);
		pros::c::delay(100);
		while (left_motor_1.get_actual_velocity() != 0)
		{
			pros::c::delay(5);
		}
		// end of drift DO NOT TOUCH

		moveForward(0.13, 400);
		// right drive forward for alignment
		right_drive_motors.move_relative(0.1 * 1600, 600);
		pros::c::delay(100);
		while (right_motor_1.get_actual_velocity() != 0)
		{
			pros::c::delay(5);
		}
		// end
		cata_motor.move_velocity(100); // speed of cata can be changed here
		pros::c::delay(35000); // time shooting can be changed here
		cata_motor.brake();
		// change from here down
		turnRight(-50, 200);
		blocker.set_value(false);
		moveForward(-1, 600);
		pros::c::delay(100);
		turnRight(20, 200);
		moveForward(-3, 600);
		turnRight(90, 200);
		right_wing.set_value(true);
		left_wing.set_value(true);
		moveForward(-1.2, 500);
		right_wing.set_value(false);
		left_wing.set_value(false);
		turnRight(95, 200);
		moveForward(-1.2, 600);
		turnRight(-90, 200);
		moveForward(-1, 600);
		right_wing.set_value(true);
		left_wing.set_value(true);
		turnRight(-100, 200);
		moveForward(-1.4, 600);
		moveForward(1.4, 600);
		moveForward(-1.4, 600);
	}
	else if (selector::auton == 1) { // red same goal
		// off 28
		// on 10
		//right_wing.set_value(true);
		/*cata_motor.move(127);
		while (cata_limit_switch.get_value_calibrated() > 25)
		{
			pros::c::delay(2);
		}
		cata_motor.move(-127);
		pros::c::delay(500);
		cata_motor.brake();
		*/// cata 1 rotation code end
		//pros::c::delay(1000); // instead of cata thing
		//right_wing.set_value(false);
		intake_motor.move_velocity(600);
		moveForward(1, 500);
		pros::c::delay(150);
		moveForward(-1.5, 500);
		left_wing.set_value(true);
		turnRight(-45, 300);
		moveForward(-0.3, 400);
		turnRight(-45, 300);
		left_wing.set_value(false);
		turnRight(40, 350);
		moveForward(-0.75, 450);
		turnRight(-40, 350);
		// aligned
		moveForward(-0.6, 600);
		intake_motor.move(0);
		// pushed balls in
		moveForward(0.2, 400);
		moveForward(-0.3, 600);
		// pushed x2
		moveForward(0.2, 450);
		turnRight(90, 400);
		moveForward(-0.5, 500);
		moveForward(1.4, 500);
		turnRight(100, 350);
		moveForward(0.7, 450);
		turnRight(90, 350);
		intake_motor.move_velocity(-600);
		pros::c::delay(200);
		moveForward(1.4, 600);
		moveForward(-0.5, 500);
		moveForward(0.8, 600);
		/*
		turnRight(30, 300);
		moveForward(0.5, 350);
		turnRight(90, 300);
		intake_motor.move(-127);
		moveForward(0.2, 400);
		turnRight(-120, 300);
		intake_motor.move(127);
		moveForward(1.5, 300);
		*/
	}
	else if (selector::auton == 2) { // red other goal
		// at opposite goal (AWP)
		// off 28 - on 10
		/*cata_motor.move(127);
		while (cata_limit_switch.get_value_calibrated() > 25)
		{
			pros::c::delay(2);
		}
		cata_motor.move(-127);
		pros::c::delay(500);
		cata_motor.brake();
		*/// cata 1 rotation code end
		right_wing.set_value(true);
		intake_motor.move(127);
		moveForward(-0.4, 400);
		pros::c::delay(100);
		right_wing.set_value(false);
		intake_motor.move(0);
		turnRight(-90, 300);
		moveForward(1, 300);
		turnRight(-45, 300);
		moveForward(0.8, 400);
		// turn towards goal
		turnRight(-100, 300);
		intake_motor.move(-127);
		moveForward(1, 400);
		// pushed in
		moveForward(-0.5, 300);
		moveForward(0.5, 500);
		// pushed in x2
		moveForward(-0.5, 300);
		right_wing.set_value(true);
		turnRight(65, 300);
		moveForward(-1.5, 300);
		// just move left side of drive
		left_drive_motors.move_relative(0.15 * oneTile, 100);
		pros::c::delay(100);
		while (left_motor_1.get_actual_velocity() != 0)
		{
			pros::c::delay(5);
		}
		// end
	}
	else if (selector::auton == -2) { // red other goal
		// at opposite goal but no AWP (USELESS)
		cata_motor.move_relative(50, 100);
		intake_motor.move_velocity(600);
		moveForward(0.1, 100);
		pros::c::delay(50);
		moveForward(-2, 450);
		turnRight(-45, 300);
		right_wing.set_value(true);
		moveForward(-1, 300);
		intake_motor.move_velocity(-600);
		turnRight(-45, 300);
		right_wing.set_value(false);
		moveForward(0.25, 400);
		turnRight(180, 350);
		left_wing.set_value(true);
		moveForward(-0.25, 400);
		left_wing.set_value(false);
		moveForward(0.5, 300);
		turnRight(105, 300);
		intake_motor.move_velocity(600);
		moveForward(2, 400);
		moveForward(-0.5, 300);
		turnRight(-100, 300);
		right_wing.set_value(true);
		moveForward(-1.25, 400);
		turnRight(20, 300);
		moveForward(-1, 450);
		moveForward(0.3, 300);
		turnRight(180, 400);
		intake_motor.move_velocity(-600);
		moveForward(-0.5, 300);
		moveForward(0.6, 300);
		//end of auton path
	}
}

void opcontrol() {
	pros::Task gifs(gifthread);

	bool left_wing_extended = false;
	bool right_wing_extended = false;
	bool blocker_out = false;

	pros::c::delay(500);
	//cata_down();

	while (true) {
		// drive
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
		// end drive

    	if (mainController.get_digital_new_press(DIGITAL_DOWN))
		{
			cata_motor.move_relative(-100, 100);
			down_pressed = true;
		}

		if (mainController.get_digital_new_press(DIGITAL_X))
		{
			if (blocker_out)
			{
				blocker.set_value(false);
				blocker_out = false;
			}
			else
			{
				blocker.set_value(true);
				blocker_out = true;
			}
		}

		if (mainController.get_digital(DIGITAL_L1))
		{
			intake_motor.move(127);
		}
		else if (mainController.get_digital(DIGITAL_L2))
		{
			intake_motor.move(-127);
		}
		else
		{
			intake_motor.move(0);
		}

		if (mainController.get_digital_new_press(DIGITAL_LEFT))
		{
			if (left_wing_extended)
			{
				left_wing.set_value(false);
				left_wing_extended = false;
			}
			else
			{
				left_wing.set_value(true);
				left_wing_extended = true;
			}
		}
		else if (mainController.get_digital_new_press(DIGITAL_RIGHT))
		{
			if (right_wing_extended)
			{
				right_wing.set_value(false);
				right_wing_extended = false;
			}
			else
			{
				right_wing.set_value(true);
				right_wing_extended = true;
			}
		}

		mainController.clear();
		mainController.print(0,0,"%f", cata_motor.get_temperature());
		pros::c::delay(5);
	}
}
