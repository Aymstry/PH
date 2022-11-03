#include "cola_asyn.h"
#include "msg.h"
#include "eventos.h"
#include "G_Alarm.h"
#include "timer.h"

void planificador(){

    temporizador_reloj(1);  // Indicamos que queremos que las interrupciones del timer0 generen un evento cada 1 ms

    while(!cola_vacia()){
        elemento evento;
        cola_desencolar_eventos(&evento.id, &evento.auxData);
        switch(evento.id){
            case T0: gestor_alarmas();
        }
    }

    while(!cola_vacia_mensaje()){
        elemento msg;
        cola_desencolar_mensaje(&msg.id, &msg.auxData);
        switch(msg.id){
            case Set_Alarma: organizador_alarmas(msg.auxData); 
            case Alarma_Vencida: while(1);// aqui avisaremos a quien le corresponde de algún modo que no sabemos 
        }
    }
}