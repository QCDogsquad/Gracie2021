
//~ Intergration
inline integration_context
MakeIntegrationContext(f64 dTime, f64 EndTime, f64 TheoreticalMaxAcceleration, f64 TheoreticalMaxVelocity){
 integration_context Context;
 
 Context.EndTime = EndTime;
 Context.dTime = dTime;
 Context.TheoreticalMaxAcceleration = TheoreticalMaxAcceleration;
 Context.TheoreticalMaxVelocity = TheoreticalMaxVelocity;
 
 return Context;
}

b8
IntegrationDoContinue(integration_context *Context){
 b8 Result = Context->CurrentTime < Context->EndTime+Context->dTime;
 return Result;
}

void 
IntegrationStep(integration_context *Context, f64 Jerk){
 Context->Position += (Context->Velocity*Context->dTime +
                       0.5*Context->Acceleration*Square(Context->dTime) + 
                       (1.0/6.0)*Jerk*Cube(Context->dTime) );
 Context->Velocity += (Context->Acceleration*Context->dTime +
                       0.5*Jerk*Square(Context->dTime) );
 Context->Acceleration += Jerk*Context->dTime;
 
 if(Context->Acceleration > Context->MaxFoundAcceleration) Context->MaxFoundAcceleration = Context->Acceleration;
 if(Context->Acceleration < Context->MinFoundAcceleration) Context->MinFoundAcceleration = Context->Acceleration;
 
 if(Context->Velocity > Context->MaxFoundVelocity) Context->MaxFoundVelocity = Context->Velocity;
 if(Context->Velocity < Context->MinFoundVelocity) Context->MinFoundVelocity = Context->Velocity;
 
 Context->CurrentTime += Context->dTime;
}

void 
IntegrationStep(integration_context *Context, v2 Jerk){
 Context->PositionV2 += (Context->VelocityV2*Context->dTime +
                         0.5*Context->AccelerationV2*Square(Context->dTime) + 
                         (1.0/6.0)*Jerk*Cube(Context->dTime) );
 Context->VelocityV2 += (Context->AccelerationV2*Context->dTime +
                         0.5*Jerk*Square(Context->dTime) );
 Context->AccelerationV2 += Jerk*Context->dTime;
 
#if 0 
 if(Context->Acceleration > Context->MaxFoundAcceleration) Context->MaxFoundAcceleration = Context->Acceleration;
 if(Context->Acceleration < Context->MinFoundAcceleration) Context->MinFoundAcceleration = Context->Acceleration;
 
 if(Context->Velocity > Context->MaxFoundVelocity) Context->MaxFoundVelocity = Context->Velocity;
 if(Context->Velocity < Context->MinFoundVelocity) Context->MinFoundVelocity = Context->Velocity;
#endif
 
 Context->CurrentTime += Context->dTime;
}

void 
IntegrationPrintResults(integration_context *Context){
 printf("Final time: %f\n", Context->CurrentTime-Context->CurrentTime);
 printf("Total time: %f\n", Context->EndTime);
 
 printf("\nTheoretical min and max Acceleration: %f %f\n", -Context->TheoreticalMaxAcceleration, Context->TheoreticalMaxAcceleration);
 printf(  "Actual min and max Acceleration:      %f %f\n", Context->MinFoundAcceleration, Context->MaxFoundAcceleration);
 printf(  "Theoretical min and max Velocity:     %f %f\n", -Context->TheoreticalMaxVelocity, Context->TheoreticalMaxVelocity);
 printf(  "Actual min and max Velocity:          %f %f\n", Context->MinFoundVelocity, Context->MaxFoundVelocity);
 
 printf("\nFinal Acceleration: %f\n", Context->Acceleration);
 printf(  "Final Velocity:     %f\n", Context->Velocity);
 printf(  "Final Position:     %f\n", Context->Position);
 
}

void 
IntegrationPrintResultsV2(integration_context *Context){
 printf("Final time: %f\n", Context->CurrentTime-Context->CurrentTime);
 printf("Total time: %f\n", Context->EndTime);
 
#if 0 
 printf("\nTheoretical min and max Acceleration: %f %f\n", -Context->TheoreticalMaxAcceleration, Context->TheoreticalMaxAcceleration);
 printf(  "Actual min and max Acceleration:      %f %f\n", Context->MinFoundAcceleration, Context->MaxFoundAcceleration);
 printf(  "Theoretical min and max Velocity:     %f %f\n", -Context->TheoreticalMaxVelocity, Context->TheoreticalMaxVelocity);
 printf(  "Actual min and max Velocity:          %f %f\n", Context->MinFoundVelocity, Context->MaxFoundVelocity);
#endif
 
 printf("\nFinal Acceleration: (%f, %f)\n", Context->AccelerationV2.X, Context->AccelerationV2.Y);
 printf(  "Final Velocity:     (%f, %f)\n", Context->VelocityV2.X, Context->VelocityV2.Y);
 printf(  "Final Position:     (%f, %f)\n", Context->PositionV2.X, Context->PositionV2.Y);
 
}
