
//~ Helpers
void MotorMoveAbsoluteAndWait(u8 Motor, f64 Position, f64 Velocity){
 motor_move_absolute(Motor, Position, Velocity);
 
 const f64 Epsilon = 0.1;
 
 while(true){
  if(fabs(motor_get_position(Motor)-Position) <= Epsilon) break;
  delay(1);
 }
}

void MotorMoveRelativeAndWait(u8 Motor, f64 Position, f64 Velocity){
 f64 StartP = motor_get_position(Motor);
 motor_move_relative(Motor, Position, Velocity);
 
 const f64 Epsilon = 0.1;
 
 while(true){
  if(fabs(motor_get_position(Motor)-StartP-Position) <= Epsilon) break;
  delay(1);
 }
}

//~ Match autonomous routines

// Right side, grab short yellow tower
void MatchAutonomousA(){
 LowerLift();
 ChassisMoveAndWait(&RobotChassis, Inch(54));
 RaiseLiftAndWait();
 delay(1000);
 ChassisMoveAndWait(&RobotChassis, -Inch(34));
 LowerLift();
 ChassisMoveAndWait(&RobotChassis, -Inch(20));
}

// Right side, alliance tower on AWP line
void MatchAutonomousB(){
 ChassisRotateAndWait(&RobotChassis, 45);
 LowerLiftAndWait();
 ChassisMoveAndWait(&RobotChassis, Inch(18));
 RaiseLiftAndWait();
 ChassisMoveAndWait(&RobotChassis, -Inch(18));
 LowerLiftAndWait();
 motor_move_absolute(PRELOAD_MOTOR, PRELOAD_EMPTY_P, PRELOAD_VELOCITY);
}

// Left side, alliance tower on ramp
void MatchAutonomousC(){
 LowerLiftAndWait();
 ChassisMoveAndWait(&RobotChassis, Inch(12));
 RaiseLiftAndWait();
 motor_move_absolute(PRELOAD_MOTOR, PRELOAD_EMPTY_P, PRELOAD_VELOCITY);
 ChassisMoveAndWait(&RobotChassis, -Inch(12));
 LowerLiftAndWait();
}


// Right side, grab short yellow tower and alliance tower on AWP line
void MatchAutonomousD(){
 // Grab short yellow tower
 LowerLift();
 ChassisMoveAndWait(&RobotChassis, Inch(54));
 RaiseLiftAndWait();
 ChassisMoveAndWait(&RobotChassis, -Inch(34  ));
 LowerLift();
 ChassisMoveAndWait(&RobotChassis, -Inch(20));
 
 // Do alliance tower
 ChassisRotateAndWait(&RobotChassis, 45);
 LowerLiftAndWait();
 ChassisMoveAndWait(&RobotChassis, Inch(12));
 RaiseLiftAndWait();
 motor_move_absolute(PRELOAD_MOTOR, PRELOAD_EMPTY_P, PRELOAD_VELOCITY);
 ChassisMoveAndWait(&RobotChassis, -Inch(12));
}

// Left side, grab short yellow tower
void MatchAutonomousE(){
 
}

//~ Skills autonomous

void SkillsAutonomousA(){
 LowerLiftPartwayAndWait();
 ChassisMoveAndWait(&RobotChassis, Inch(24));
 
#if 0
 LowerLift();
 ChassisMoveAndWait(&RobotChassis, Inch(0));
 RaiseLiftAndWait();
 ChassisRotateAndWait(&RobotChassis, 180);
 ChassisMoveAndWait(&RobotChassis, Inch(0));
 LowerLiftAndWait();
 ChassisMoveAndWait(&RobotChassis, -Inch(0));
 
 ChassisRotateAndWait(&RobotChassis, -90);
 LowerLift();
 ChassisMoveAndWait(&RobotChassis, Inch(0));
#endif
}

//~ 
void autonomous(){
 // NOTE(Tyler): We always want it to hold and stop immediately
 ChassisBrakeMode(&RobotChassis, E_MOTOR_BRAKE_HOLD);
 
 if(lv_tabview_get_tab_act(AutonomousSelector.Tabview) == 0) return;
 
 switch(AutonomousSelector.Selected){
  case MatchAutonomous_A:  MatchAutonomousA();  break;
  case MatchAutonomous_B:  MatchAutonomousB();  break;
  case MatchAutonomous_C:  MatchAutonomousC();  break;
  case MatchAutonomous_D:  MatchAutonomousD();  break;
  case MatchAutonomous_E:  MatchAutonomousE();  break;
  case SkillsAutonomous_A: SkillsAutonomousA(); break;
 }
}
