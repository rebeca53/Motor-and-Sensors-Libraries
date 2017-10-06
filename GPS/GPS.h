#ifndef GPS_H
#define GPS_H

#include "mbed.h"

class GPS{
	public:
		GPS (PinName tx, PinName rx);
		void leitura_simples();
		void leitura_moda(int numero_de_amostras);
		
		float getLatitude();
		float getLongitude();
		
		void setDeclination(float new_declination);
		
	private:	
		Serial gps;
		float latitude, longitude, declination;
		float moda(float *vetor, int numero_de_amostras);

		char ns, ew;                                    // fix quality, Number of satellites being tracked, 3D fix
		int testData, bufferGps, aux = 0, aux1 = 0, dataValid;
		char gpsData[500], c, lat[20], lon[20];	
};
#endif
