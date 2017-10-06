#include "StepServoMotor.h"
#include "mbed.h"

StepServo::StepServo(PinName pinPulso, PinName pinDir, PinName pinFeedback) : _pinPulso(pinPulso), _pinDir(pinDir), _pinFeedback(pinFeedback) {
    _pinPulso.write(0.0f);
    _pinPulso.period_ms(8);
    _pinDir = 0;    
    _pinFeedback.rise(this, &StepServo::contarPassos);
    modo = 0;
    tempoPulso = 8;
    microSteps = 8000;
    resolucao = 360.0f/microSteps;
    posicao_atual = 0.0f;
}
    
    
float StepServo::moverHorario(int numero_de_passos){
    _pinDir = 1;
    contador = 0;
	printf("mover horario\r\n");
//ALERTA DE GAMBI
//	numero_de_passos = 2*numero_de_passos;
    while(contador < numero_de_passos){
        _pinPulso.write(0.5f);
        if (modo == 0)
            wait_ms(tempoPulso);
        else
            wait_us(tempoPulso);
    }
    _pinPulso.write(0.0f);
 //   float variacao_em_angulo = resolucao*contador/2; //GAMBI - dividir por 2
    float variacao_em_angulo = resolucao*contador; 
    
    return variacao_em_angulo;
}
    
    
float StepServo::moverAntihorario(int numero_de_passos){
    _pinDir = 0;
    contador = 0;
	printf("mover antihorario\r\n");
	//ALERTA DE GAMBI
	numero_de_passos = 2*numero_de_passos;
//    int i;
//    for (i=0; i < numero_de_passos; i++){
    while(contador < numero_de_passos){
        _pinPulso.write(0.5f);
        if (modo == 0)
            wait_ms(tempoPulso);  
        else
            wait_us(tempoPulso);
    }
    _pinPulso.write(0.0f);    
    float variacao_em_angulo = resolucao*contador/2; //GAMBI - dividir por 2
  //  float variacao_em_angulo = resolucao*contador;

    return variacao_em_angulo;
}
  
    
float StepServo::setAngulo(float angulo_em_graus){
    //redefinir periodo just in case (todos os pinos pwm apresentam mesmo periodo
    // o servo apresenta periodo de pwm diferente)
    setPeriodoPwm(tempoPulso,modo);
   // printf("set angulo: %f\r\n", angulo_em_graus);
   // printf("posica_atual: %f\r\n", posicao_atual);
    //caso a entrada esteja fora do intervalo [0,360]
    if ((angulo_em_graus > 360.0f) || (angulo_em_graus < 0.0f)){
        printf("Insira valor dentro do intervalo [0,360]\r\n");
        return posicao_atual;
    }
          
    //calcular angulo a ser percorrido
    float angulo_variacao = angulo_em_graus - posicao_atual;
   // printf("angulo_variacao: %f\r\n", angulo_variacao);
          
    if (angulo_variacao > 180.0f) {
        angulo_variacao = angulo_variacao - 360.0f;
    }else if (angulo_variacao < -180.0f) {
        angulo_variacao = angulo_variacao + 360.0f;
    }

    printf("angulo a variar: %f\r\n", angulo_variacao);
    float variacao_real;
    
    while( fabs(angulo_variacao) > resolucao){        
        //definir sentido de giro
        if (angulo_variacao >= 0){
            int passos = fabs(floor(angulo_variacao/resolucao));
			printf("passos %d\r\n", passos);
            variacao_real = moverHorario(passos);
        }else{
            int passos = fabs(floor(angulo_variacao/resolucao));
			printf("passos %d\r\n", passos);
            variacao_real = -moverAntihorario(passos);
        }
            
        posicao_atual = posicao_atual+variacao_real;
		printf("posical_atual %f\r\n", posicao_atual);
        if (posicao_atual > 360.0f) {
            posicao_atual = posicao_atual - 360.0f;
        }else if (posicao_atual < 0.0f){
            posicao_atual = posicao_atual + 360.0f;    
        }
        printf("numero de passos reais: %d \r\n", contador);
      //  printf("variacao_real %f\r\n", variacao_real);
      //  printf("angulo_atual %f\r\n", posicao_atual);
    
    
        angulo_variacao = angulo_em_graus - posicao_atual;      
        if (angulo_variacao > 180.0f) {
            angulo_variacao = angulo_variacao - 360.0f;
        }else if (angulo_variacao < -180.0f) {
            angulo_variacao = angulo_variacao + 360.0f;
        }
    }
    return posicao_atual;
}

    
void StepServo::setMicroSteps(int numero_de_microsteps){
    microSteps = numero_de_microsteps;
    resolucao = 360.0f/microSteps;    
    printf("Resolucao atual: %f\r\n", resolucao);
}
    
    
void StepServo::setPeriodoPwm(int periodo, int modo_tempo){
    modo = modo_tempo;
        
    if (modo == 0){
        _pinPulso.period_ms(periodo);    
    }
    else {
        _pinPulso.period_us(periodo);
    }
        
    tempoPulso = periodo;
}
    
    
//calibrar com sensor de curso
void StepServo::setPosicao(float posicao_em_graus){
    if (posicao_em_graus < 0 || posicao_em_graus > 360.0f){
        printf("Insira posicao em graus no intervalo [0.0f,360.0f]\r\n");
        return;
    }
    posicao_atual = posicao_em_graus;    
}
    
float StepServo::converterPara360(float angulo_original){
    if (fabs(angulo_original) > 180.0f){
        printf("Insira um valor no intervalo [-180.0,180.0]\r\n");
        return 0;
    }
            
    float angulo_convertido;
    if (angulo_original < 0.0f){
        angulo_convertido = angulo_original + 360.0f;
    }
    else {
        angulo_convertido = angulo_original;    
    }
    
    return angulo_convertido;
}


void StepServo::contarPassos() {
    contador++;
//printf("contador %d\r\n", contador);
}
