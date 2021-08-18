
#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_

#define PROS_USE_SIMPLE_NAMES

#define PROS_USE_LITERALS

//~ Constants
#define LEFT_DRIVE_MOTOR 19
#define RIGHT_DRIVE_MOTOR 18
#define CLAW_OPEN_MOTOR 7
#define CLAW_WRIST_MOTOR 6 
#define LEFT_LIFT_MOTOR 20
#define RIGHT_LIFT_MOTOR 13

#define PATH_A "A"

#define MILLISECONDS_PER_TICK 10

#define GEARSET_36_MAX_VELOCITY 100 // Red
#define GEARSET_18_MAX_VELOCITY 200 // Green
#define GEARSET_6_MAX_VELOCITY  600 // Blue

// TODO(Tyler): These are just complete guesses, these need to be tuned to reasonable starting values
#define DEFAULT_DISTANCE_GAINS PIDF(0.1f, 0.0f, 0.0f, 0.5f)
#define DEFAULT_TURN_GAINS     PIDF(0.1f, 0.0f, 0.0f, 0.5f)


//~ Library includes
#include "api.h"
#include "pros/apix.h"
#include "assert.h"

#include "okapi/api.hpp"

using namespace pros;
using namespace pros::c;
using namespace pros::literals;
using namespace okapi::literals;

//~ Project includes
#include "tyler_utilities.h"
#include "chassis.h"
#include "motion_profile.h"


// NOTE(Tyler): Declared here so the ones that aren't used are rememembered
void autonomous(void);
void initialize(void);
void disabled(void);
void competition_initialize(void);
void opcontrol(void);

#endif  // _PROS_MAIN_H_
