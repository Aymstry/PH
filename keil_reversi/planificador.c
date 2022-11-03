#include "planificador.h"

void planificador(void){

    temporizador_reloj(1);  // Indicamos que queremos que las interrupciones del timer0 generen un evento cada 1 ms
    init_Parametros_GA();

    while(1){
        while(!cola_vacia()){
            elemento evento;
            cola_desencolar_evento(&evento.id, &evento.auxData);
            switch(evento.id){
                case T0: 
                    gestor_alarmas();
                    break;
                default: break;
            }
        }

        while(!cola_vacia_mensaje()){
            elemento msg;
            cola_desencolar_mensaje(&msg.id, &msg.auxData);
            switch(msg.id){
                case Set_Alarma: 
                    organizador_alarmas(msg.auxData); 
                    break;
                case Alarma_Vencida: while(1);// aqui avisaremos a quien le corresponde de algún modo que no sabemos 
                default: break;
            }
        }
    }
}
