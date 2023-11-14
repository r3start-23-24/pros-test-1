#pragma once

#include <string>

/* 
  selector::auton == 1 : Red Front
  selector::auton == 2 : Red Back
  selector::auton == 3 : Do Nothing
  selector::auton == -1 : Blue Front
  selector::auton == -2 : Blue Back
  selector::auton == -3 : Do Nothing
  selector::auton == 0 : Skills
*/

//selector configuration
#define HUE 360
#define DEFAULT 1
#define AUTONS "same goal", "other goal", "nothing"
// 1, 2, 3

namespace selector{

extern int auton;
const char *b[] = {AUTONS, ""};
void init(int hue = HUE, int default_auton = DEFAULT, const char **autons = b);

}
