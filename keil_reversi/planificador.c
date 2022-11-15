#include "planificador.h"
#include "tableros.h"
#include "conecta4_2022.h"

static int permisoDescanso = 0;

void planificador(void){
    
    // definimos estructuras iniciales 
	static volatile uint8_t entrada[8] = {0, 0, 0, 0, 0, 0, 0, 0 }; 
    // inicializamos los periféricos 
    uint8_t column, row, colour;
    colour = 1; // empiezan jugador 1 (blancas)
    
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
                    if (evento.auxData == 1){ // EINT1 (realizar la jugada)
                        // leer columna 
                        column = leercolumna(); 
                        // calcular fila
                        row = C4_calcular_fila(cuadricula_1, column);
                        if(C4_fila_valida(row) && C4_columna_valida(column)) {
                            C4_actualizar_tablero(cuadricula_1,row,column,colour); //actualiza el tablero
                            if(C4_verificar_4_en_linea(cuadricula_1, row, column, colour)) {
                                while(1); //ganas la partida
                            }
                            if (C4_comprobar_empate(cuadricula_1)){
                                while(1); //quedan en empate los dos jugadores
                            }
                        }  // jugada invalida 
                        colour = cambioColor(colour);
                    } else {                  // se reinicia el juego 

                    }
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
                break;
                default: break;
            }
        }
        /*
        if(permisoDescanso == 1){
            idle();
        }
        else if (cola_vacia()){
            cola_encolar_evento(Suspender, 0, 0);
        }*/
    }
}
