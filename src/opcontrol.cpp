
//~ Driver control
void opcontrol(){
 b8 LiftToggled = false;
 b8 ReverseToggled = false;
 b8 SlowModeToggled = false;
 
 u32 PreviousTime = millis();
 f64 dTime = (f64)MILLISECONDS_PER_TICK / 1000;
 while(true)
 {
  //~ Chassis control
  f64 Forward = (f64)controller_get_analog(CONTROLLER_MASTER, ANALOG_LEFT_Y)/127.0;
  f64 Turn = (f64)controller_get_analog(CONTROLLER_MASTER, ANALOG_RIGHT_X)/127.0;
  
  if(controller_get_digital_new_press(CONTROLLER_MASTER, DIGITAL_DOWN)) ReverseToggled = !ReverseToggled;
  if(ReverseToggled) Forward *= -1.0;
  
  if(controller_get_digital_new_press(CONTROLLER_MASTER, DIGITAL_UP)) SlowModeToggled = !SlowModeToggled;
  if(SlowModeToggled) Forward *= 0.5;
  
  ChassisMoveArcade(&RobotChassis, Forward, Turn);
  v2 P = ChassisIntegrateP(&RobotChassis, dTime);
  printf("P: (%f, %f)\n", P.X, P.Y);
  
  //~ Lift controls
#if 1
  if (controller_get_digital_new_press(CONTROLLER_MASTER, DIGITAL_R2)) LiftToggled = !LiftToggled;
  if(LiftToggled)  motor_move_absolute(LIFT_MOTOR, LIFT_MOVEMENT, GEARSET_36_MAX_VELOCITY);
  else motor_move_absolute(LIFT_MOTOR, 0, GEARSET_36_MAX_VELOCITY);
#else
  if(controller_get_digital(CONTROLLER_MASTER, DIGITAL_R2)) motor_move_voltage(LIFT_MOTOR, -12000);
  else if(controller_get_digital(CONTROLLER_MASTER, DIGITAL_R1)) motor_move_voltage(LIFT_MOTOR, 12000);
#endif
  
  task_delay_until(&PreviousTime, MILLISECONDS_PER_TICK);
 }
}