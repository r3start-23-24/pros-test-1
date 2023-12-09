#include "main.h"
#include "gif-pros/gifclass.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include "robot.hpp"
#include "autoSelect/selection.h"

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

const int allowance = 500; // +/- 5 degrees
const int cata_down_pos = 18950;
const int cata_mid_pos = 27500;
const int cata_up_pos = 10000;

void cata_down() {
	cata_motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	cata_motor.move(127);
	while (!(cata_rotation_sensor.get_angle() > cata_down_pos-allowance && cata_rotation_sensor.get_angle() < cata_down_pos+allowance))
	{
		pros::c::delay(1);
		printf("%d\n", cata_rotation_sensor.get_angle());                                                                                                                                                                                                                                                      
	}
	cata_motor.move(0);
	cata_motor.brake();
	pros::c::delay(50);
	cata_motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
}
void cata_mid() {
	cata_motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	cata_motor.move(127);
	while (!(cata_rotation_sensor.get_angle() > cata_mid_pos-allowance && cata_rotation_sensor.get_angle() < cata_mid_pos+allowance))
	{
		pros::c::delay(1);
		printf("%d\n", cata_rotation_sensor.get_angle());                                                                                                                                                                                                                                                      
	}
	cata_motor.move(0);
	cata_motor.brake();
	pros::c::delay(50);
	cata_motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
}
void cata_up() {
	cata_motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	cata_motor.move(127);
	while (!(cata_rotation_sensor.get_angle() > cata_up_pos-allowance && cata_rotation_sensor.get_angle() < cata_up_pos+allowance))
	{
		pros::c::delay(1);
		printf("%d\n", cata_rotation_sensor.get_angle());                                                                                                                                                                                                                                                      
	}
	cata_motor.move(0);
	cata_motor.brake();
	pros::c::delay(50);
	cata_motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
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
		else if (pressingR1)
		{
				pressingR1 = false;
				cata_down();
		}
		if (mainController.get_digital_new_press(DIGITAL_R2))
		{
			cata_mid();
		}
		if (mainController.get_digital_new_press(DIGITAL_B))
		{
			cata_up();
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

	cata_motor.set_brake_mode(MOTOR_BRAKE_COAST);
	cata_limit_switch.calibrate();
	intake_motor.set_brake_mode(MOTOR_BRAKE_BRAKE);
	left_drive_motors.set_brake_modes(MOTOR_BRAKE_COAST);
	right_drive_motors.set_brake_modes(MOTOR_BRAKE_COAST);

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
		cata_motor.move_velocity(80); // speed of cata can be changed here
		pros::c::delay(35000); // time shooting can be changed here
		cata_motor.brake();
		// change from here down
		turnRight(-60, 200);
		blocker.set_value(false);
		moveForward(-1, 600);
		pros::c::delay(100);
		turnRight(20, 200);
		moveForward(-3, 600);
		turnRight(-90, 200);
		right_wing.set_value(true);
		left_wing.set_value(true);
		moveForward(1.2, 500);
		turnRight(-95, 200);
		moveForward(1.2, 600);
		right_wing.set_value(false);
		left_wing.set_value(false);
		turnRight(90, 200);
		moveForward(1, 600);
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

		// start
		cata_mid();
		moveForward(1.8, 500);
		turnRight(-100, 300);
		intake_motor.move_velocity(-600);
		moveForward(0.35, 250);
		// triball pushed in
		pros::c::delay(300); // for consistency
		moveForward(-0.35, 400);
		// arc start
		left_drive_motors.move_relative(-0.2 * oneTile, 100);
		right_drive_motors.move_relative(-0.8 * oneTile, 400);
		pros::c::delay(100);
		while (right_motor_1.get_actual_velocity() != 0)
		{
			pros::c::delay(5);
		}
		// arc end
		intake_motor.move_velocity(600);
		moveForward(0.35, 550);
		// picked up ball
		moveForward(-0.3, 550);
		turnRight(100, 400);
		intake_motor.move_velocity(-600);
		pros::c::delay(250);
		moveForward(0.05, 550);
		// ball 1 pushed over
		// arc start
		left_drive_motors.move_relative(-0.4 * oneTile, 450);
		right_drive_motors.move_relative(-0.1 * oneTile, 150);
		pros::c::delay(100);
		while (left_motor_1.get_actual_velocity() != 0)
		{
			pros::c::delay(5);
		}
		// arc end
		intake_motor.move_velocity(600);
		moveForward(0.6, 550);
		moveForward(-0.4, 550);
		turnRight(45, 400);
		intake_motor.move_velocity(-600);
		pros::c::delay(250);
		moveForward(0.2, 550);
		// ball 2 pushed over
		moveForward(-0.5, 300);
		turnRight(120, 350);
		moveForward(1, 500);
		moveForward(0.5, 100); // continue but SLOW
		right_wing.set_value(true);
		turnRight(-145, 500);
		moveForward(0.5, 400);
		right_wing.set_value(false);
		moveForward(1, 400);
		// done
	}
	else if (selector::auton == -2) { // red other goal
		// AWP CODE
		// start
		moveForward(-1.5, 600);
		moveForward(0.5, 200);
		right_wing.set_value(true);
		moveForward(0.5, 200);
		turnRight(-83, 450);
		right_wing.set_value(false);
		intake_motor.move_velocity(-600);
		left_wing.set_value(true);
		moveForward(0.3, 200);
		turnRight(20, 200);
		moveForward(0.5, 200);
		turnRight(-20, 200);
		moveForward(0.83, 300);

	}
}

void opcontrol() {
	blocker.set_value(true);
	//pros::Task gifs(gifthread);

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

		if (mainController.get_digital(DIGITAL_L1))
		{
			intake_motor.move(-127);
		}
		else if (mainController.get_digital(DIGITAL_L2))
		{
			intake_motor.move(127);
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

		pros::c::delay(5);
	}
}
