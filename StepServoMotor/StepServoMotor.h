#ifndef STEPSERVOMOTOR_H
#define STEPSERVOMOTOR_H

#include "mbed.h"
/* Biblioteca para acionamento de StepServo Motor Kalatec utilizando driver KTC-DR34EC
*  Data: Junho/2017
*  Autora: Rebeca N Rodrigues, Manaus Instituto de Tecnologia 
*  Conexoes: Circuitos amplificadores com BC547 para enviar sinais de controle
*/

class StepServo {
    public:
        /* Metodo Construtor
        *
        * @param pinPulso - pino usado enviar pulso PWM
        * @param pinDir - pino usado para definir direção (horario/antihorario) do acionamento
		* @param pinFeedback - pino usado para contagem de pulsos PWM
        */
        StepServo(PinName pinPulso, PinName pinDir, PinName pinFeedback);

        /* Função para rotacionar motor no sentido horario
        * 
        * @param numero_de_passos - passa para a funcao o numero de passos desejado
        */
        float moverHorario(int numero_de_passos);    
    
        /* Função para rotacionar motor no sentido anti-horario
        * 
        * @param numero_de_passos - passa para a funcao o numero de passos desejado
        */
        float moverAntihorario(int numero_de_passos);
    
        /* Função acionar/ definir novo angulo de posicao do stepservo motor
        *
        * @param angulo_em_graus - indica angulo entre 0 e 360 desejado para o motor
        */
        float setAngulo(float angulo_em_graus);
        
        /* Configurar parametro de numero de microsteps. Atualiza resolucao.
        *
        * @param numero_de_microsteps - indica numero de microsteps 
        * os valores possiveis sao verificados no manual do driver
        */
        void setMicroSteps(int numero_de_microsteps);

        /* Configurar parametro de periodo do sinal de PWM 
        *
        * @param periodo - valor inteiro que indica o periodo do sinal
        * @param modo_tempo - indica se a escala de tempo é em us ou ms
        *                   0 - milissegundos
        *                   1 - microssegundos
        */    
        void setPeriodoPwm(int periodo, int modo_tempo);
        
        /* Atualizar variavel que guarda posicao atual do motor
        * 
        * @param posicao_em_graus - indica angulo da posicao atual em graus
        *                   o valor deve estar entre 0 e 360 graus
        */
        void setPosicao(float posicao_em_graus);
    
        /* Função estática para converter angulos do intervalo de -180 a 180 para
        *   o intervalo de 0 a 360 graus.
        *
        * @param angulo_original - angulo entre -180 e 180 a ser convertido
        */
         float converterPara360(float angulo_original);
    
private:
    PwmOut _pinPulso;
    DigitalOut _pinDir;
    InterruptIn _pinFeedback;
    int modo; //0 para ms, 1 para us
    int tempoPulso;
    float microSteps;
    float resolucao;
    float posicao_atual;    
    volatile int contador;
    
    /* Método associado à interrupção do pino de Feedback para contar
    * número de pulso enviados ao motor.
    *
    */
    void contarPassos();
};

#endif
