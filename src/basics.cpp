
b8 MotorIsAtPosition(u8 Motor, f64 Position, f64 Epsilon){
 if(fabs(motor_get_position(Motor)-Position) <= Epsilon) return true;
 return false;
}

b8 Motor2xIsAtPosition(u8 A, u8 B, f64 Position, f64 Epsilon){
 f64 AverageP = 0.5*(motor_get_position(A)+motor_get_position(B));
 if(fabs(AverageP-Position) <= Epsilon) return true;
 return false;
}
