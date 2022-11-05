#include "planificador.h"

static int permisoDescanso = 0;

void planificador(void){

    temporizador_reloj(1);  // Indicamos que queremos que las interrupciones del timer0 generen un evento cada 1 ms
    init_Parametros_GA();

    while(1){
        while(!cola_vacia()){
            permisoDescanso = 0;    
            elemento evento;
            cola_desencolar_evento(&evento.id, &evento.auxData);
            switch(evento.id){
                case T0: 
                    gestor_alarmas();
                    break;
                case BotonPulsado:
                    gestor_botones(evento.auxData);
                    break;
                case CancelarAlarma:
                    desactivarAlarma(evento.auxData);
                    break;
                case Suspender: 
                    introducir_power();
                    permisoDescanso = 1;
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
                case Alarma_Vencida: // aqui avisaremos a quien le corresponde
                    switch(msg.auxData){
                        case BP1:
                            comprobar_eint1(); 
                            break;
                        case BP2:
                            comprobar_eint2(); 
                            break; 
                        case Sleep:
                            dormir();
                            break; 
                    } 
                default: break;
            }
        }

        if(permisoDescanso == 1){
            idle();
        }
        else if (cola_vacia()){
            cola_encolar_evento(Suspender, 0, 0);
        }
    }
}
