#ifndef CHASSIS_H
#define CHASSIS_H

//~ PID
struct pidf_gains {
 f64 kP, kI, kD, kF;
};

struct pidf_controller {
 union {
  f64 kP, kI, kD, kF;
  pidf_gains Gains;
 };
 f64 PreviousError;
 f64 ErrorSum;
};

//~ Chassis
struct basic_chassis {
 f64 MaxVelocity;
 u8 Left;
 u8 Right;
};

struct pidf_chassis : public basic_chassis {
 pidf_controller DistancePIDF;
 pidf_controller TurnPIDF;
};


#endif //CHASSIS_H
