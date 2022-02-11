#ifndef ROBOT_BASICS_H
#define ROBOT_BASICS_H

#define Meter(X) (X)
#define Centimeter(X) (0.01*(X))
#define CM(X) Centimeter(X)
#define Inch(X) (Centimeter(2.54*X))
#define Feet(X) (12*Inch(X))

#define GEARSET_36_MAX_VELOCITY 100 // Red
#define GEARSET_18_MAX_VELOCITY 200 // Green
#define GEARSET_6_MAX_VELOCITY  600 // Blue

#define MOTOR_MIN_VOLTAGE = -12000
#define MOTOR_MAX_VOLTAGE =  12000

#endif //ROBOT_BASICS_H
