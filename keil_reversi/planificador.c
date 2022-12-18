#include <stdio.h>
#include "RTC.h"
#include "planificador.h"
#include "tableros.h"
#include "conecta4_2022.h"
#include "funciones_swi.h"
#include "Uart0.h"
#include "g_serie.h"
#include <inttypes.h>




void planificador(void){

    // inicializamos los periféricos 
    bool permiso = true; 
    uint8_t column;
    conecta4_init();
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

            disable_irq_fiq();  //Deshabilitamos las interrupciones
            cola_desencolar_evento(&evento.id, &evento.auxData);
            enable_irq_fiq();
           

            switch(evento.id){
                case T0: 
                    gestor_alarmas();
                    break;
                case BotonPulsado:
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
                    } else {                  // boton 2 - se reinicia el juego 
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
                case UART0_ESCRITO:
                    GSERIE_procesarEntrada(evento.auxData);
                    break;
                case UART0_LEIDO:
                    uart0_continuar_envio();
                    break;
                case FIN: 
                    conecta4_init();
                    idle();
                    // if aux.Data == 1 se han rendido END             
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
                            permiso = actualizarAviso(cuadricula); 
                            break;
                        case MIdle:
                            parpadeoBlinBlin();
                            break;
                        case CONECTA4:      // no se ha cancelado la jugada 
                            conecta4_seguir(1);
                            break;
                        default: break;
                    } 
                    break;
                case COMANDO:
                    conecta4_tratamientoComando(msg.auxData);
                    break;
                case GSERIE_IMPRIMIR:
                    sendchar(msg.auxData); 
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
