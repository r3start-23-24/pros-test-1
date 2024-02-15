#include "autons.hpp"
#include "robot.hpp"

const float one_lemlib_tile = 24;
float lemlib_x;
float lemlib_y;

void move_lemlib(float tiles, char direction, int velocity = 90) {
    direction == 'x' ? lemlib_x += one_lemlib_tile*tiles : lemlib_y += one_lemlib_tile*tiles;
    lemlib_chassis.moveTo(lemlib_x, lemlib_y, 3000, velocity);
}

void turn_lemlib(int alpha, int velocity = 50) {
    int theta = lemlib_chassis.getPose().theta;
    theta = lemlib::degToRad(theta);
    alpha = lemlib::degToRad(alpha);
    lemlib_chassis.turnTo(lemlib_x - sin(theta + alpha), lemlib_y + cos(theta + alpha), 1000);
}

void skills_auton() {
	intake_motor.move(127);
	move_lemlib(2.25, 'y', 90);
	intake_motor.move(0);
	lemlib_chassis.turnTo(-100, 2.5*one_lemlib_tile, 1000, 60);
	front_left_wing.set_value(true);
	intake_motor.move(-127);
	move_lemlib(1, 'x', 90);
	front_left_wing.set_value(false);
	//move_lemlib(-0.5, 'x', 90);
	lemlib_chassis.moveTo(0.4*one_lemlib_tile, 2.25*one_lemlib_tile, 2000, 80);
	lemlib_chassis.moveTo(-one_lemlib_tile+10, one_lemlib_tile/2, 3000, 60);
	lemlib_chassis.turnTo(-24, 5, 1000, true, 60);
}

void awp_auton() {

}

void points_auton() {

}
