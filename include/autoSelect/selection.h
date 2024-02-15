#pragma once

#include <string>

/* 
  selector::auton == 1 : Red same goal POINTS
  selector::auton == 2 : Red other goal AWP
  selector::auton == 3 : Do Nothing
  selector::auton == -1 : Blue same goal
  selector::auton == -2 : Blue other goal
  selector::auton == -3 : Do Nothing
  selector::auton == 0 : Skills
*/

//selector configuration
#define HUE 360
#define DEFAULT -2
#define AUTONS "same goal", "other goal", "nothing"
// 1, 2, 3

namespace selector {
    extern int auton;
    const char *b[] = {AUTONS, ""};
    void init(int hue = HUE, int default_auton = DEFAULT, const char **autons = b);
}
