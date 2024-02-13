#include "main.h"
#include "gif-pros/gifclass.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include "robot.hpp"
#include "autoSelect/selection.h"
#include "lemlib/api.hpp"

bool front_left_wing_extended = false;
bool front_right_wing_extended = false;
bool back_left_wing_extended = false;
bool back_right_wing_extended = false;

// start move blocker funcs
const int allowance = 500; // 5 degrees
const int down_pos = 166;
const int blocker_up_pos = -102000;
const int hang_up_pos = 47000;

void blocker_move(float pos) {
	cata_motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	if (cata_rotation_sensor.get_position() - pos > 0);
    // actually do this lmao
    // needs to determine direction to move motor
	cata_motor.move(127);
	while (!(cata_rotation_sensor.get_position() > pos-allowance && cata_rotation_sensor.get_position() < pos+allowance))
	{
		pros::c::delay(5);
        printf("%d\n", cata_rotation_sensor.get_position());
	}
	cata_motor.brake();
	pros::c::delay(50);
	cata_motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
}
// end move blocker funcs

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

const int one_lemlib_tile = 24;
int lemlib_x;
int lemlib_y;
void move_lemlib(float tiles, int velocity = 50) {
    lemlib_y += one_lemlib_tile*tiles;
    lemlib_chassis.moveTo(lemlib_x, lemlib_y, 1000*tiles, velocity);
}
void turn_lemlib(int alpha, int velocity = 50) {
    int theta = lemlib_chassis.getPose().theta;
    lemlib_x -= sin(theta + alpha);
    lemlib_y += sin(theta + alpha);
    lemlib_chassis.turnTo(lemlib_x, lemlib_y, 1000);
}

void initialize() {
	lemlib_chassis.calibrate();
	selector::init();

	cata_rotation_sensor.set_data_rate(5);
    // default is 10 (ms)

	cata_motor.set_brake_mode(MOTOR_BRAKE_COAST);
	intake_motor.set_brake_mode(MOTOR_BRAKE_BRAKE);
	left_drive_motors.set_brake_modes(MOTOR_BRAKE_COAST);
	right_drive_motors.set_brake_modes(MOTOR_BRAKE_COAST);
}

void disabled() {}

// pre-auton (eg auton selector)
void competition_initialize() {}

void autonomous() {
	lemlib_chassis.setPose(0,0,0);
	lemlib_chassis.moveTo(0, 36, 2000, 50);
	lemlib_chassis.turnTo(100, 36, 1000);
	lemlib_chassis.moveTo(72, 36, 3000, 60);
	lemlib_chassis.turnTo(72, 0, 3000, true);
	lemlib_chassis.moveTo(72, 0, 3000);
}

void drive_loop() {
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
}
void regular_loop() {
	// puncher = false, blocker = true

    if (mainController.get_digital(DIGITAL_X))
    {
        lemlib_chassis.moveTo(0, 0, 2000, 50);
        lemlib_chassis.turnTo(100, 0, 2000, false, 50);
    }

	if (mainController.get_digital(DIGITAL_UP))
	{
		// blocker
        pto.set_value(true);
		cata_motor.move_velocity(100);
	}
	else if (mainController.get_digital(DIGITAL_DOWN))
    {
		// blocker
        pto.set_value(true);
        cata_motor.move_velocity(-100);
    }
	else
	{
		cata_motor.brake();
		cata_motor.move_velocity(0);
	}
	
	if (mainController.get_digital(DIGITAL_R1))
	{
		// puncher
        pto.set_value(false);
		cata_motor.move_velocity(-100);
	}

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
        front_left_wing_extended = !front_left_wing_extended;
        front_left_wing.set_value(front_left_wing_extended);
	}
	else if (mainController.get_digital_new_press(DIGITAL_RIGHT))
	{
        front_right_wing_extended = !front_right_wing_extended;
        front_right_wing.set_value(front_right_wing_extended);
	}
}
void shifted_loop() {
	if (mainController.get_digital_new_press(DIGITAL_LEFT))
	{
        back_left_wing_extended = !back_left_wing_extended;
        back_left_wing.set_value(back_left_wing_extended);
	}
	else if (mainController.get_digital_new_press(DIGITAL_RIGHT))
	{
        back_right_wing_extended = !back_right_wing_extended;
        back_right_wing.set_value(back_right_wing_extended);
	}

    if (mainController.get_digital_new_press(DIGITAL_L1))
    {
        // pto shift pull (blocker)
        pto.set_value(true);
        // rotate to block point lol
        blocker_move(blocker_up_pos);
    }
    else if (mainController.get_digital_new_press(DIGITAL_L2))
    {
        // pto shift pull (blocker)
        pto.set_value(true);
        // rotate to hang up point lol
        blocker_move(hang_up_pos);
    }

    if (mainController.get_digital_new_press(DIGITAL_DOWN))
    {
        // pto shift pull (blocker)
        pto.set_value(true);
        // rotate to bottom point lol
        blocker_move(down_pos);
    }

    if (mainController.get_digital_new_press(DIGITAL_UP))
    {
        front_left_wing.set_value(true);
        front_right_wing.set_value(true);
        back_left_wing.set_value(true);
        back_right_wing.set_value(true);
        // blocker up thingy (blocker position)
        blocker_move(blocker_up_pos);
    }
}

void opcontrol() {
	//lemlib_chassis.calibrate();
    //lemlib_chassis.setPose(0,0,0);
    //lemlib_chassis.moveTo(0,25,2000);

	while (true) {
		drive_loop();
		mainController.get_digital(DIGITAL_R2) ? shifted_loop() : regular_loop();
		printf("%d\n", cata_rotation_sensor.get_position());
		pros::c::delay(5);
	}
}
