#include "autons.hpp"
#include "pros/motors.h"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include "robot.hpp"

const float one_lemlib_tile = 24;
float lemlib_x;
float lemlib_y;

void move_lemlib_deprecated(float tiles, char direction, int velocity = 90) {
    direction == 'x' ? lemlib_x += one_lemlib_tile*tiles : lemlib_y += one_lemlib_tile*tiles;
    lemlib_chassis.moveTo(lemlib_x, lemlib_y, 3000, velocity);
}

void move_lemlib(float tiles, char direction, int velocity = 90) {
    lemlib_x = lemlib_chassis.getPose().x;
    lemlib_y = lemlib_chassis.getPose().y;
    direction == 'x' ? lemlib_x += one_lemlib_tile*tiles : lemlib_y += one_lemlib_tile*tiles;
    lemlib_chassis.moveTo(lemlib_x, lemlib_y, 3000, velocity);
}

const int oneTile = 160;
void move_forward_old(float tiles, int velocity) {
    left_drive_motors.move_relative(tiles * oneTile, velocity);
    right_drive_motors.move_relative(tiles * oneTile, velocity);
    pros::c::delay(100);
    while (left_motor_1.get_actual_velocity() > 2)
    {
        pros::c::delay(5);
    }
    left_drive_motors.brake();
    right_drive_motors.brake();
}

void skills_auton() {
    intake_motor.move(-127);
    pros::c::delay(500);
    lemlib_chassis.moveTo(0.2*one_lemlib_tile, -0.7*one_lemlib_tile, 2500, 80);
    lemlib_chassis.turnTo(one_lemlib_tile, 1.5*one_lemlib_tile, 1500);
    back_left_wing.set_value(true);
    puncher_motor.move(127);
    intake_motor.move(0);
    pros::c::delay(28000); // punch for __ secs
    mainController.rumble("--");
    pros::c::delay(5000);
    puncher_motor.move(0);
    back_left_wing.set_value(false);
    lemlib_chassis.moveTo(-8, 0, 3000, 100);
    lemlib_chassis.moveTo(-8, 3.4*one_lemlib_tile, 3000, 100); // gone under bar
    lemlib_chassis.moveTo(11, 3*one_lemlib_tile, 2000);
    lemlib_chassis.moveTo(11, 1.65*one_lemlib_tile, 2000); // dodge pos
    front_left_wing.set_value(true);
    front_right_wing.set_value(true);
    lemlib_chassis.moveTo(1.25*one_lemlib_tile, 3*one_lemlib_tile, 2000); // ready to push
    lemlib_chassis.turnTo(1.5*one_lemlib_tile, 1000, 1500); // turn to face goal
    lemlib_chassis.moveTo(1.5*one_lemlib_tile, 3*one_lemlib_tile, 2000); // push in
    lemlib_chassis.moveTo(1.5*one_lemlib_tile, 1.75*one_lemlib_tile, 2000); // pull out
    lemlib_chassis.moveTo(1.5*one_lemlib_tile, 3*one_lemlib_tile, 2000); // push in
    lemlib_chassis.moveTo(1.5*one_lemlib_tile, 1.75*one_lemlib_tile, 2000); // pull out
    front_left_wing.set_value(false);
    front_right_wing.set_value(false);
    lemlib_chassis.moveTo(2.5*one_lemlib_tile, 1.75*one_lemlib_tile, 2000); // push 3 pos
    front_left_wing.set_value(true);
    front_right_wing.set_value(true);
    lemlib_chassis.turnTo(2*one_lemlib_tile, 10000, 2000); // push 3 pos
    lemlib_chassis.moveTo(2*one_lemlib_tile, 3.5*one_lemlib_tile, 2000); // push in
    lemlib_chassis.moveTo(2*one_lemlib_tile, 1.75*one_lemlib_tile, 2000); // pull out
    lemlib_chassis.moveTo(2*one_lemlib_tile, 3.5*one_lemlib_tile, 2000); // push in
    lemlib_chassis.moveTo(2*one_lemlib_tile, 1.75*one_lemlib_tile, 2000); // pull out
    front_left_wing.set_value(false);
    front_right_wing.set_value(false);
    lemlib_chassis.moveTo(4.1*one_lemlib_tile, 3.4*one_lemlib_tile, 3000, 100);
    lemlib_chassis.moveTo(4.1*one_lemlib_tile, 4.4*one_lemlib_tile, 1500, 100);
    lemlib_chassis.moveTo(3.1*one_lemlib_tile, 4.4*one_lemlib_tile, 750, 100);
    lemlib_chassis.moveTo(4.1*one_lemlib_tile, 4.4*one_lemlib_tile, 1500, 100);
}

void points_auton() {
    intake_motor.move(127);
    move_lemlib(0.1, 'y', 100);
    move_lemlib(-1.2, 'y', 100);
    back_left_wing.set_value(true);
    // start arc
    left_drive_motors.move_velocity(-400);
    right_drive_motors.move_velocity(-200);
    pros::c::delay(1100);
    left_drive_motors.brake();
    right_drive_motors.brake();
    // end arc
    back_left_wing.set_value(false);
    lemlib_chassis.turnTo(-one_lemlib_tile, -1.4*one_lemlib_tile, 1500, false, 70);
    // dodgy sideways hit ^^
    lemlib_chassis.moveTo(1.2, -3*one_lemlib_tile, 1500);
    move_lemlib(-0.2, 'x');
    lemlib_chassis.turnTo(100, -2.5*one_lemlib_tile, 1500, false, 70);
    intake_motor.move(-127);
    move_lemlib(0.5, 'x', 600);
    move_lemlib(-0.2, 'x');
    pros::c::delay(5000000);
    intake_motor.move(127);
    pros::c::delay(5000);
    lemlib_chassis.moveTo(1.5*one_lemlib_tile, -0.1*one_lemlib_tile, 2000, 90);
    lemlib_chassis.turnTo(2*one_lemlib_tile, -1*one_lemlib_tile, 2000, 70);
    intake_motor.move(-127);
    pros::c::delay(500); // wait for ball to be outtaked
    intake_motor.move(127);
    lemlib_chassis.moveTo(2.5*one_lemlib_tile, -0.1*one_lemlib_tile, 1500, 60);
    lemlib_chassis.turnTo(2.5*one_lemlib_tile, 1000, 1500, 60);
    back_left_wing.set_value(true);
    move_lemlib(-2, 'y');
    move_lemlib(0.5, 'y');
    back_left_wing.set_value(false);
    lemlib_chassis.turnTo(2.5*one_lemlib_tile, -2*one_lemlib_tile, 1500, 70);
    intake_motor.move(-127);
    move_lemlib(-0.6, 'y');
}

void dodge_auton() {
    lemlib_chassis.setPose(-6, 0, 345);
    intake_motor.move(127);
    lemlib_chassis.moveTo(-0.65*one_lemlib_tile, 2.1*one_lemlib_tile, 1500);
    // picked up ball one
    lemlib_chassis.turnTo(100, 2*one_lemlib_tile, 1500);
    intake_motor.move(-127);
    pros::c::delay(750);
    // outtaken
    intake_motor.move(127);
    lemlib_chassis.turnTo(-100, 2.2*one_lemlib_tile, 1500);
    lemlib_chassis.moveTo(-1.25*one_lemlib_tile, 2.3*one_lemlib_tile, 1500);
    // picked up ball two
    lemlib_chassis.turnTo(100, 2.1*one_lemlib_tile, 1500);
    lemlib_chassis.moveTo(-0.8*one_lemlib_tile, 2.1*one_lemlib_tile, 1500);
    intake_motor.move(-127);
    pros::c::delay(1000);
    // outtaken
    intake_motor.move(127);
    lemlib_chassis.turnTo(-100, 1*one_lemlib_tile, 1500);
    lemlib_chassis.moveTo(-1.4*one_lemlib_tile, 1.2*one_lemlib_tile, 1500, 100);
    // picked up ball three
    lemlib_chassis.turnTo(100, 2*one_lemlib_tile, 1500);
    lemlib_chassis.moveTo(-1.25*one_lemlib_tile, 2*one_lemlib_tile, 1500);
    lemlib_chassis.turnTo(100, 2*one_lemlib_tile, 1500);
    intake_motor.move(-127);
    // outtaken
    front_left_wing.set_value(true);
    front_right_wing.set_value(true);
    lemlib_chassis.moveTo(0, 2*one_lemlib_tile, 1500);
    lemlib_chassis.moveTo(-one_lemlib_tile, 2*one_lemlib_tile, 1500);
    lemlib_chassis.moveTo(0, 2*one_lemlib_tile, 1500);
    lemlib_chassis.moveTo(-one_lemlib_tile, 2*one_lemlib_tile, 1500);
    front_left_wing.set_value(false);
    front_right_wing.set_value(false);
}

void open_wing() {
    pros::c::delay(400);
    back_right_wing.set_value(true);
}

int mode = 0;

void stop_if_bad() {
    pros::c::delay(200);
    while (true) {
        if ( (mode == 1 && inertial_sensor.get_accel().x > 0.04) || (mode == 2 && inertial_sensor.get_accel().x < 0.0425) )
        {
            left_drive_motors.brake();
            right_drive_motors.brake();
            break;
        }
        pros::c::delay(5);
    }
}

void awp_auton() {
	intake_motor.move(127);
	move_lemlib(2.25, 'y', 130);
    lemlib_chassis.turnTo(100, 2.25*one_lemlib_tile, 1000);
	front_left_wing.set_value(true);
    lemlib_chassis.turnTo(-1000, 2.25*one_lemlib_tile, 1000, 60);
    mode = 1;
    pros::Task stop_thr(stop_if_bad);
    lemlib_chassis.moveTo(1.1*one_lemlib_tile, 2.25*one_lemlib_tile, 3000, 150);
    // pushed over
	front_left_wing.set_value(false);
    lemlib_chassis.turnTo(-1000, 2.25*one_lemlib_tile, 1500);
    intake_motor.move(-127);
    mode = 2;
    pros::Task stop_thr_2(stop_if_bad);
    lemlib_chassis.moveTo(-5, 2.25*one_lemlib_tile, 1500);
    pros::c::delay(500);
    // scored coloured ball
    lemlib_chassis.moveTo(-0.2, 2.25*one_lemlib_tile, 1500);
    left_drive_motors.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
    right_drive_motors.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
    lemlib_chassis.moveTo(0, -2, 1500, 200);
    // gone to 0,0
    lemlib_chassis.moveTo(-24, 12, 3000, 50);
    left_drive_motors.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
    right_drive_motors.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
    lemlib_chassis.turnTo(0, -12, 1500);
    pros::Task open_wing_thread(open_wing);
    pros::c::delay(200);
    lemlib_chassis.moveTo(2, -14, 1500, 50); // dis
    back_right_wing.set_value(false);
    pros::c::delay(100);
    back_right_wing.set_value(false);
    pros::c::delay(200);
    back_right_wing.set_value(false);
    lemlib_chassis.turnTo(-100, 20, 1500, true, 80); // move 2.3?
    lemlib_chassis.moveTo(24, -4.8, 1500, 70);
}
