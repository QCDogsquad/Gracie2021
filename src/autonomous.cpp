//~ Match autonomous routines

// Right side, grab short yellow tower
void MatchAutoA(){
 PreloadBegin(&RobotManager, &RobotPreload);
 ChassisBegin(&RobotManager, &RobotChassis);
 
 //LiftLower();
 COMMAND(&RobotManager, SubsystemFlag_None,                       LiftLower,   &RobotLift);
 COMMAND(&RobotManager, SubsystemFlag_None,                       ChassisMove, &RobotChassis, Inch(54));
 COMMAND(&RobotManager, SubsystemFlag_Chassis|SubsystemFlag_Lift, LiftRaise,   &RobotLift);
 delay(1000); // Settle time
 COMMAND(&RobotManager, SubsystemFlag_Lift,    ChassisMove, &RobotChassis, -Inch(34));
 COMMAND(&RobotManager, SubsystemFlag_Chassis, LiftLower,   &RobotLift);
 COMMAND(&RobotManager, SubsystemFlag_Lift,    ChassisMove, &RobotChassis, -Inch(15));
 
 // AWP tower
 COMMAND(&RobotManager, SubsystemFlag_Chassis, LiftLower,    &RobotLift);
 COMMAND(&RobotManager, SubsystemFlag_None,    ChassisRotate,&RobotChassis, 45);
 COMMAND(&RobotManager, SubsystemFlag_Chassis|SubsystemFlag_Lift, ChassisMove, &RobotChassis, Inch(32));
 COMMAND(&RobotManager, SubsystemFlag_Chassis, LiftPartway,  &RobotLift);
 COMMAND(&RobotManager, SubsystemFlag_Lift,    ChassisMove,  &RobotChassis, -Inch(32));
 
 // Deploy preload
 COMMAND(&RobotManager, SubsystemFlag_Chassis, DoPreload, &RobotPreload, &RobotLift);
 
 // Prepare for OP Control
 COMMAND(&RobotManager, SubsystemFlag_Preload, ChassisRotate, &RobotChassis, 45);
 COMMAND(&RobotManager, SubsystemFlag_Chassis, ChassisMove,   &RobotChassis, Inch(10));
 COMMAND(&RobotManager, SubsystemFlag_Chassis, LiftLower,     &RobotLift);
 COMMAND(&RobotManager, SubsystemFlag_Lift,    ChassisMove,   &RobotChassis, -Inch(10));
 COMMAND(&RobotManager, SubsystemFlag_Chassis, ChassisRotate, &RobotChassis, -125);
 COMMAND(&RobotManager, SubsystemFlag_Chassis|SubsystemFlag_Lift, ChassisMove,   &RobotChassis, Inch(18));
}

// Right side, alliance tower on AWP line
void MatchAutoB(){
 PreloadBegin(&RobotManager, &RobotPreload);
 
 // AWP tower
 COMMAND(&RobotManager, SubsystemFlag_Chassis, LiftLower,        &RobotLift);
 COMMAND(&RobotManager, SubsystemFlag_None,    ChassisRotate,    &RobotChassis, 45);
 COMMAND(&RobotManager, SubsystemFlag_Chassis|SubsystemFlag_Lift, ChassisMove, &RobotChassis, Inch(32));
 COMMAND(&RobotManager, SubsystemFlag_Chassis, LiftPartway, &RobotLift);
 COMMAND(&RobotManager, SubsystemFlag_Lift,    ChassisMove,      &RobotChassis, -Inch(32));
 
 // Deploy preload
 COMMAND(&RobotManager, SubsystemFlag_Chassis, DoPreload, &RobotPreload, &RobotLift);
 
}

// Left side, alliance tower on ramp
void MatchAutoC(){
 
}

// Right side, grab short yellow tower and alliance tower on AWP line
void MatchAutoD(){
 
}

// Left side, grab short yellow tower
void MatchAutoE(){
 
}

//~ Skills autonomous

void SkillsAutoA(){
#if 0
 LiftLowerPartwayAndWait();
 ChassisMoveAndWait(&RobotChassis, Inch(24));
 
 LiftLower();
 ChassisMoveAndWait(&RobotChassis, Inch(0));
 RaiseLiftAndWait();
 ChassisRotateAndWait(&RobotChassis, 180);
 ChassisMoveAndWait(&RobotChassis, Inch(0));
 LiftLowerAndWait();
 ChassisMoveAndWait(&RobotChassis, -Inch(0));
 
 ChassisRotateAndWait(&RobotChassis, -90);
 LiftLower();
 ChassisMoveAndWait(&RobotChassis, Inch(0));
#endif
}

//~ 
void autonomous(){
 // NOTE(Tyler): We always want it to hold and stop immediately
 ChassisBrakeMode(&RobotChassis, E_MOTOR_BRAKE_HOLD);
 
 if(lv_tabview_get_tab_act(AutoSelector.Tabview) == 0) return;
 //AutoSelector.Selected = MatchAuto_B;
 
 switch(AutoSelector.Selected){
  case MatchAuto_A:  MatchAutoA();  break;
  case MatchAuto_B:  MatchAutoB();  break;
  case MatchAuto_C:  MatchAutoC();  break;
  case MatchAuto_D:  MatchAutoD();  break;
  case MatchAuto_E:  MatchAutoE();  break;
  case SkillsAuto_A: SkillsAutoA(); break;
 }
}
