
//~ Helpers
void MotorMoveAbsoluteAndWait(u8 Motor, f64 Position, f64 Velocity){
 motor_move_absolute(LIFT_MOTOR, 0, GEARSET_36_MAX_VELOCITY);
 
 const f64 Epsilon = 0.001;
 
 while(true){
  if(fabs(motor_get_position(Motor)-Position) <= Epsilon) break;
  delay(1);
 }
}

void MotorMoveRelativeAndWait(u8 Motor, f64 Position, f64 Velocity){
 f64 StartP = motor_get_position(Motor);
 motor_move_relative(LIFT_MOTOR, 0, GEARSET_36_MAX_VELOCITY);
 
 const f64 Epsilon = 0.001;
 
 while(true){
  if(fabs(motor_get_position(Motor)-StartP-Position) <= Epsilon) break;
  delay(1);
 }
}

//~ Match autonomous routines

// Grab short yellow tower
void MatchAutonomousA(){
 motor_move_absolute(LIFT_MOTOR, LIFT_MOVEMENT, GEARSET_36_MAX_VELOCITY);
 ChassisMoveAndWait(&RobotChassis, Inch(48));
 MotorMoveAbsoluteAndWait(LIFT_MOTOR, 0, GEARSET_36_MAX_VELOCITY);
 ChassisMoveAndWait(&RobotChassis, -Inch(28));
 motor_move_absolute(LIFT_MOTOR, LIFT_MOVEMENT, GEARSET_36_MAX_VELOCITY);
 ChassisMoveAndWait(&RobotChassis, -Inch(20));
}

// Aalliance tower on AWP line
void MatchAutonomousB(){
 MotorMoveAbsoluteAndWait(LIFT_MOTOR, LIFT_MOVEMENT, GEARSET_36_MAX_VELOCITY);
 ChassisMoveAndWait(&RobotChassis, Inch(12));
 MotorMoveAbsoluteAndWait(LIFT_MOTOR, 0, GEARSET_36_MAX_VELOCITY);
 // Something here to drop the autonomous preload into the tower
 ChassisMoveAndWait(&RobotChassis, -Inch(12));
 MotorMoveAbsoluteAndWait(LIFT_MOTOR, LIFT_MOVEMENT, GEARSET_36_MAX_VELOCITY);
}

// Alliance tower on ramp
void MatchAutonomousC(){
 MotorMoveAbsoluteAndWait(LIFT_MOTOR, LIFT_MOVEMENT, GEARSET_36_MAX_VELOCITY);
 ChassisMoveAndWait(&RobotChassis, Inch(12));
 MotorMoveAbsoluteAndWait(LIFT_MOTOR, 0, GEARSET_36_MAX_VELOCITY);
 // Something here to drop the autonomous preload into the tower
 ChassisMoveAndWait(&RobotChassis, -Inch(12));
 MotorMoveAbsoluteAndWait(LIFT_MOTOR, LIFT_MOVEMENT, GEARSET_36_MAX_VELOCITY);
}


// Grab short yellow tower and alliance tower on AWP line
void MatchAutonomousD(){
 // Grab short yellow tower
 motor_move_absolute(LIFT_MOTOR, LIFT_MOVEMENT, GEARSET_36_MAX_VELOCITY);
 ChassisMoveAndWait(&RobotChassis, Inch(48));
 MotorMoveAbsoluteAndWait(LIFT_MOTOR, 0, GEARSET_36_MAX_VELOCITY);
 ChassisMoveAndWait(&RobotChassis, -Inch(28));
 motor_move_absolute(LIFT_MOTOR, LIFT_MOVEMENT, GEARSET_36_MAX_VELOCITY);
 ChassisMoveAndWait(&RobotChassis, -Inch(20));
 
 // Do alliance tower
 ChassisRotateAndWait(&RobotChassis, 45);
 MotorMoveAbsoluteAndWait(LIFT_MOTOR, LIFT_MOVEMENT, GEARSET_36_MAX_VELOCITY);
 ChassisMoveAndWait(&RobotChassis, Inch(12));
 MotorMoveAbsoluteAndWait(LIFT_MOTOR, 0, GEARSET_36_MAX_VELOCITY);
 // Something here to drop the autonomous preload into the tower
 ChassisMoveAndWait(&RobotChassis, -Inch(12));
}

void MatchAutonomousE(){
}


//~ Skills autonomous
void SkillsAutonomousA(){
 
}

//~ 
void autonomous(){
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
