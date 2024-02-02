#include "main.h"
#include "gif-pros/gifclass.hpp"
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
const float allowance = 0.1; // in rotations now
const int down_pos = 2;
const int blocker_up_pos = -1;
const float hang_up_pos = -0.5;

void blocker_move(float pos) {
	cata_motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	cata_motor.move(127);
	while (!(cata_rotation_sensor.get_position() > pos-allowance && cata_rotation_sensor.get_position() < pos+allowance))
	{
		pros::c::delay(1);
        printf("%d", cata_rotation_sensor.get_position());
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
	lemlib_chassis.calibrate();
    lemlib_chassis.setPose(0,0,0);
	selector::init();

	cata_motor.set_brake_mode(MOTOR_BRAKE_COAST);
	intake_motor.set_brake_mode(MOTOR_BRAKE_BRAKE);
	left_drive_motors.set_brake_modes(MOTOR_BRAKE_COAST);
	right_drive_motors.set_brake_modes(MOTOR_BRAKE_COAST);
}

void disabled() {}

// pre-auton (eg auton selector)
void competition_initialize() {}

void autonomous() {
	lemlib_chassis.calibrate();
    lemlib_chassis.setPose(0,0,0);
    lemlib_chassis.turnTo(30,0,2000);
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
	// new puncher code
	if (mainController.get_digital(DIGITAL_UP))
	{
        pto.set_value(false);
		cata_motor.move_velocity(100);
	}
	else if (mainController.get_digital(DIGITAL_R1))
	{
        pto.set_value(false);
		cata_motor.move_velocity(-100);
	}
	else if (mainController.get_digital(DIGITAL_DOWN))
    {
        pto.set_value(false);
        cata_motor.move_velocity(-100);
    }
	else
	{
		cata_motor.brake();
		cata_motor.move_velocity(0);
	}
	// end new puncher code
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
	
	//pros::Task gifs(gifthread);

	pros::c::delay(500);
	//cata_down();

	while (true) {
		drive_loop();
		mainController.get_digital(DIGITAL_R2) ? shifted_loop() : regular_loop();
		pros::c::delay(5);
	}
}
