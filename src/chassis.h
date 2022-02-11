#ifndef CHASSIS_H
#define CHASSIS_H

struct basic_chassis {
 u8 Left;
 u8 Right;
 u8 IMU;
 
 f64 MaxVelocity;
 f64 AutonomousVelocity;
 
 f64 Track; 
 f64 WheelDiameter;
 f64 GearingFactor;
 
 v2 dP;
 v2 P;
};


#endif //CHASSIS_H
