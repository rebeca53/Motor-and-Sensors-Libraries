#include "GY80.h"
#include "mbed.h"

GY80::GY80(PinName sda, PinName scl): gyroscope(sda,scl), magnetometer(sda,scl), accelerometer(sda,scl){
	//inicializa accelerometer
	printf("Starting ADXL345 test...\n");
	wait(.001);
	printf("Device ID is: 0x%02x\n", accelerometer.getDeviceID());
	wait(.001);
    
 	// These are here to test whether any of the initialization fails. It will print the failure
 	if (accelerometer.setPowerControl(0x00)){
		printf("didn't intitialize power control\n"); 
	}
     	//Full resolution, +/-16g, 4mg/LSB.
     	wait(.001);
     
     	if(accelerometer.setDataFormatControl(0x0B)){
		printf("didn't set data format\n");
        }
     	wait(.001);
     
  	//3.2kHz data rate.
     	if(accelerometer.setDataRate(ADXL345_3200HZ)){
		printf("didn't set data rate\n");
        }
     	wait(.001);
     
     	//Measurement mode.
     
     	if(accelerometer.setPowerControl(MeasurementMode)) {
		printf("didn't set the power control to measurement\n"); 
        } 
		
	t.start();
	now_comp = 0;
	before_comp = 0;
}

double GY80::complementaryFilter(double newAngle, double newRate, float alpha, double previous_angle){

//http://www.pieter-jan.com/node/11
	double complementary_angle = previous_angle;
	now_comp = t.read_us();
	deltat = (double) ((now_comp - before_comp)/1000000.0f);
	before_comp = now_comp;

	complementary_angle = (complementary_angle + newRate*deltat)*alpha + (1-alpha)*newAngle;
	return complementary_angle;
}

