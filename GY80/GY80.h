#ifndef GY80_H
#define GY80_H

#include "mbed.h"
#include "L3G4200D.h"
#include "HMC5883L.h"
#include "ADXL345_I2C.h"
/*
This file calls the library of each sensor inside GY80 so all sensors can be accessed
from the same object.
It also implements a complementary filter. A complementary filter is a simpler 
substitute for a Kalman Filter.
*/
class GY80{
	public:
		L3G4200D gyroscope;
		HMC5883L magnetometer;
		ADXL345_I2C accelerometer;
	
		GY80(PinName sda, PinName scl);

		double complementaryFilter(double newAngle, double newRate, float alpha, double previous_angle);
	private:
		int now_comp;
		int before_comp;
		double deltat;
		Timer t;
};

#endif
