#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_

#define PROS_USE_SIMPLE_NAMES

#define PROS_USE_LITERALS

#define MILLISECONDS_PER_TICK 10

//~ Library includes
#include "api.h"
#include "pros/apix.h"
#include "assert.h"

#include "okapi/api.hpp"

using namespace pros;
using namespace pros::c;
using namespace pros::literals;
using namespace okapi::literals;


//~
#include "tyler_basics.h"
#include "robot_basics.h"

#include "integration.h"
#include "motion_profile.h"
#include "chassis.h"

global_constant	u8 LEFT_DRIVE_MOTOR = 1;
global_constant	u8 RIGHT_DRIVE_MOTOR = 10;

global_constant u8 LIFT_MOTOR = 11;
global_constant f64 LIFT_MOVEMENT = 450;

global_constant u8 IMU_PORT = 18;

//~ Autonomous selector

const char *MatchButtonsMap[] = {
 "None",
 "Auto A",
 "Auto B",
 "Auto C",
 "Auto D",
 "Auto E",
 "",
};

const char *SkillsButtonsMap[] = {
 "Auto A",
 "",
};

enum autonomous_mode {
 Autonomous_None    = 0,
 
 MatchAutonomous_A  = 1,
 MatchAutonomous_B  = 2,
 MatchAutonomous_C  = 3,
 MatchAutonomous_D  = 4,
 MatchAutonomous_E  = 5,
 
 SkillsAutonomous_A = 6,
 
 // NOTE(Tyler): Always keep at the end
 Autonomous_TOTAL
};

enum autonomous_selector_tab {
 AutonomousSelectorTab_None   = 0,
 AutonomousSelectorTab_Match  = 1,
 AutonomousSelectorTab_Skills = 2,
};

struct autonomous_selector {
 autonomous_mode Selected;
 
 lv_obj_t *Tabview;
 
 lv_obj_t *MatchPage;
 lv_obj_t *MatchMatrix;
 
 lv_obj_t *SkillsPage;
 lv_obj_t *SkillsMatrix;
};

//~ Declarations
void autonomous(void);
void initialize(void);
void disabled(void);
void competition_initialize(void);
void opcontrol(void);

#endif  // _PROS_MAIN_H_
