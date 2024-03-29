#include "planificador.h"
#include "funciones_swi.h"


void planificador(void){
    // inicializamos los periféricos 
    bool permiso = true; 
    conecta4_init();
    temporizador_reloj(1);  // Indicamos que queremos que las interrupciones del timer0 generen un evento cada 1 ms
    init_Parametros_GA();
    jugadaNoValidaInit();
    RTC_init(); 
    WD_init(1);
    WD_feed();
    // colocamos la alarma para pasar a modo apagado 
    uint32_t leidoFIQ = read_FIQ_bit();
    if (leidoFIQ == 0){
        disable_fiq(); 
    }
    cola_encolar_evento(Suspender, 0, 0);
    if (leidoFIQ == 0){
        enable_fiq(); 
    } 
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
                    leidoFIQ = read_FIQ_bit();
                    cola_encolar_evento(Suspender, 0, 0); // Cuando se pulsa un boton se reprograma la alrma de power_down
                    permiso = terminarLatido();
                    gestor_botones(evento.auxData);
                    if (evento.auxData == 1){ // EINT1 (cancelar la jugada)
                        cancelarJugada();
                        conecta4_seguir(0);       
                    } else {                  // boton 2 - se reinicia el juego 
                        resetearJuego();
                        conecta4_acabarPorBoton();
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
                    GSERIE_continuar_envio();
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
            cola_desencolar_mensaje(&msg.id, &msg.auxData, &msg.numInt);
            conecta4_ActualizarTiempoMedio(msg.numInt);
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
                        case WATCHDOG:
                            WD_feed();
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
