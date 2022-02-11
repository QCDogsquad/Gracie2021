//~ Match autonomous
void MatchAutonomousA(){
 motor_move_absolute(LIFT_MOTOR, LIFT_MOVEMENT, GEARSET_36_MAX_VELOCITY);
 ChassisMoveForwardAndWait(&RobotChassis, Inch(48));
 motor_move_absolute(LIFT_MOTOR, 0, GEARSET_36_MAX_VELOCITY);
 delay(1000); // Wait one second
 ChassisMoveForwardAndWait(&RobotChassis, -Inch(28));
 motor_move_absolute(LIFT_MOTOR, LIFT_MOVEMENT, GEARSET_36_MAX_VELOCITY);
 ChassisMoveForwardAndWait(&RobotChassis, -Inch(20));
}


//~ Skills autonomous
void SkillsAutonomousA(){
 
}


//~ 
void autonomous(){
 if(lv_tabview_get_tab_act(AutonomousSelector.Tabview) == 0) return;
 
 switch(AutonomousSelector.Selected){
  case MatchAutonomous_A:  MatchAutonomousA();  break;
  case SkillsAutonomous_A: SkillsAutonomousA(); break;
 }
}
