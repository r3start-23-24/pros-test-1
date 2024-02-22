#include "main.h"
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/util.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.h"
#include "robot.hpp"
#include "autons.hpp"
#include "autoSelect/selection.h"

bool front_left_wing_extended = false;
bool front_right_wing_extended = false;
bool back_left_wing_extended = false;
bool ratchet_piston_extended = false;

bool down_just_pressed = false;

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

void initialize() {
	lemlib_chassis.calibrate();
    lemlib_chassis.setPose(0,0,0);
	selector::init();

	cata_rotation_sensor.set_data_rate(5);
    // default is 10 (ms)

	cata_motor.set_brake_mode(MOTOR_BRAKE_COAST);
	intake_motor.set_brake_mode(MOTOR_BRAKE_BRAKE);
	left_drive_motors.set_brake_modes(MOTOR_BRAKE_COAST);
	right_drive_motors.set_brake_modes(MOTOR_BRAKE_COAST);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	ratchet_piston.set_value(true);
	pros::c::delay(200);
	cata_motor.move_relative(100, 100);
	pros::c::delay(1000);
	cata_motor.move_relative(-100, 100);
    /*switch (selector::auton) {
        case 0:
            skills_auton();
            break;
        case 1:
            points_auton();
            break;
        case 2:
            awp_auton();
            break;
        default:
            break;
    }*/
	awp_auton();
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
    if (mainController.get_digital(DIGITAL_X))
    {
        lemlib_chassis.moveTo(0, 0, 2000, 50);
        lemlib_chassis.turnTo(100, 0, 2000, false, 50);
    }

	if (mainController.get_digital(DIGITAL_UP))
	{
		// blocker
        pto.set_value(false);
		ratchet_piston.set_value(true);
		cata_motor.move_velocity(100);
	}
	else if (mainController.get_digital(DIGITAL_DOWN))
    {
		// blocker
        pto.set_value(false);
		ratchet_piston.set_value(true);
        cata_motor.move_velocity(-100);
		down_just_pressed = true;
    }
	else
	{
		if (down_just_pressed) {
			ratchet_piston.set_value(false);
			down_just_pressed = false;
		}
		cata_motor.brake();
		cata_motor.move_velocity(0);
	}
	
	if (mainController.get_digital(DIGITAL_R1))
	{
		// puncher
        pto.set_value(true);
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
	if (mainController.get_digital(DIGITAL_L1))
	{
		// blocker
        pto.set_value(false);
		ratchet_piston.set_value(true);
		cata_motor.move_velocity(100);
	}
	else if (mainController.get_digital(DIGITAL_L2))
    {
		// blocker
        pto.set_value(false);
		ratchet_piston.set_value(true);
        cata_motor.move_velocity(-100);
		down_just_pressed = true;
    }
}

void opcontrol() {
	while (true) {
		drive_loop();
		mainController.get_digital(DIGITAL_R2) ? shifted_loop() : regular_loop();
		pros::c::delay(5);
	}
}
