#include "planificador.h"
#include "tableros.h"
#include "conecta4_2022.h"
#include <stdio.h>

void planificador(void){

    // inicializamos los periféricos 
    bool permiso = true; 
    uint8_t column, row, colour, i, j;
    colour = 1; // empiezan jugador 1 (blancas)
    
    temporizador_reloj(1);  // Indicamos que queremos que las interrupciones del timer0 generen un evento cada 1 ms
    init_Parametros_GA();
    jugadaNoValidaInit();
    // colocamos la alarma para pasar a modo apagado 
    cola_encolar_evento(Suspender, 0, 0);
    // guardamos una copia del tabero
    CELDA tablero[TAM_FILS][TAM_COLS];
    for(i = 0; i<TAM_FILS; i++){
        for(j = 0; j<TAM_COLS; j++){
            tablero[i][j] = cuadricula_victoria_j2[i][j];
        }
    }
    
    while(1){
        while(!cola_vacia()){  
            elemento evento;
            cola_desencolar_evento(&evento.id, &evento.auxData);
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
                        // Iniciamos timer1 para medir el tiempo de procesamiento de comprobar si hay línea en
                        // microsegundos. Es decir, desde que el jugador ha introducido su
                        // movimiento, hasta comprobar si hay línea o empate.
                        temporizador_empezar();
                        // calcular fila
                        row = C4_calcular_fila(cuadricula_victoria_j2, column);
                        if(C4_fila_valida(row) && C4_columna_valida(column)) {
                            actualizarJugada(cuadricula_victoria_j2,row,column,colour);
                            temporizador_leer();
                            if(C4_verificar_4_en_linea(cuadricula_victoria_j2, row, column, colour)) {
                                endgame(colour);  //ganas la partida
                                while(1);
                            }
                            if (C4_comprobar_empate(cuadricula_victoria_j2)){
                                endgame(3);  //quedan en empate los dos jugadores
                                while(1);
                            }
                        }  // jugada invalida 
                        colour = cambioColor(colour);
                        temporizador_parar();
                    } else {                  // se reinicia el juego 
                        cola_iniciar();
                        initgame(); 
                        for(i = 0; i<TAM_FILS; i++){
                            for(j = 0; j<TAM_COLS; j++){
                                cuadricula_victoria_j2[i][j] = tablero[i][j];
                            }
                        }
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
