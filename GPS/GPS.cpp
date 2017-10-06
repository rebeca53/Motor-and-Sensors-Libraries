#include "GPS.h"
#include "mbed.h"

GPS::GPS(PinName tx, PinName rx) : gps(tx,rx){
	gps.baud(9600);
	printf("Aquecendo...");
}

void GPS::leitura_simples(){
	dataValid = 0;

	//verifica chegada da informacao
	if(gps.readable()){
		for(int i=0; i<500; i++){
			c = gps.getc();
			if(c=='\r'){
				i = 500;
			}
			else{
				gpsData[i] = c;
				//printf("%c", c); //impressao para debug
			}
		}
	}
	else{
		printf("Sem dados de GPS\r\n");
		gpsData[0] = ' ';
	}


	//interpreta informacao
	if (gpsData[0] != ' '){
		bufferGps = strlen(gpsData);
		lat[0] = ' ';
		lon[0] = ' ';
		ns = '0';
		ew = '0';
		for (int i=0; i<bufferGps; i++){
			if(gpsData[i] =='$'){
				if((gpsData[i+1] == 'G')&&(gpsData[i+2] == 'P')&&(gpsData[i+3] == 'R')&&(gpsData[i+4] == 'M')&&(gpsData[i+5] == 'C')){
				    for(int j = i+7; j < bufferGps; j++){
				        if((gpsData[j] == ',') && (aux1 == 0)){
				            aux1 = 1;
				        }else if((gpsData[j] == ',') && (aux1 == 1) && (aux == 0)){
				            for(int k = 0; k < bufferGps; k++){
				                if(gpsData[j+1+k] != ','){
				                    lat[k] = gpsData[j+1+k];
				                    //pc.printf("%c", lat[k]);
				                } else{
				                    ns = gpsData[j+1+k+1];
				                    //pc.printf(", %c, ", ns);
				                    aux = j+1+k+1;
				                    k = 500;
				                }
				            }
				        j = aux;
				        } else if((gpsData[j] == ',') && (aux1 == 1) && (aux != 0)){
				            for(int k = 0; k < bufferGps; k++){
				                if(gpsData[j+1+k] != ','){
				                    lon[k] = gpsData[j+1+k];
				                   // pc.printf("%c", lon[k]);
				                } else{
				                    ew = gpsData[j+1+k+1];
				                 //   pc.printf(", %c\n\r\n\r", ew);
				                    aux = 0;
				                    k = 500;
				                }
				            }
				        j = 500;
				        aux1 = 0;
				        }
				    }
				    for(int j = 0; j < bufferGps; j++){
				        if((gpsData[j] == ',') && (gpsData[j+1] == 'A') && (gpsData[j+2] == ',')){
				            dataValid = 1;
				        }
				    }
				}
			    }
			}
		}

		//calculo das coordenadas
		if ((lat[0] != ' ')&&(lon[0]!=' ')&&(ns != '0')&&(ew != '0')&&(dataValid==1)){
			char str[2];
			char strMin[7];
			str[0] = lat[0]; str[1] = lat[1];
			float latGraus = atof(str);
			for(int i = 0; i < 7; i++){
				strMin[i] = lat[i+2];
			}
			float latMin = atof(strMin);
			latitude = latGraus + (latMin / 60.0f);
			if (ns=='S'){
				latitude = latitude*(-1.0f);
			}
			printf("Latitude: %f\r\n", latitude);

			char str1[3];
			str1[0] = lon[0]; str1[1] = lon[1]; str1[2] = lon[2];
			float lonGraus = atof(str1);
			for(int i = 0; i < 7; i++){
				strMin[i] = lon[i+3];
			}
			float lonMin = atof(strMin);
			longitude = lonGraus + (lonMin/60.0f);
			if(ew == 'W'){
				longitude = longitude*(-1.0f);
			}
			printf("Longitude: %f\r\n", longitude);
		}
	}

void GPS::leitura_moda(int numero_de_amostras){
	float latitudeTemp[numero_de_amostras], longitudeTemp[numero_de_amostras];
	for (int i=0; i<numero_de_amostras; i++){
		leitura_simples();
		latitudeTemp[i] = latitude;
		longitudeTemp[i] = longitude;
	}

	latitude = moda(latitudeTemp, numero_de_amostras);
	longitude = moda(longitudeTemp, numero_de_amostras);
}

float GPS::getLatitude(){
	return latitude;
}
float GPS::getLongitude(){
	return longitude;
}

void GPS::setDeclination(float new_declination){
	declination = new_declination;
}

float GPS::moda(float *vetor, int numero_de_amostras){
	int tamanho = numero_de_amostras;
	int freq_posicao[tamanho];
	for(int k=0; k<tamanho; k++){
		freq_posicao[k]=0;
	}

	int j,k;
	for (j=0; j<tamanho; j++){
		if(vetor[j] != 0){
			for(k=0; k<tamanho; k++){
				if(vetor[j] == vetor[k]){
					freq_posicao[j]++;
				}
			}
		}
	}

	int posicao_moda =0;
	for(j=0; j<tamanho; j++){
		if(freq_posicao[posicao_moda] < freq_posicao[j]){
			posicao_moda = j;
		}
	}

	return vetor[posicao_moda];
}
