#ifndef __KALAMAN_H_
#define __KALAMAN_H_
#include "Variable.h"




void Kalaman_feedback(KalmanFilter_t *kf,float dt,float z0,float z1);
void Kalman_Filter_Init(KalmanFilter_t *kf, uint8_t xhatSize, uint8_t uSize, uint8_t zSize);


extern KalmanFilter_t Kalman0;
extern KalmanFilter_t Kalman1;
extern KalmanFilter_t Kalman2;
#endif

