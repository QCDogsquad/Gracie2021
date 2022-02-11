/* date = August 17th 2021 7:57 pm */

#ifndef INTEGRATION_H
#define INTEGRATION_H

struct integration_context {
 union {
  f64 Position     = 0.0;
  v2  PositionV2;
 };
 union {
  f64 Velocity     = 0.0;
  v2  VelocityV2;
 };
 union {
  f64 Acceleration = 0.0;
  v2  AccelerationV2;
 };
 
 f64 TheoreticalMaxAcceleration = 0.0f;
 f64 TheoreticalMaxVelocity = 0.0f;
 
 f64 MaxFoundAcceleration = -INFINITY;
 f64 MaxFoundVelocity     = -INFINITY;
 f64 MinFoundAcceleration = INFINITY;
 f64 MinFoundVelocity     = INFINITY;
 
 f64 CurrentTime = 0.0f;
 
 f64 dTime = 0.0001;
 f64 EndTime = 0.0;
};

#endif //INTEGRATION_H
