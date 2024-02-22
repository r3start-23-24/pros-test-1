#include "main.h"
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/util.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include "robot.hpp"
#include "autons.hpp"
#include "autoSelect/selection.h"

bool front_left_wing_extended = false;
bool front_right_wing_extended = false;
bool back_left_wing_extended = false;
bool back_right_wing_extended = false;
bool ratchet_piston_extended = false;

bool down_just_pressed = false;

// start move blocker funcs
const int allowance = 500; // 5 degrees
const int down_pos = 0;
const int high_hang_pos = 106514;
const int sidebar_hang_pos = 73907;

void blocker_move(float pos) {
	cata_motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    if (cata_rotation_sensor.get_position() - pos > 0) cata_motor.move(-127);
    else cata_motor.move(127);
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

void auton_start() {
    ratchet_piston.set_value(true);
    pros::c::delay(200);
    blocker_move(down_pos);
}

void autonomous() {
    pros::Task auton_start_thread(auton_start);
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
        ratchet_piston.set_value(true);
        blocker_move(sidebar_hang_pos);
		cata_motor.move_velocity(100);
	}
	else if (mainController.get_digital(DIGITAL_DOWN))
    {
		ratchet_piston.set_value(true);
        blocker_move(down_pos);
        cata_motor.move_velocity(-100);
        // push ratchet in
    }
	
	if (mainController.get_digital(DIGITAL_R1))
	{
		// puncher
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

    if (mainController.get_digital_new_press(DIGITAL_UP))
    {
        blocker_move(high_hang_pos);
    }

	if (mainController.get_digital(DIGITAL_L1))
	{
		// blocker
		ratchet_piston.set_value(true);
		cata_motor.move_velocity(100);
	}
	else if (mainController.get_digital(DIGITAL_L2))
    {
		// blocker
		ratchet_piston.set_value(true);
        cata_motor.move_velocity(-100);
		down_just_pressed = true;
    }
}

void opcontrol() {
	while (true) {
		drive_loop();
		mainController.get_digital(DIGITAL_R2) ? shifted_loop() : regular_loop();
        //printf("%d\n", cata_rotation_sensor.get_position());
		pros::c::delay(5);
	}
}
