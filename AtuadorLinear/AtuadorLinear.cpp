#include "AtuadorLinear.h"
#include "math.h"
#include "mbed.h"


// Public Methods /////////////////////////////////

// Construtor
AtuadorLinear::AtuadorLinear(PinName habilitaPin, PinName sentidoPin, PinName feedbackPin) : _habilitaPin(habilitaPin), _sentidoPin(sentidoPin), _feedbackPin(feedbackPin){   
    _habilitaPin = 0;
    _sentidoPin = 0;
    max_leitura = 0.820f; //0.810
    min_leitura = 0.200f; //0.190
    amplitude = max_leitura - min_leitura;
    resolucao = amplitude/105.0f; //103.0
    //tolerancia = floor(resolucao*1000.0) / 1000.0;    //for 1mm
    //tolerancia = 0.005;    
    tolerancia = 0.002;
    //tolerancia = 0.007; 
    iterador = 0;
    printf("tolerancia %f\r\n", tolerancia);
}

// Verifica leitura do potenciometro interno do Atuador
float AtuadorLinear::getFeedback(){
    float feed_vector[50];
    for (int i=0; i< 50; i++){
        feed_vector[i] = _feedbackPin.read();
        //printf("feed_vector %f\r\n", feed_vector[i]);
    }
    // Utiliza moda para ignorar eventuais leituras erradas
    feedback_atual = moda(feed_vector);
    printf("Feedback: %f\r\n", feedback_atual);
    return feedback_atual;
}

// Verificar comprimento atual do Atuad0.or em milimetros
float AtuadorLinear::getComprimento(){
    getFeedback();
    comprimento_atual = (feedback_atual - min_leitura)/resolucao;
    if (comprimento_atual < 0) comprimento_atual = 0; //ignorando leituras negativas
      
    printf("Comprimento: %f\r\n", comprimento_atual);  
    printf("Feed: %f\r\n", feedback_atual);
    return comprimento_atual;
}

// Retrair atuador
void AtuadorLinear::retrair(float comprimento_em_milimetros){
    getFeedback();
   // valor_alvo = 0.0060408*comprimento_em_milimetros+0.191161;
    valor_alvo = resolucao*comprimento_em_milimetros+min_leitura;
    printf("valor_alvo %f\r\n", valor_alvo);
	//ACIONAMENTO STRAIGHTFORWARD: 
	_habilitaPin = 1;
    while (feedback_atual - valor_alvo > tolerancia){
        getFeedback();
        _sentidoPin = 0;
//        _habilitaPin = 0;
    }
  //  wait_ms(100);
//    _sentidoPin = 0; //nova placa recebe 1
	

	//ACIONAMENTO DE 3 RELES:
	_habilitaPin = 0;
	printf("retraiu ate %f\r\n", feedback_atual);
    printf("done retrair\r\n\r\n");
}

// Estender atuador 
void AtuadorLinear::estender(float comprimento_em_milimetros){
    getFeedback();
    //valor_alvo = 0.00595003*comprimento_em_milimetros+0.176624;
    valor_alvo = resolucao*comprimento_em_milimetros+min_leitura;
    printf("valor_alvo: %f\r\n", valor_alvo);
   	//ACIONAMENTO STRAIGHTFORWARD: 
	_habilitaPin = 1;
    while(valor_alvo - feedback_atual > tolerancia) {
        //_habilitaPin = 0;
		//wait_ms();
        getFeedback();
        _sentidoPin = 1;
 //       _habilitaPin = 1;
		//wait(0.025);
        //quebra_loop++;
    }
   // wait_ms(100);
    _habilitaPin = 0;
    //getFeedback();
    printf("estendeu ate: %f\r\n", feedback_atual);
    printf("done estender\r\n\r\n");
}

// Definir novo comprimento do Atuador
float AtuadorLinear::setComprimento(float comprimento_em_milimetros){
    if (comprimento_em_milimetros > 103.0f) comprimento_em_milimetros = 103.0f;
    if (comprimento_em_milimetros < 0.0f) comprimento_em_milimetros = 0.0f;
    valor_alvo = resolucao*comprimento_em_milimetros+min_leitura;
	printf("valor alvo: %f\r\n",valor_alvo);
    //valor_alvo = 0.00598632*comprimento_em_milimetros+0.184311;
    getComprimento();
    int quebra_loop = 0;
//    while(abs(comprimento_em_milimetros - comprimento_atual) > 1.0f){
	//tolerancia anterior 1.5*tolerancia
    while ((fabs(valor_alvo - feedback_atual) > 1.5*tolerancia) && (quebra_loop < 3)){
		wait_ms(100);
        if (comprimento_em_milimetros > comprimento_atual){
            estender(comprimento_em_milimetros);
        }
        else {
            retrair(comprimento_em_milimetros);
        }
        comprimento_atual = getComprimento();
        quebra_loop++;

    }
    //getComprimento();
    printf("done setComprimento\r\n\r\n");
    return feedback_atual;

}

float AtuadorLinear::setAngulo(float angulo_em_graus){
	if (angulo_em_graus > 40.0f) angulo_em_graus = 40.0f;
	if (angulo_em_graus < 0.0f) angulo_em_graus = 0.0f;
	float coef1, coef2, coef3, coef4;
	coef1 = -0.000679;
	coef2 = 0.040733;
	coef3 = 0.915889;
	coef4 = 7;

	float comprimento_alvo = coef1*(pow(angulo_em_graus,3.0)) + coef2*(pow(angulo_em_graus,2.0)) + coef3*angulo_em_graus + coef4;
	printf("comprimento_alvo: %f\r\n", comprimento_alvo);
	getComprimento();
	int quebra_loop = 0;
	while( (fabs(comprimento_alvo - comprimento_atual) > 0.75) && (quebra_loop < 4)){
		setComprimento(comprimento_alvo);
		getComprimento();
		quebra_loop++;
	}

	float a1,a2,a3,a4;
	a1 = 0.000139;
	a2 = -0.014777;
	a3 = 1.085297;
	a4 = -6.150323;

	float angulo_estimado = a1*(pow(comprimento_atual,3.0)) + a2*(pow(comprimento_atual,2.0)) + a3*comprimento_atual + a4;

	printf("angulo_estimado %f\r\n", angulo_estimado);
	printf("done setAngulo\r\n\r\n");
	return angulo_estimado;
}
// Variar comprimento
void AtuadorLinear::variar_comprimento(float variacao_em_milimetros){
    float novo_comprimento = getComprimento() + variacao_em_milimetros;
    if (novo_comprimento > 103.0f) novo_comprimento = 103.0f;
    if (novo_comprimento < 0.0f) novo_comprimento = 0.0f;
    
    printf("novo_comprimento %f\r\n", novo_comprimento);
    setComprimento(novo_comprimento);    
}

// Private Methods //////////////////////////////////////////

//funcao de moda utilizada por getFeedback()
float AtuadorLinear::moda(float *vetor){
    int tamanho = 10;
    int freq_posicao[tamanho];
    for (int k=0; k<tamanho; k++){
        freq_posicao[k]=0;
    }
    int j, k;
    for (j=0; j<tamanho; j++){
        if(vetor[j] != 0){
            for( k=0; k<tamanho; k++){
                if(vetor[j] == vetor[k]){ 
                    freq_posicao[j]++;
                }
            }
        }
    }

    int posicao_moda = 0;
    for( j=0; j<tamanho; j++){
    //printf("freq_posicao: %d\n", freq_posicao[j]);
        if (freq_posicao[posicao_moda] < freq_posicao[j]){
            posicao_moda = j;
        }
    }
    
    return vetor[posicao_moda];
}           
