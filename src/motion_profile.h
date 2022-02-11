/* date = August 17th 2021 8:11 pm */

#ifndef MOTION_PROFILE_H
#define MOTION_PROFILE_H

//~ 1D motion profiles
enum motion_profile_1d_type {
 MotionProfile_Position,
 MotionProfile_Velocity,
};

struct motion_profile_1d {
 motion_profile_1d_type Type;
 f64 Jerk;
 f64 tr; // The period of interrupted time where jerk is non-zero
 f64 th; // The period of interrupted time where acceleration is constant and non-zero
 f64 tw; // The period of interrupted time where acceleration is zero
};

//~ 2D morion profiles
struct motion_profile_2d {
 motion_profile_1d X;
 motion_profile_1d Y;
};

#endif //MOTION_PROFILE_H
