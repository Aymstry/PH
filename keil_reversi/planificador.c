#include <stdio.h>
#include "RTC.h"
#include "planificador.h"
#include "tableros.h"
#include "conecta4_2022.h"


void planificador(void){

    // inicializamos los periféricos 
    bool permiso = true; 
		int time = 0;
    uint8_t column;
    temporizador_reloj(1);  // Indicamos que queremos que las interrupciones del timer0 generen un evento cada 1 ms
    init_Parametros_GA();
    jugadaNoValidaInit();
    RTC_init(); 
    // colocamos la alarma para pasar a modo apagado 
    cola_encolar_evento(Suspender, 0, 0);
    conecta4_recuperar_tablero(); 
    
    while(1){
        
        while(!cola_vacia()){  
            elemento evento;

            uint32_t leidoIRQ = read_IRQ_bit();
            uint32_t leidoFIQ = read_FIQ_bit();
            disable_irq_fiq();  //Deshabilitamos las interrupciones
            cola_desencolar_evento(&evento.id, &evento.auxData);
            if (leidoIRQ == 1 && leidoFIQ == 1){
                enable_irq_fiq();
            } else if (leidoIRQ == 1){
                enable_irq();
            }

            switch(evento.id){
                case T0: 
                    gestor_alarmas();
                    break;
                case BotonPulsado:
                    time = RTC_read_time(); 
                    time = time + clock_get_us();
                    cola_encolar_evento(Suspender, 0, 0); // Cuando se pulsa un boton se reprograma la alrma de power_down
                    permiso = terminarLatido();
                    gestor_botones(evento.auxData);
                    if (evento.auxData == 1){ // EINT1 (realizar la jugada)
                        // leer columna 
                        column = leercolumna();
                        // Comenzamos el juego
                        conecta4_jugar(column);
                        // si se gana la partida se apaga el sistema
                        if( conecta4_ganado_empate() == true){
                            power_down();
                        }
                    } else {                  // se reinicia el juego 
                        cola_iniciar();
                        initgame();  
                        conecta4_resetear_juego();       
                    }
                    break;
                case CancelarAlarma:
                    desactivarAlarma(evento.auxData);
                    break;
                case Suspender: 
                    introducir_power();
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
                        case JugadaRealizada:
                            ApagarLedConfirmacion(); 
                            break; 
                       case JugadaNoValida:
                            permiso = actualizarAviso(cuadricula_victoria_j2); 
                            break;
                        case MIdle:
                            parpadeoBlinBlin();
                            break;
                        default: break;
                    } 
                    break;
                default: break;
            }
        }
        
        if(cola_vacia() && permiso){   
            empezarLatido(); 
            permiso = false; 
        } else if(cola_vacia() && cola_vacia_mensaje() ){
            idle();
        }      

    }
}
