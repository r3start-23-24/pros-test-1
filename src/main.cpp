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

const int allowance = 500; // 5 degrees
const int down_pos = 10000;
const int high_hang_pos = 106514;
const int sidebar_hang_pos = 73907;

bool cata_was_moving = false;
bool cata_toggle_on = false;

bool skills = false;

void blocker_move(float pos) {
    if (cata_rotation_sensor.get_position() - pos > 0) hang_motor.move(-127);
    else hang_motor.move(127);
	while (!(cata_rotation_sensor.get_position() > (pos-allowance) && cata_rotation_sensor.get_position() < (pos+allowance) ))
	{
		pros::c::delay(5);
        //printf("%d\n", cata_rotation_sensor.get_position());
	}
	hang_motor.brake();
}

void blocker_move_thread() {
    while (true) {
        if (mainController.get_digital_new_press(DIGITAL_UP) && !mainController.get_digital(DIGITAL_R2)) blocker_move(sidebar_hang_pos);
        else if (mainController.get_digital_new_press(DIGITAL_DOWN) && !mainController.get_digital(DIGITAL_R2)) blocker_move(down_pos);
        pros::c::delay(5);
    }
}

void ratchet_lock() {
    pros::c::delay(103*1000);
    ratchet_piston.set_value(true);
}

void initialize() {
	lemlib_chassis.calibrate();
    lemlib_chassis.setPose(0,0,0);
	selector::init();

    if (selector::auton == 0) skills = true;

	cata_rotation_sensor.set_data_rate(5);
    // default is 10 (ms)

	hang_motor.set_brake_mode(MOTOR_BRAKE_HOLD);
    puncher_motor.set_brake_mode(MOTOR_BRAKE_COAST);
	intake_motor.set_brake_mode(MOTOR_BRAKE_BRAKE);
	left_drive_motors.set_brake_modes(MOTOR_BRAKE_COAST);
	right_drive_motors.set_brake_modes(MOTOR_BRAKE_COAST);
}

void disabled() {}

void competition_initialize() {}

void auton_start() {
    if (!skills) blocker_move(down_pos);
}

void autonomous() {
    pros::Task auton_start_thread(auton_start);
    switch (selector::auton) {
        case 0:
            skills_auton();
            skills = true;
            break;
        case 1:
            points_auton();
            break;
        case 2:
            awp_auton();
            break;
        default:
            break;
    }
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
    if (mainController.get_digital_new_press(DIGITAL_X) && skills)
    {
        const int one_lemlib_tile = 24;
        lemlib_chassis.moveTo(0.2*one_lemlib_tile, -0.7*one_lemlib_tile, 2500, 80);
        lemlib_chassis.turnTo(one_lemlib_tile, 1.5*one_lemlib_tile, 1500);
        back_left_wing.set_value(true);
    }
	if (mainController.get_digital_new_press(DIGITAL_R1))
	{
        if (cata_toggle_on)
        {
            puncher_motor.brake();
            cata_toggle_on = false;
        }
        else
        {
            puncher_motor.move_velocity(100);
            cata_toggle_on = true;
        }
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

    if (mainController.get_digital(DIGITAL_L1))
    {
        hang_motor.move(127);
        cata_was_moving = true;
    }
    else if (mainController.get_digital(DIGITAL_L2)) 
    {
        hang_motor.move(-127);
        cata_was_moving = true;
    }
    else if (cata_was_moving)
    {
        hang_motor.brake();
    }
}

void opcontrol() {
    //if (!skills) {
        pros::Task blocker_move_thread_thr(blocker_move_thread);
        pros::Task ratchet_lock_thread(ratchet_lock);
    //}
	while (true) {
		drive_loop();
		mainController.get_digital(DIGITAL_R2) ? shifted_loop() : regular_loop();
        //printf("%d\n", cata_rotation_sensor.get_position());
        printf("x: %f, y: %f, z: %f", inertial_sensor.get_accel().x, inertial_sensor.get_accel().y, inertial_sensor.get_accel().z);
		pros::c::delay(5);
	}
}
