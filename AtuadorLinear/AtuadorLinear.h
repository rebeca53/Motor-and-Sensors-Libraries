#ifndef ATUADORLINEAR_H
#define ATUADORLINEAR_H

#include "mbed.h"
/* Biblioteca para acionamento de Atuador Linear de Pequeno Porte KTC-D24
*  Data: Junho/2017
*  Autora: Rebeca N Rodrigues, Manaus Instituto de Tecnologia 
*  Conexoes: Alimentação (V+ e V-) ligados ao circuito de acionamento.
*            Feedback: Amarelo - 3.3v, Branco - GND, Azul - pino de Feedback
*/

class AtuadorLinear {
    public:
        /* Metodo Construtor
        *
        * @param habilitaPin - 	pino usado para habilitar acionamento do Atuador. Deve ser o nível
		* 			lógico contrário ao do sentido para habilitar
        * @param sentidoPin - pino usado para definir sentido (retração/extensão) do acionamento
        * @param feedbackPin - pino usado para leitura do potenciometro interno
        */
        AtuadorLinear(PinName habilitaPin, PinName sentidoPin, PinName feedbackPin);
        
        /* Verifica leitura do potenciometro interno do Atuador
        * Armazena resultado na variavel privada "feedback_atual"
        */
        float getFeedback();
        
        /* Verificar comprimento atual do Atuador em milimetros
        * Armazena resultado na variavel privada "comprimento_atual"
        */
        float getComprimento();
        
        /* Função para retrair atuador até o comprimento desejado
        * 
        * @param comprimento_em_milimetros - passa para a funcao o comprimento desejado
        */
        void retrair(float comprimento_em_milimetros);

        /* Função para estender atuador até o comprimento desejado
        * 
        * @param comprimento_em_milimetros - passa para a funcao o comprimento desejado
        */        
        void estender(float comprimento_em_milimetros);
        
        /* Função acionar/ definir novo comprimento do atuador
        *
        * @param comprimento_em_milimetros - indica comprimento desejado para atuador
        */
        float setComprimento(float comprimento_em_milimetros);

        /* Função acionar/ definir novo angulo de elevacao utilizando atuador
        *
        * @param angulo_em_graus - indica angulo de elevacao desejado em graus
        */
        float setAngulo(float angulo_em_graus);

        /* Função para variar comprimento do atuador
        *
        * @param variacao_em_milimetros - indica de quanto o comprimento do atuador deve mudar
        */
        void variar_comprimento (float variacao_em_milimetros);
        
    private: 
        DigitalOut _habilitaPin;
        DigitalOut _sentidoPin;
        AnalogIn _feedbackPin;
        float feedback_atual;
        float comprimento_atual;
        float max_leitura;
        float min_leitura;
        float amplitude;
        float resolucao;
        float tolerancia;
        int iterador;
        float valor_alvo;
        float moda(float *vetor);
};

#endif
